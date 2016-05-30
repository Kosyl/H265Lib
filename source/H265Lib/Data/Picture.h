#pragma once

#include <vector>
#include <Common/TypeDef.h>
#include <Common/Matrix.h>
#include <ParameterSets/PictureParameterSet.h>
#include <ParameterSets/ParametersBundle.h>
#include "BlockBase.h"

namespace HEVC
{
	class CTU;
	class CU;
	class Slice;

	class Picture
	{
	protected:

		std::vector<std::shared_ptr<Slice>> slices{ 0 };
		Matrix<std::shared_ptr<CTU>> CTUs{ 0,0 };

		void resetBuffers();
		void initCTUs();
		void setTileIds(SequenceParameterSet &sps);

	public:
		Matrix3D<Sample> input_samples;
		Matrix3D<Sample> reconstructed_samples;

		std::shared_ptr<PictureParameterSet> pps;
		size_t width[NUM_IMG_PLANES], height[NUM_IMG_PLANES];
		size_t width_in_ctus{ 0 }, height_in_ctus{ 0 };
		size_t ctu_size{ 0 };
		size_t log2_min_tb_size{ 2 };
		bool tiles_enabled{ false };

		Picture();
		Picture(ParametersBundle &parameters);
		void initFromParameters(ParametersBundle &parameters);
		void loadFrameFromYuv(std::ifstream &yuvFile);

		std::shared_ptr<Slice> createSlice(size_t beginCtuX, size_t beginCtuY, size_t lengthInCtus);

		std::shared_ptr<CTU> getCTU(size_t ctuX, size_t ctuY);
		std::shared_ptr<CTU> getCTUBySamplePosition(int sampleX, int sampleY);
		std::shared_ptr<CU> getCuContainingPosition(int sampleX, int sampleY);

		size_t getZScanIdx(const size_t x, const size_t y, bool shiftToMinTb = true) const;
		size_t getRasterIdx(const size_t x, const size_t y, bool shiftToMinTb = true) const;

		void printDescription(LogId logId, bool recursive = true, bool printSamples = false);
		Position scalePositionToLuma(const Position& pos, ImgComp plane) const;
	};
}
