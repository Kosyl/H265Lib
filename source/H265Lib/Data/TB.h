#pragma once

#include <Common/Bitset.h>
#include <Data/BlockBase.h>
#include <Data/ReferenceSamples.h>
#include <Common/Matrix.h>

namespace HEVC
{
	struct TB : public BlockBase
	{
		ImgComp plane;

		MatrixRef<Sample> input_samples;
		Matrix<Sample> prediction;
		Matrix<Sample> residuals;
		Matrix<Coeff> transform_coefficients;
		MatrixRef<Sample> reconstructed_samples;

		int last_sig_coeff_x, last_sig_coeff_y;
		Bitset coded_sub_block_flags;
		int distortion;
		int num_bins;

		TB(int x, int y, int size, ImgComp comp);
		~TB() = default;

		bool hasAtLeastOneNonZeroCoeff();

		virtual void print(LogId logId, bool recursive = true) override;
	};
}
