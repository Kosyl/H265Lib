
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
	BlockBase(x, y, size, parameters), _subTrees(2, 2)
{
	clear();
}

template <class T, class self>
Void QuadTree<T, self>::clear()
{
	for (auto& subtree : _subTrees)
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