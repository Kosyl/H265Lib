
#include "QuadTree.h"
#include "CU.h"
#include "CTU.h"
#include "TU.h"

namespace H265Lib
{
	CUQuadTree::CUQuadTree(UInt x, UInt y, UInt size, ParametersBundle parameters) :
		QuadTree(x, y, size, parameters)
	{
		assert(size == 8 || size == 16 || size == 32 || size == 64);
	}

	std::shared_ptr<CU> CUQuadTree::getCU()
	{
		return _leaf;
	}

	Void CUQuadTree::printDescription(Logs logId, Bool recursive)
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

	std::shared_ptr<TU> TUQuadTree::getTU()
	{
		return _leaf;
	}

	Void TUQuadTree::printDescription(Logs logId, Bool recursive)
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
			left = Position.X + step > x ? 0 : 1;
			up = Position.Y + step > x ? 0 : 1;
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