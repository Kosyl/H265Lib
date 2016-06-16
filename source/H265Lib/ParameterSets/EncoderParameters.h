#pragma once
#include <string>
#include <Common/Enums.h>
#include <Common/JSON.h>
#include <Data/BlockBase.h>

namespace HEVC
{
	struct TuCodingConfiguration
	{
		Position pos;
		size_t size{ 0 };
		bool is_split{ false };

		std::vector<TuCodingConfiguration> tus_configuration;
	};

	struct CuCodingConfiguration
	{
		Position pos;
		PredictionType prediction_type{ PredictionType::Intra };
		Partition partition_mode{ Partition::Mode_2Nx2N };
		size_t size{ 0 };
		bool is_split{ false };
		int intra_modes_luma[4]{ -1,-1,-1,-1 };
		int intra_mode_chroma{ -1 };

		std::vector<CuCodingConfiguration> cus_configuration;
		TuCodingConfiguration tus_configuration;
	  short getChromaDerivationType(short luma_mode) const;
	};

	struct SliceCodingConfiguration
	{
		int slice_id{ 0 };
		int start_ctu_address{ 0 };
		int end_ctu_address{ 0 };

		std::vector<CuCodingConfiguration> cus_configuration;
	};

	struct PictureCodingConfiguration
	{
		std::vector<SliceCodingConfiguration> slices_configuration;
	};

	class EncoderParameters
	{
	private:
		void loadPictureEncodingConfiguration(nlohmann::json& config);
		void loadCuConfiguration(nlohmann::json& j_cu, CuCodingConfiguration& dst);
		void loadTuConfiguration(nlohmann::json& j_tu, TuCodingConfiguration& dst);

	public:
		int num_frames_to_encode;

		std::string input_file_path;
		std::string output_file_path;

		int pic_width;
		int pic_height;
		SubsamplingFormat subsampling;

		int intra_period;
		int gop_size;

		nlohmann::json complete_configuration;
		PictureCodingConfiguration picture_coding_info;

		EncoderParameters() = default;

		void initFromConfigurationFile(std::string configFilePath);
	};
}
