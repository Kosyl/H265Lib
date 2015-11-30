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

		UShort _widthLuma{ 0 }, _widthChroma{ 0 }, _heightLuma{ 0 }, _heightChroma{ 0 };
		UShort _widthInCTUs{ 0 }, _heightInCTUs{ 0 };
		UShort _ctuSize{ 0 };
		Bool _tilesEnabled{ false };

		std::shared_ptr<Matrix<Sample>> _inputSamplesY{ nullptr };
		std::shared_ptr<Matrix<Sample>> _inputSamplesCb{ nullptr };
		std::shared_ptr<Matrix<Sample>> _inputSamplesCr{ nullptr };

		std::shared_ptr<Matrix<Sample>> _reconstructedSamplesY{ nullptr };
		std::shared_ptr<Matrix<Sample>> _reconstructedSamplesCb{ nullptr };
		std::shared_ptr<Matrix<Sample>> _reconstructedSamplesCr{ nullptr };

		Matrix<UShort> _tilesMap{ 0,0 };
		Matrix<UShort> _slicesMap{ 0,0 };

		std::vector<std::shared_ptr<Slice>> _slices{ 0 };
		Matrix<std::shared_ptr<CTU>> _CTUs{ 0,0 };

		Void setSize(UShort width, UShort height, UShort chromaWidth, UShort chromaHeight);
		Void initCTUs();

	public:

		Picture();
		Picture(ParametersBundle parameters);

		Void initFromParameters(ParametersBundle parameters);
		Void loadFrameFromYuv(std::ifstream& yuvFile);
		Void resetSampleBuffers();

		std::shared_ptr<Matrix<Sample>> getInputSamples(const ImgComp comp);
		std::shared_ptr<Matrix<Sample>> getReconstructionMatrix(const ImgComp comp);

		std::shared_ptr<CTU> getCTU(UShort ctuX, UShort ctuY);
		std::shared_ptr<CTU> getCTUBySamplePosition(UShort sampleX, UShort sampleY);
		std::shared_ptr<CU> getCuContainingPosition(UShort sampleX, UShort sampleY);

		UShort getWidth(const ImgComp comp) const;
		UShort getHeight(const ImgComp comp) const;

		Void printDescription(LogId logId, Bool recursive = true, Bool printSamples = false);
	};
}