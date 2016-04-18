#pragma once

#include "Picture.h"
#include "BlockBase.h"
#include "Data/CU.h"
#include "Data/TU.h"
#include <stack>

namespace HEVC
{
	template< class T, class self >
	class QuadTree : public BlockBase
	{
	protected:

		void refreshPositionInCTU(size_t ctu_size)
		{
			pos_in_ctu.x = pos.x % ctu_size;
			pos_in_ctu.y = pos.y % ctu_size;

			pos_in_ctu.idx = Calc::zscan_idx(pos_in_ctu.x, pos_in_ctu.y);
		}

		virtual std::unique_ptr<self> createSubtree(size_t x, size_t y) = 0;
		virtual std::unique_ptr<T> createLeaf() = 0;

		void createSubTrees(size_t picWidth, size_t picHeight)
		{
			subtrees(0, 0) = createSubtree(pos.x, pos.y);

			bool createRight = pos.x + size() / 2 < picWidth;
			bool createLower = pos.y + size() / 2 < picHeight;

			if (createRight)
				subtrees(1, 0) = createSubtree(pos.x + (size() / 2), pos.y);

			if (createLower)
				subtrees(0, 1) = createSubtree(pos.x, pos.y + (size() / 2));

			if (createLower && createRight)
				subtrees(1, 1) = createSubtree(pos.x + (size() / 2), pos.y + (size() / 2));
		}

		void clear()
		{
			for (int i = 0; i < 2; ++i)
			{
				for (int j = 0; j < 2; ++j)
				{
					subtrees(i, j).reset();
				}
			}
			leaf.reset();
		}

		std::shared_ptr<T> leaf;
	public:

		Matrix<std::unique_ptr<self>> subtrees;
		Position pos_in_ctu;

		QuadTree(int x, int y, int size) :
			BlockBase(x, y, size),
			subtrees(2, 2)
		{
			clear();
		}
		virtual ~QuadTree() = default;

		void buildSubtrees(size_t picWidth, size_t picHeight)
		{
			clear();
			createSubTrees(picWidth, picHeight);
		}

		void buildLeaf()
		{
			clear();
			leaf = createLeaf();
		}

		bool isLeaf() const { return leaf != nullptr; }
		bool isSplit() const { return leaf == nullptr; }

		template
			<
			class Type,
			class UnqualifiedType = std::remove_cv_t<Type>
			>
			class ZorderIterator
			: public std::iterator < std::forward_iterator_tag,
			UnqualifiedType,
			std::ptrdiff_t,
			Type*,
			Type& >
		{
			struct TreeAddress
			{
				QuadTree* tree;
				int placement;
				TreeAddress(QuadTree* t, int subtree) :
					tree(t), placement(subtree)
				{

				}
			};

			QuadTree* root;
			std::stack<TreeAddress> path;
			std::shared_ptr<Type> current;
			int idx = 0;

			static const int SUBTREE_BEGIN = -1;
			static const int SUBTREE_END = 4;

			void next()
			{
				if (path.empty())
				{
					current = nullptr;
					idx = -1;
					return;
				}

				auto& current_level = path.top();

				if (current_level.placement == SUBTREE_BEGIN)
				{
					if (current_level.tree->isLeaf())
					{
						current = current_level.tree->leaf;
						++idx;
						path.pop();
						return;
					}
				}
				if (current_level.placement < SUBTREE_END)
				{
					current_level.placement += 1; 
					
					if (current_level.placement == SUBTREE_END)
					{
						path.pop();
						next();
						return;
					}
					auto& newtree = current_level.tree->subtrees(current_level.placement % 2, current_level.placement / 2);
					path.push({ newtree.get(),SUBTREE_BEGIN });
					next();
				}
			}

		public:

			void moveToStart()
			{
				while (!path.empty())
				{
					path.pop();
				}
				current = nullptr;
				idx = 0;
				path.push({ root, SUBTREE_BEGIN });

				next();
			}

			void moveToEnd()
			{
				current = nullptr;
				idx = -1;
				while (!path.empty())
				{
					path.pop();
				}
			}

			explicit ZorderIterator(QuadTree* source) :
				root(source)
			{
				moveToStart();
			}

			void swap(ZorderIterator& other)
			{
				using std::swap;
				swap(current, other.current);
				swap(path, other.path);
				swap(root, other.root);
			}

			ZorderIterator& operator++ ()
			{
				next();
				return *this;
			}

			ZorderIterator operator++ (int)
			{
				ZorderIterator tmp(*this);
				next();
				return tmp;
			}

			template<class OtherType>
			bool operator == (const ZorderIterator<OtherType>& rhs) const
			{
				return idx == rhs->idx;
			}

			template<class OtherType>
			bool operator != (const ZorderIterator<OtherType>& rhs) const
			{
				return idx != rhs->idx;
			}

			typename std::shared_ptr<Type> operator* () const
			{
				return current;
			}

			typename std::shared_ptr<Type> operator-> () const
			{
				return current;
			}

			bool atEnd() const
			{
				return current == nullptr;
			}

			operator ZorderIterator<const Type>() const
			{
				return ZorderIterator<const Type>(root);
			}
		};

		friend class ZorderIterator < T >;
		friend class ZorderIterator < const T >;

		typedef ZorderIterator<T> iterator;
		typedef ZorderIterator<const T> const_iterator;

		iterator begin()
		{
			return iterator(this);
		}

		iterator end()
		{
			iterator iter(this);
			iter.moveToEnd();
			return iter;
		}

		const_iterator cbegin()
		{
			return const_iterator(this);
		}

		const_iterator cend()
		{
			const_iterator iter(this);
			iter.moveToEnd();
			return iter;
		}
	};

	class CUQuadTree : public QuadTree < CU, CUQuadTree >
	{
	protected:

		virtual std::unique_ptr<CU> createLeaf() override
		{
			return std::make_unique<CU>(pos.x, pos.y, size());
		}
		virtual std::unique_ptr<CUQuadTree> createSubtree(size_t x, size_t y) override
		{
			return std::make_unique<CUQuadTree>(x, y, size() / 2);
		}

	public:

		CUQuadTree(int x, int y, size_t size);
		virtual ~CUQuadTree() = default;

		std::shared_ptr<CU> getCU() { return leaf; }

		virtual void print(LogId logId, bool recursive = true) override;
	};

	class TUQuadTree : public QuadTree < TU, TUQuadTree >
	{
	protected:

		virtual std::unique_ptr<TU> createLeaf() override
		{
			return std::make_unique<TU>(pos.x, pos.y, size());
		}
		virtual std::unique_ptr<TUQuadTree> createSubtree(size_t x, size_t y) override
		{
			return std::make_unique<TUQuadTree>(x, y, size() / 2);
		}
	public:

		TUQuadTree(int x, int y, size_t size);
		virtual ~TUQuadTree() = default;

		std::shared_ptr<TU> getTU() { return leaf; }

		virtual void print(LogId logId, bool recursive = true) override;

		std::shared_ptr<TU> getTuContainingPosition(int x, int y);
	};
}