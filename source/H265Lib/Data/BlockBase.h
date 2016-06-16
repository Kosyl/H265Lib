#pragma once

#include <Common/TypeDef.h>

namespace HEVC
{
	struct Position
	{
		int x{ 0 }, y{ 0 };
		int idx{ 0 };

		Position();
		Position(int new_x, int new_y);
		Position move(int dx, int dy) const
		{
			return Position(x + dx, y + dy);
		}

		Position scale(int chroma_scale_factor_x, int chroma_scale_factor_y) const
		{
			return Position(x * chroma_scale_factor_x, y * chroma_scale_factor_y);
		}
	};

	class BlockBase
	{
	protected:
		size_t m_size{ 0 };

	public:

		BlockBase();
		BlockBase(int x, int y, size_t size);
		virtual ~BlockBase() = default;

		Position pos;
		size_t size() { return m_size; }

		virtual void print(LogId logId, bool recursive);
	};
}