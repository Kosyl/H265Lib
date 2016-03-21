#pragma once

#include "Picture.h"
#include "BlockBase.h"
#include "Data/CU.h"
#include "Data/TU.h"

namespace HEVC
{
	class Cintra;
	class Pintra;

	template< class T, class self >
	class QuadTree : public BlockBase
	{
	protected:

		void refreshPositionInCTU(SequenceParameterSet& sps )
		{
			auto log2CTUSize = sps.getLog2CTUSize();
			auto log2MinBlock = sps.getLog2MinTUSize();

			position_in_ctu.x = x >> log2CTUSize >> log2MinBlock;
			position_in_ctu.y = y >> log2CTUSize >> log2MinBlock;

			position_in_ctu.resolveIdx(sps, Indexing::ZScanByBlock);
		}

		virtual std::shared_ptr<self> makeSubTree(int x, int y)
		{
			return std::make_shared<self>(x, y, size / 2);
		}
		virtual void createLeaf() = 0;
		void createSubTrees(int picWidth, int picHeight) 
		{
			subtrees[0][0] = makeSubTree(x, y);

			bool createRight = x + size / 2 < picWidth;
			bool createLower = y + size / 2 < picHeight;

			if (createRight)
				subtrees[1][0] = makeSubTree(x + (size / 2), y);

			if (createLower)
				subtrees[0][1] = makeSubTree(x, y + (size / 2));

			if (createLower && createRight)
				subtrees[1][1] = makeSubTree(x + (size / 2), y + (size / 2));
		}

	public:

		std::shared_ptr<self> subtrees[2][2];
		std::shared_ptr<T> leaf;
		QTMode mode;
		Position position_in_ctu;

		QuadTree(int x, int y, int size) :
			BlockBase(x, y, size)
		{
			clear();
		}
		virtual ~QuadTree() = default;

		void clear()
		{
			for (int i = 0; i < 2; ++i)
			{
				for (int j = 0; j < 2; ++j)
				{
					subtrees[i][j].reset();
				}
			}
			leaf.reset();

			mode = QTMode::Unset;
		}

		void rebuild(QTMode val, int picWidth, int picHeight) 
		{
			clear();
			mode = val;

			switch (mode)
			{
			case QTMode::Split:
				createSubTrees(picWidth, picHeight);
				break;
			case QTMode::Leaf:
			default:
				createLeaf();
				break;
			}
		}

		bool isLeaf() const { return mode == QTMode::Leaf; }
		bool isSplit() const { return mode == QTMode::Split; }
	};

	class CUQuadTree : public QuadTree < CU, CUQuadTree >
	{
	private:

	public:

		CUQuadTree(int x, int y, int size);
		virtual ~CUQuadTree() = default;

		std::shared_ptr<CU> getCU();

		virtual void createLeaf() override
		{
			leaf = std::make_shared<CU>(x, y, size);
		}

		virtual void print(LogId logId, bool recursive = true) override;
	};

	class TUQuadTree : public QuadTree < TU, TUQuadTree >
	{
	private:

	public:

		TUQuadTree(int x, int y, int size);
		virtual ~TUQuadTree() = default;

		virtual void createLeaf() override
		{
			leaf = std::make_shared<TU>(x, y, size);
		}

		std::shared_ptr<TU> getTU();

		virtual void print(LogId logId, bool recursive = true) override;

		std::shared_ptr<TU> getTuContainingPosition(int x, int y);
	};
}