#include "EncoderParameters.h"

namespace HEVC
{
	void EncoderParameters::initFromConfigurationFile(std::string configFilePath)
	{
		//HARDCODED
		num_frames_to_encode = 2;
		gop_size = 1;
		input_file_path = "D:/HEVC/akiyo_qcif.yuv";
		output_file_path = "D:/result.bin";
		intra_period = 1;
		max_cu_size = 64;
		qp = 30;

		pic_width = 176;
		pic_height = 144;
		subsampling = SubsamplingFormat::Mode_420;
	}
}