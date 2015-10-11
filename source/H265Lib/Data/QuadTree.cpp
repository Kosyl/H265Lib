
#include "QuadTree.h"
#include "CU.h"
#include "CTU.h"
#include "TU.h"

namespace H265Lib
{
	template<class T, class S>
	QuadTree<T, S>::QuadTree(UInt x, UInt y, UInt size, ParametersBundle parameters) :
		BlockBase(x, y, size, parameters), _subTrees(2,2)
	{
		clear();
	}

	template<class T, class S>
	QuadTree<T, S>::~QuadTree()
	{
	}

	CUQuadTree::CUQuadTree(UInt x, UInt y, UInt size, ParametersBundle parameters) :
		QuadTree(x, y, size, parameters)
	{
		assert(size == 8 || size == 16 || size == 32 || size == 64);
	}

	CUQuadTree::~CUQuadTree()
	{
	}

	Void CUQuadTree::printDescription(Logs logId)
	{
		//LOG( "PART" ) << "CUQuadTree[" << getX( ) << "][" << getY( ) << "], size: " << getSize( ) << std::endl;
		//LOG_TAB );
		/*if (isLeaf())
		{
		_leaf->printDescription();
		}
		else
		{
		printSubTreesDescription();
		}*/
		//LOG_UNTAB );
	}

	TUQuadTree::TUQuadTree(UInt x, UInt y, UInt size, ParametersBundle parameters) :
		QuadTree(x, y, size, parameters)
	{
		assert(size == 4 || size == 8 || size == 16 || size == 32 || size == 64);
	}

	TUQuadTree::~TUQuadTree()
	{
	}

	Void TUQuadTree::printDescription(Logs logId)
	{
		//LOG( "PART" ) << "TUQuadTree[" << getX( ) << "][" << getY( ) << "], size: " << getSize( ) << std::endl;
		//LOG_TAB );
		/*if (isLeaf())
		{
		_leaf->printDescription();
		}
		else
		{
		printSubTreesDescription();
		}*/
		//LOG_UNTAB );
	}

	std::shared_ptr<TU> TUQuadTree::getTuContainingPosition(UInt x, UInt y)
	{
		if (_mode == QTMode::Leaf)
		{
			return _leaf;
		}
		else
		{
			UInt step = _size / 2;
			Int left, up;
			left = PositionInPicture.X + step > x ? 0 : 1;
			up = PositionInPicture.Y + step > x ? 0 : 1;
			TreePart comp;
			if (left && up)
				comp = TreePart::UpperLeft;
			else if (!left && up)
				comp = TreePart::UpperRight;
			else if (left && !up)
				comp = TreePart::LowerLeft;
			else
				comp = TreePart::LowerRight;

			return getSubTree(comp)->getTuContainingPosition(x, y);
		}
	}
}