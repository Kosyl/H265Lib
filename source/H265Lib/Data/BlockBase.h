#pragma once

#include <Common/TypeDef.h>
#include <ParameterSets/SequenceParameterSet.h>
#include <ParameterSets/ParametersBundle.h>

namespace HEVC
{
	struct Position
	{
		int x, y;
		int idx;

		Position();
		Position(int new_x, int new_y);
		Position move(int dx, int dy) { return Position(x + dx, y + dy); };
	};

	class BlockBase
	{
	protected:
		size_t m_size;

	public:

		BlockBase();
		BlockBase(int x, int y, size_t size);
		virtual ~BlockBase() = default;

		Position pos;
		size_t size() { return m_size; }

		virtual void print(LogId logId, bool recursive);
	};
}