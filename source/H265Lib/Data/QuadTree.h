#pragma once

#include "Picture.h"
#include "BlockBase.h"

namespace H265Lib
{
	class CU;
	class TU;
	class CUIntra;
	class PUIntra;

	template< class T, class self >
	class QuadTree : public BlockBase
	{
	protected:

		Matrix<std::shared_ptr<self>> _subTrees;

		std::shared_ptr<T> _leaf;

		QTMode _mode;

		void refreshPositionInCTU()
		{
			auto log2CTUSize = Parameters.Sps->getLog2CTUSize();
			auto log2MinBlock = Parameters.Sps->getLog2MinTUSize();

			auto x = this->PositionInPicture.X >> log2CTUSize;
			auto y = this->PositionInPicture.Y >> log2CTUSize;
			PositionInCTU.X = x >> log2MinBlock;
			PositionInCTU.Y = y >> log2MinBlock;

			PositionInCTU.resolveIdx(*Parameters.Sps, Indexing::ZScanByBlock);
		}

		Void setLeaf(std::shared_ptr<T> rhs)
		{
			_leaf = rhs;
		}

		virtual std::shared_ptr<self> makeSubTree(UInt x, UInt y)
		{
			return std::make_shared<self>(x, y, _size / 2, Parameters);
		}

		std::shared_ptr<T> makeLeaf()
		{
			return std::make_shared<T>(PositionInPicture.X, PositionInPicture.Y, _size, Parameters);
		}

		Void createSubTrees()
		{
			_subTrees(0, 0) = makeSubTree(PositionInPicture.X, PositionInPicture.Y);

			Bool createRight = PositionInPicture.X + _size / 2 < Parameters.Sps->getPicWidth();
			Bool createLower = PositionInPicture.Y + _size / 2 < Parameters.Sps->getPicHeight();

			if (createRight)
				_subTrees(1, 0) = makeSubTree(PositionInPicture.X + (_size / 2), PositionInPicture.Y);

			if (createLower)
				_subTrees(0, 1) = makeSubTree(PositionInPicture.X, PositionInPicture.Y + (_size / 2));

			if (createLower && createRight)
				_subTrees(1, 1) = makeSubTree(PositionInPicture.X + (_size / 2), PositionInPicture.Y + (_size / 2));
		}

	public:

		Position PositionInCTU;

		QuadTree(UInt x, UInt y, UInt size, ParametersBundle parameters);
		virtual ~QuadTree();

		std::shared_ptr<self> getSubTree(TreePart placement)
		{
			switch (placement)
			{
			case UpperRight:
				return _subTrees(1, 0);
			case LowerLeft:
				return _subTrees(0, 1);
			case LowerRight:
				return _subTrees(1, 1);
			case UpperLeft:
			default:
				return _subTrees(0, 0);
			}
		}

		Matrix<std::shared_ptr<self>>& getSubTrees()
		{
			return _subTrees;
		}

		std::shared_ptr<T> getLeaf()
		{
			return _leaf;
		}

		Void clear()
		{
			for (auto& subtree:_subTrees)
			{
				subtree = nullptr;
			}
			setLeaf(nullptr);
			_mode = QTMode::Unset;
		}

		QTMode getQTMode() const
		{
			return _mode;
		}

		void rebuild(QTMode val)
		{
			assert(val != QTMode::Unset);

			clear();
			_mode = val;

			switch (_mode)
			{
			case QTMode::Split:
				createSubTrees();
				break;
			case QTMode::Leaf:
			default:
				_leaf = makeLeaf();
				break;
			}
		}

		Bool isLeaf()
		{
			return _mode == QTMode::Leaf;
		}

		Bool isSplit()
		{
			return _mode == QTMode::Split;
		}
	};

	class CUQuadTree : public QuadTree < CU, CUQuadTree >
	{
	private:

	public:

		CUQuadTree(UInt x, UInt y, UInt size, ParametersBundle parameters);

		virtual ~CUQuadTree();

		std::shared_ptr<CU> getCU()
		{
			return _leaf;
		}

		/*virtual std::shared_ptr<CUQuadTree> createSubTree(UInt x, UInt y) override
		{
			return std::make_shared<CUQuadTree>(x, y, _size/2);
		}*/

		virtual Void printDescription(Logs logId) override;
	};

	class TUQuadTree : public QuadTree < TU, TUQuadTree >
	{
	private:

	public:

		TUQuadTree(UInt x, UInt y, UInt size, ParametersBundle parameters);

		virtual ~TUQuadTree();

		std::shared_ptr<TU> getTU()
		{
			return _leaf;
		}

		/*virtual std::shared_ptr<TUQuadTree > createSubTree(UInt x, UInt y) override
		{
			return std::make_shared<TUQuadTree>(x, y, _size/2);
		}*/

		virtual Void printDescription(Logs logId) override;

		std::shared_ptr<TU> getTuContainingPosition(UInt x, UInt y);
	};
}