#include "Data/BlockBase.h"
#include "Common/Logger.h"
#include <Common/Calc.h>
#include <cassert>

namespace HEVC
{
	Position::Position() :
		Position(0, 0)
	{

	}

	Position::Position(int new_x, int new_y):
		x(new_x), y(new_y), idx(Calc::zscan_idx(new_x,new_y))
	{

	}

	BlockBase::BlockBase():
		BlockBase(0, 0, 0)
	{

	}

	BlockBase::BlockBase(int x, int y, size_t size) :
		pos(x, y),
		m_size(size)
	{
		assert(size > 0 && size % 4 == 0);
	}

	void BlockBase::print(LogId logId, bool recursive)
	{
		LOGLN(logId, "(", pos.x, ",", pos.y, "), size=", m_size);
	}
}