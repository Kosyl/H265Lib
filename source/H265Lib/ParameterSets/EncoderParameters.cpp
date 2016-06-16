#include "EncoderParameters.h"
#include <Common/JSON.h>
#include <fstream>

namespace HEVC
{
	using namespace nlohmann;

	void EncoderParameters::loadPictureEncodingConfiguration(json& config)
	{
		json& j_slice = config["slice"];

		SliceCodingConfiguration slice;
		slice.start_ctu_address = j_slice["start_ctu_addr"].get<unsigned int>();
		slice.end_ctu_address = j_slice["end_ctu_addr"].get<unsigned int>();
		slice.slice_id = j_slice["slice_id"].get<unsigned int>();

		slice.cus_configuration.clear();
		for (auto& j_ctu : j_slice["ctus"])
		{
			for (auto& j_cu : j_ctu["cus"])
			{
				CuCodingConfiguration cu;
				loadCuConfiguration(j_cu, cu);
				slice.cus_configuration.push_back(std::move(cu));
			}
		}

		picture_coding_info.slices_configuration.clear();
		picture_coding_info.slices_configuration.push_back(std::move(slice));
	}

	void EncoderParameters::loadCuConfiguration(json& j_cu, CuCodingConfiguration& dst)
	{
		dst.size = j_cu["size"].get<unsigned int>();
		dst.pos.x = j_cu["x"].get<unsigned int>();
		dst.pos.y = j_cu["y"].get<unsigned int>();
		dst.is_split = j_cu["split_flag"].get<bool>();

		dst.cus_configuration.clear();

		if (dst.is_split)
		{
			for (auto& j_sub_cu : j_cu["cus"])
			{
				CuCodingConfiguration cu;
				loadCuConfiguration(j_sub_cu, cu);
				dst.cus_configuration.push_back(std::move(cu));
			}
		}
		else
		{
			dst.partition_mode = j_cu["part_size"].get<std::string>() == "SIZE_2Nx2N" ? Partition::Mode_2Nx2N : Partition::Mode_NxN;

			if(dst.partition_mode == Partition::Mode_2Nx2N)
			{
				dst.intra_modes_luma[0] = j_cu["intra_modes_luma"][0].get<unsigned int>();
				dst.intra_modes_luma[1] = -1;
				dst.intra_modes_luma[2] = -1;
				dst.intra_modes_luma[3] = -1;
			}
			else // NxN
			{
				dst.intra_modes_luma[0] = j_cu["intra_modes_luma"][0].get<unsigned int>(); 
				dst.intra_modes_luma[1] = j_cu["intra_modes_luma"][1].get<unsigned int>(); 
				dst.intra_modes_luma[2] = j_cu["intra_modes_luma"][2].get<unsigned int>(); 
				dst.intra_modes_luma[3] = j_cu["intra_modes_luma"][3].get<unsigned int>();
			}

			dst.intra_mode_chroma = j_cu["pred_mode_chroma"].get<unsigned int>();

      TuCodingConfiguration tu;
      loadTuConfiguration(j_cu["transforms"][0], tu);
      dst.tus_configuration = tu;
		}
	}

	void EncoderParameters::loadTuConfiguration(json& j_tu, TuCodingConfiguration& dst)
	{
		dst.is_split = j_tu["subdiv"].get<bool>();

		dst.tus_configuration.clear();

		if (dst.is_split)
		{
			for (auto& j_sub_tu : j_tu["transforms"])
			{
				TuCodingConfiguration tu;
				loadTuConfiguration(j_sub_tu, tu);
				dst.tus_configuration.push_back(std::move(tu));
			}
		}
	}

	void EncoderParameters::initFromConfigurationFile(std::string configFilePath)
	{
		//HARDCODED
		std::ifstream file("D:/HEVC/decoder.txt");

		complete_configuration = nlohmann::json::parse(file);

		num_frames_to_encode = 1;
		gop_size = 1;
		input_file_path = "D:/HEVC/akiyo_qcif.yuv";
		output_file_path = "D:/result.bin";
		intra_period = 1;

		json &sps = complete_configuration["sps"];
		loadPictureEncodingConfiguration(complete_configuration);

		pic_width = sps["pic_width_in_luma_samples"].get<unsigned int>();
		pic_height = sps["pic_height_in_luma_samples"].get<unsigned int>();
		subsampling = SubsamplingFormat(sps["chroma_format_idc"].get<unsigned int>());
	}

  short CuCodingConfiguration::getChromaDerivationType( short luma_mode) const
  {
    switch (intra_mode_chroma)
    {
    case 36:
      return 4;
    case 1:
      return 3;
    case 10:
      return 2;
    case 26:
      return 1;
    case 0:
      return 0;
    case 34:
      switch (luma_mode)
      {
      case 1:
        return 3;
      case 10:
        return 2;
      case 26:
        return 1;
      case 0:
        return 0;
      default:
        assert(false);
        return -1;
      }
    default:
      assert(false);
      return -1;
    }
  }
}
