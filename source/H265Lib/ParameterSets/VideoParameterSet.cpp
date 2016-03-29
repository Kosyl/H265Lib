#include "VideoParameterSet.h"

namespace HEVC
{
	void VideoParameterSet::initWithDefaults()
	{
		base_layer_internal_flag = true;
		base_layer_available_flag = true;

		max_layers = 1;
		max_sub_layers = 1;

		temporal_id_nesting_flag = true;

		profile_tier_level.initWithDefaults();

		sub_layer_ordering_info_present_flag = true;
		sub_layer_ordering_infos.push_back(SubLayerOrderingInfo(2, 0, -1));

		max_layer_id = 0;
		num_layer_sets = 1;
		timing_info_present_flag = false;
		num_units_in_tick = 0;
		time_scale = 0;
		poc_proportional_to_timing_flag = false;
		num_ticks_poc_diff_one = 0;
		num_hrd_parameters = 0;
		extension_flag = 0;
	}

	VideoParameterSet::VideoParameterSet(int idx) : ParameterSetBase(idx)
	{
		initWithDefaults();
	}

	VideoParameterSet::~VideoParameterSet()
	{

	}

	void VideoParameterSet::configure(EncoderParameters configuration)
	{
		//defaultowe sa ok
	}
}
