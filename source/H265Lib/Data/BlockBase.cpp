#include "Data/BlockBase.h"
#include <cassert>

namespace H265Lib
{
	Position::Position() :
		Position(0, 0)
	{

	}

	Position::Position(UShort x, UShort y):
		X(x), Y(y), Idx(0)
	{

	}

	Position::Position(UShort x, UShort y, const SequenceParameterSet& sps, Indexing idxType):
		X(x), Y(y), Idx(sps.calcIdx(x, y, idxType))
	{

	}

	Void Position::resolveIdx(const SequenceParameterSet& sps, Indexing idxType)
	{
		this->Idx = sps.calcIdx(this->X, this->Y, idxType);
	}

	BlockBase::BlockBase():
		BlockBase(0, 0, 0)
	{

	}

	BlockBase::BlockBase(UShort x, UShort y, UShort size) :
		Position(x, y),
		_size(size)
	{
		assert(size > 0 && size % 4 == 0);
	}

	Void BlockBase::printDescription(LogId logId, Bool recursive)
	{
		LOGLN(logId, "(", this->Position.X, ",", this->Position.Y, "), size=", this->_size);
	}

	UShort BlockBase::getSize()
	{
		return _size;
	}
}