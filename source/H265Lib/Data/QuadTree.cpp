
#include "QuadTree.h"
#include "CU.h"
#include "CTU.h"
#include "TU.h"

namespace HEVC
{
	CUQuadTree::CUQuadTree(int x, int y, size_t size) :
		QuadTree(x, y, size)
	{
		assert(size == 8 || size == 16 || size == 32 || size == 64);
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

	TUQuadTree::TUQuadTree(int x, int y, size_t size) :
		QuadTree(x, y, size)
	{
		assert(size == 4 || size == 8 || size == 16 || size == 32 || size == 64);
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
		if (isLeaf())
		{
			return leaf;
		}
		else
		{
			int step = size() / 2;
			int left, up;
			left = pos.x + step < x ? 0 : 1;
			up = pos.y + step < x ? 0 : 1;

			return subtrees(left,up)->getTuContainingPosition(x, y);
		}
	}
}