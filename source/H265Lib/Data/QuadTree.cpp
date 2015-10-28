
#include "QuadTree.h"
#include "CU.h"
#include "CTU.h"
#include "TU.h"

namespace H265Lib
{
	template <class T, class self>
	Void QuadTree<T, self>::createSubTrees()
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

	template<class T, class S>
	QuadTree<T, S>::QuadTree(UInt x, UInt y, UInt size, ParametersBundle parameters) :
		BlockBase(x, y, size, parameters), _subTrees(2,2)
	{
		clear();
	}

	template <class T, class self>
	Void QuadTree<T, self>::clear()
	{
		for (auto& subtree:_subTrees)
		{
			subtree = nullptr;
		}
		setLeaf(nullptr);
		_mode = QTMode::Unset;
	}

	template <class T, class self>
	QTMode QuadTree<T, self>::getQTMode() const
	{
		return _mode;
	}

	template <class T, class self>
	void QuadTree<T, self>::rebuild(QTMode val)
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

	template <class T, class self>
	Bool QuadTree<T, self>::isLeaf() const
	{
		return _mode == QTMode::Leaf;
	}

	template <class T, class self>
	Bool QuadTree<T, self>::isSplit() const
	{
		return _mode == QTMode::Split;
	}

	template <class T, class self>
	void QuadTree<T, self>::refreshPositionInCTU()
	{
		auto log2CTUSize = Parameters.Sps->getLog2CTUSize();
		auto log2MinBlock = Parameters.Sps->getLog2MinTUSize();

		auto x = this->PositionInPicture.X >> log2CTUSize;
		auto y = this->PositionInPicture.Y >> log2CTUSize;
		PositionInCTU.X = x >> log2MinBlock;
		PositionInCTU.Y = y >> log2MinBlock;

		PositionInCTU.resolveIdx(*Parameters.Sps, Indexing::ZScanByBlock);
	}

	////////////////////////////////////////////////

	CUQuadTree::CUQuadTree(UInt x, UInt y, UInt size, ParametersBundle parameters) :
		QuadTree(x, y, size, parameters)
	{
		assert(size == 8 || size == 16 || size == 32 || size == 64);
	}

	CUQuadTree::~CUQuadTree()
	{
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

	TUQuadTree::~TUQuadTree()
	{
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