#pragma once

#include <Common/Bitset.h>
#include <Data/BlockBase.h>
#include <Data/ReferenceSamples.h>

namespace HEVC
{
	class TB : public BlockBase
	{
	private:

	public:
		ImgComp plane;

		ReferenceSamples reference_samples;

		Matrix<Sample> input_samples;
		Matrix<Sample> prediction;
		Matrix<Sample> residuals;
		Matrix<Coeff> transform_coefficients;
		Matrix<Sample> reconstructed_samples;

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