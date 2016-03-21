#include "VideoParameterSet.h"

namespace HEVC
{
	void VideoParameterSet::initWithDefaults()
	{
		max_layers = 1;
		max_sub_layers = 0;
		temporal_id_nesting_flag = false;
		sub_layer_ordering_info_present_flag = false;
		max_layer_id = 0;
		num_layer_sets = 0;
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
}
