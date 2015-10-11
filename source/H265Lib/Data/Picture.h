#pragma once

#include <vector>
#include <Common/TypeDef.h>
#include <Common/Matrix.h>
#include <ParameterSets/PictureParameterSet.h>
#include <ParameterSets/ParametersBundle.h>

namespace H265Lib
{
	class CTU;
	class CU;
	class Slice;

	class Picture
	{
	protected:

		std::shared_ptr<Matrix<Sample>> _inputSamplesY;
		std::shared_ptr<Matrix<Sample>> _inputSamplesCb;
		std::shared_ptr<Matrix<Sample>> _inputSamplesCr;

		std::shared_ptr<Matrix<Sample>> _reconstructedSamplesY;
		std::shared_ptr<Matrix<Sample>> _reconstructedSamplesCb;
		std::shared_ptr<Matrix<Sample>> _reconstructedSamplesCr;
		
		Matrix<UShort> _tilesMap;
		Matrix<UShort> _slicesMap;

		std::vector<std::shared_ptr<Slice>> _slices;
		Matrix<std::shared_ptr<CTU>> _CTUs;

		void setSize(unsigned width, unsigned height, unsigned chromaWidth, unsigned chromaHeight);

		Void initCTUs();

	public:

		ParametersBundle Parameters;

		Picture();
		Picture(ParametersBundle parameters);

		Void initFromParameters(ParametersBundle parameters);
		Void loadFrameFromYuv(std::ifstream& yuvStream);

		std::shared_ptr<Matrix<Sample>> getInputSamples(const ImgComp comp);
		std::shared_ptr<Matrix<Sample>> getReconstructionMatrix(const ImgComp comp);
		Void resetSampleBuffers();

		std::shared_ptr<CTU> getCTU(UInt ctuX, UInt ctuY);
		std::shared_ptr<CTU> getCTUBySamplePosition(UInt sampleX, UInt sampleY);
		std::shared_ptr<CU> getCuContainingPosition(UInt sampleX, UInt sampleY);

		Void printDescription();
	};
}