#pragma once

#include <Common/Bitset.h>
#include <Data/BlockBase.h>
#include <Data/TB.h>

namespace HEVC
{
	struct TU : public BlockBase
	{
		Matrix3DRef<Sample> input_samples;
		Matrix3DRef<Sample> reconstructed_samples;
		std::vector<std::shared_ptr<TB> > transform_blocks;

		short tu_qp_delta;
		short transform_depth;

		bool cbf[3];

		TU(int x, int y, int size);
		virtual ~TU() = default;

		void createTransformBlocks(size_t min_tu_size, SubsamplingFormat subsampling);
		virtual void print(LogId logId, bool recursive = true) override;
	};
}