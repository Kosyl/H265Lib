#pragma once

#include <ParameterSets/EncoderParameters.h>
#include <ParameterSets/ParametersBundle.h>
#include <Data/Picture.h>
#include "PictureEncoder.h"

namespace HEVC
{
	class SequenceEncoder
	{
	private:
		EncoderParameters config;
		ParametersBundle encoding_parameters;

		std::vector<std::unique_ptr<Picture>> input_frames;
		std::unique_ptr<IPictureEncoder> pic_encoder;

		void loadFrames(std::string filePath, int numPics, std::shared_ptr<SequenceParameterSet> sps);
		void cleanup();

	public:
		SequenceEncoder();
		~SequenceEncoder();

		void encodeSequence(EncoderParameters configuration);
		void writeBitstream();
	};
}
