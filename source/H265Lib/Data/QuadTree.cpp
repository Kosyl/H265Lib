
#include "QuadTree.h"
#include "CU.h"
#include "CTU.h"
#include "TU.h"

namespace HEVC
{
	CUQuadTree::CUQuadTree(int x, int y, int size) :
		QuadTree(x, y, size)
	{
		assert(size == 8 || size == 16 || size == 32 || size == 64);
	}

	std::shared_ptr<CU> CUQuadTree::getCU()
	{
		return leaf;
	}

	void CUQuadTree::print(LogId logId, bool recursive)
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

	TUQuadTree::TUQuadTree(int x, int y, int size) :
		QuadTree(x, y, size)
	{
		assert(size == 4 || size == 8 || size == 16 || size == 32 || size == 64);
	}

	std::shared_ptr<TU> TUQuadTree::getTU()
	{
		return leaf;
	}

	void TUQuadTree::print(LogId logId, bool recursive)
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

	std::shared_ptr<TU> TUQuadTree::getTuContainingPosition(int x, int y)
	{
		if (mode == QTMode::Leaf)
		{
			return leaf;
		}
		else
		{
			int step = size / 2;
			int left, up;
			left = x + step > x ? 0 : 1;
			up = y + step > x ? 0 : 1;
			TreePart comp;
			if (left && up)
				comp = TreePart::UpperLeft;
			else if (!left && up)
				comp = TreePart::UpperRight;
			else if (left && !up)
				comp = TreePart::LowerLeft;
			else
				comp = TreePart::LowerRight;

			return subtrees[!!left][!!up]->getTuContainingPosition(x, y);
		}
	}
}