#pragma once
#include "ParameterSetBase.h"
#include "Common/TypeDef.h"
#include <vector>
#include "ParameterBank.h"
#include "ProfileTierLevel.h"
#include "SubLayerOrderingInfo.h"
#include <Common/Matrix.h>
#include "HRDParameters.h"
#include "EncoderParameters.h"

namespace HEVC
{
	class SubLayerOrderingInfo
	{
	public:
		int max_dec_pic_buffering{ 0 };
		int max_num_reorder_pic{ 0 };
		int max_latency_increase{ 0 };

		SubLayerOrderingInfo() = default;
		SubLayerOrderingInfo(int maxDecPicBuffering, int maxNumReorderPic, int maxLatencyIncrease) :
			max_dec_pic_buffering(maxDecPicBuffering),
			max_num_reorder_pic(maxNumReorderPic),
			max_latency_increase(maxLatencyIncrease)
		{

		}
	};

	class VideoParameterSet : public ParameterSetBase
	{
	public:
		bool base_layer_internal_flag;
		bool base_layer_available_flag;

		uint8_t max_layers;
		uint8_t max_sub_layers;
		bool temporal_id_nesting_flag;
		ProfileTierLevel profile_tier_level;

		bool sub_layer_ordering_info_present_flag;
		std::vector<SubLayerOrderingInfo> sub_layer_ordering_infos;

		int max_layer_id;
		int num_layer_sets;
		std::unique_ptr<Matrix<bool>> layer_id_included_flag;

		bool timing_info_present_flag;
		int num_units_in_tick;
		int time_scale;
		bool poc_proportional_to_timing_flag;
		int num_ticks_poc_diff_one;

		int num_hrd_parameters;
		std::vector<int> hrd_layer_set_idx;
		std::vector<bool> cprms_present_flag;
		std::vector<HRDParameters> hrd_parameters;

		bool extension_flag;

		VideoParameterSet() = delete;
		VideoParameterSet(int idx);

		virtual ~VideoParameterSet() override;
		void configure(EncoderParameters configuration);

		void initWithDefaults() override;
	};

	class VideoParameterSetBank : public ParameterBank<VideoParameterSet, VideoParameterSetBank>
	{
	public:

		std::shared_ptr<VideoParameterSet> createNext( )
		{
			int idx = getNextIdx( );
			auto set = std::make_shared<VideoParameterSet>( idx );
			parameter_sets[ idx ] = set;
			return set;
		}
	};
}
