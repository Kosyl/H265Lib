#pragma once

#include <ParameterSets/EncoderParameters.h>
#include <ParameterSets/ParametersBundle.h>
#include <Data/Picture.h>

namespace HEVC
{
	class SequenceEncoder
	{
	private:
		EncoderParameters config;
		ParametersBundle encoding_parameters;

		std::vector<std::shared_ptr<Picture>> input_frames;

		void loadFrames(std::string filePath, int numPics, std::shared_ptr<SequenceParameterSet> sps);

	public:
		SequenceEncoder() = default;

		void encodeSequence(EncoderParameters configuration);

		void writeBitstream();
	};
}
