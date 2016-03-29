#pragma once
#include <string>
#include <Common/Enums.h>

namespace HEVC
{
	class EncoderParameters
	{
	public:
		int num_frames_to_encode;

		std::string input_file_path;
		std::string output_file_path;

		int pic_width;
		int pic_height;
		SubsamplingFormat subsampling;

		int max_cu_size;
		int intra_period;
		int gop_size;
		int qp;

		EncoderParameters() = default;

		void initFromConfigurationFile(std::string configFilePath);
	};
}
