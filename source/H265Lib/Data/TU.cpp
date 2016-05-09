#include "TU.h"

namespace HEVC
{
	TU::TU(int x, int y, int size) :
		BlockBase(x, y, size)
	{
		assert(size == 4 || size == 8 || size == 16 || size == 32);
	}

	void TU::createTransformBlocks(size_t min_tu_size, SubsamplingFormat subsampling)
	{
		transform_blocks.push_back(std::make_shared<TB>(pos.x, pos.y, m_size, Luma));

		if (subsampling == SubsamplingFormat::Mode_444_SeparatelyCoded)
			return;

		bool minimal_transform_unit = m_size == min_tu_size;
		bool first_unit_in_parent_tu = pos.x % 8 == 0 && pos.y % 8 == 0;

		if (minimal_transform_unit)
		{
			if (first_unit_in_parent_tu)
			{
				transform_blocks.push_back(std::make_shared<TB>(pos.x / 2, pos.y / 2, m_size, Cb));
				transform_blocks.push_back(std::make_shared<TB>(pos.x / 2, pos.y / 2, m_size, Cr));
			}
		}
		else
		{
			transform_blocks.push_back(std::make_shared<TB>(pos.x / 2, pos.y / 2, m_size / 2, Cb));
			transform_blocks.push_back(std::make_shared<TB>(pos.x / 2, pos.y / 2, m_size / 2, Cr));
		}
	}

	void TU::print(LogId logId, bool recursive)
	{
	}
}
