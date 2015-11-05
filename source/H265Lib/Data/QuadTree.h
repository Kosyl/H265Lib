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

		void refreshPositionInCTU();

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

		Void createSubTrees();

	public:

		Position PositionInCTU;

		QuadTree(UInt x, UInt y, UInt size, ParametersBundle parameters);
		virtual ~QuadTree() = default;

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

		Void clear();

		QTMode getQTMode() const;
		void rebuild(QTMode val);
		Bool isLeaf() const;
		Bool isSplit() const;
	};

#include "QuadTreeImpl.cpp"

	class CUQuadTree : public QuadTree < CU, CUQuadTree >
	{
	private:

	public:

		CUQuadTree(UInt x, UInt y, UInt size, ParametersBundle parameters);
		virtual ~CUQuadTree() = default;

		std::shared_ptr<CU> getCU();

		virtual Void printDescription(Logs logId, Bool recursive = true) override;
	};

	class TUQuadTree : public QuadTree < TU, TUQuadTree >
	{
	private:

	public:

		TUQuadTree(UInt x, UInt y, UInt size, ParametersBundle parameters);
		virtual ~TUQuadTree() = default;

		std::shared_ptr<TU> getTU();

		virtual Void printDescription(Logs logId, Bool recursive = true) override;

		std::shared_ptr<TU> getTuContainingPosition(UInt x, UInt y);
	};
}