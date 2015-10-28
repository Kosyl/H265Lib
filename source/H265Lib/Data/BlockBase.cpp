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
		BlockBase(0, 0, 0, ParametersBundle())
	{

	}

	BlockBase::BlockBase(UShort x, UShort y, UShort size, ParametersBundle parameters):
		PositionInPicture(x, y),
		_size(size),
		Parameters(parameters)
	{
		assert(size > 0 && size % 4 == 0);
	}

	Void BlockBase::printDescription(Logs logId, Bool recursive)
	{
		LOGLN(logId, "(", this->PositionInPicture.X, ",", this->PositionInPicture.Y, "), size=", this->_size);
	}

	UShort BlockBase::getSize()
	{
		return _size;
	}
}