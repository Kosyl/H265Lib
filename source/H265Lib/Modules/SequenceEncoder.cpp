#include <Modules/SequenceEncoder.h>
#include <Common/Logger.h>
#include <fstream>
#include "SyntaxWriter.h"

namespace HEVC
{
	SequenceEncoder::SequenceEncoder()
	{

	}

	SequenceEncoder::~SequenceEncoder()
	{
		cleanup();
	}

	void SequenceEncoder::encodeSequence(EncoderParameters configuration)
	{
		LOGLN(Logger::Dump, "Rozpoczecie kodowania");
		LOGLN(Logger::Dump, "plik wejsciowy: ", configuration.input_file_path);
		LOGLN(Logger::Dump, "plik wyjsciowy: ", configuration.output_file_path);
		LOGLN(Logger::Dump, "klatki: ", configuration.num_frames_to_encode, ", QP: ", configuration.qp);
		
		config = configuration;
		encoding_parameters = ParametersBundle::fromConfiguration(configuration);

		loadFrames(configuration.input_file_path, configuration.num_frames_to_encode, encoding_parameters.Sps);

		pic_encoder = std::make_unique<HardcodedPictureEncoder>(encoding_parameters);

		for(auto& frame: input_frames)
		{
			pic_encoder->encodePicture(*frame);
		}

		writeBitstream();
	}

	void SequenceEncoder::loadFrames(std::string filePath, int numPics, std::shared_ptr<SequenceParameterSet> sps)
	{
		input_frames.clear();

		std::ifstream file(filePath, std::ios::binary);
		for (int i = 0; i < numPics; ++i)
		{
			auto pic = std::make_unique<Picture>();
			pic->initFromParameters(*sps);

			pic->loadFrameFromYuv(file);

			input_frames.push_back(std::move(pic));
		}
		file.close();
	}

	void SequenceEncoder::cleanup()
	{
		
	}

	void SequenceEncoder::writeBitstream()
	{
		std::ofstream destination(config.output_file_path, std::ios::binary | std::ios::trunc);
		std::vector<uint8_t> buffer;

		SyntaxWriter writer(buffer);
		writer.writeVpsNalUnit(encoding_parameters.Vps);
		//writer.writeSpsNalUnit(encoding_parameters.Sps);
		//writer.writePpsNalUnit(encoding_parameters.Pps);
		destination.write(reinterpret_cast<const char*>(&(*buffer.begin())), buffer.size());

		//write frames

		destination.flush();
		destination.close();
	}
}
