#include "Data/BlockBase.h"
#include "Common/Logger.h"
#include <cassert>

namespace HEVC
{
	Position::Position() :
		Position(0, 0)
	{

	}

	Position::Position(int x, int y):
		x(x), y(y), idx(0)
	{

	}

	void Position::resolveIdx(const SequenceParameterSet& sps, Indexing idxType)
	{
		this->idx = sps.calcIdx(this->x, this->y, idxType);
	}

	BlockBase::BlockBase():
		BlockBase(0, 0, 0)
	{

	}

	BlockBase::BlockBase(int x, int y, int size) :
		Position(x, y),
		size(size)
	{
		assert(size > 0 && size % 4 == 0);
	}

	void BlockBase::print(LogId logId, bool recursive)
	{
		LOGLN(logId, "(", x, ",", y, "), size=", size);
	}
}