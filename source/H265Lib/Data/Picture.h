#pragma once

#include <vector>
#include <Common/TypeDef.h>
#include <Common/Matrix.h>
#include <ParameterSets/PictureParameterSet.h>
#include <ParameterSets/ParametersBundle.h>

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
		Matrix<Sample> input_samples[3];
		Matrix<Sample> reconstructed_samples[3];

		size_t width[NUM_IMG_PLANES], height[NUM_IMG_PLANES];
		size_t width_in_ctus{ 0 }, height_in_ctus{ 0 };
		size_t ctu_size{ 0 };
		bool tiles_enabled{ false };

		Picture();
		Picture(SequenceParameterSet &parameters);
		void initFromParameters(SequenceParameterSet &parameters);
		void loadFrameFromYuv(std::ifstream &yuvFile);

		std::shared_ptr<Slice> createSlice(size_t beginCtuX, size_t beginCtuY, size_t lengthInCtus);

		std::shared_ptr<CTU> getCTU(size_t ctuX, size_t ctuY);
		std::shared_ptr<CTU> getCTUBySamplePosition(int sampleX, int sampleY);
		std::shared_ptr<CU> getCuContainingPosition(int sampleX, int sampleY);

		void printDescription(LogId logId, bool recursive = true, bool printSamples = false);
	};
}