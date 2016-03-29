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

		void setSize(int width, int height, int chromaWidth, int chromaHeight);
		void initCTUs();

	public:
		Matrix<Sample> input_samples[3];
		Matrix<Sample> reconstructed_samples[3];

		Matrix<int> tiles_map{ 0,0 };
		Matrix<int> slices_map{ 0,0 };

		int width_luma{ 0 }, width_chroma{ 0 }, height_luma{ 0 }, height_chroma{ 0 };
		int width_in_ctus{ 0 }, height_in_ctus{ 0 };
		int ctu_size{ 0 };
		bool tiles_enabled{ false };

		Picture();

		void initFromParameters(std::shared_ptr<SequenceParameterSet> parameters);
		void loadFrameFromYuv(std::ifstream& yuvFile);

		std::shared_ptr<CTU> getCTU(int ctuX, int ctuY);
		std::shared_ptr<CTU> getCTUBySamplePosition(int sampleX, int sampleY);
		std::shared_ptr<CU> getCuContainingPosition(int sampleX, int sampleY);

		int getWidth(const ImgComp comp) const;
		int getHeight(const ImgComp comp) const;

		void printDescription(LogId logId, bool recursive = true, bool printSamples = false);
	};
}