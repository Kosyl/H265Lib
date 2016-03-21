#include "PictureParameterSet.h"
#include <Common/Calc.h>

namespace HEVC
{
	PictureParameterSet::PictureParameterSet(int idx) :
		ParameterSetBase(idx)
	{
		initWithDefaults();
	}

	PictureParameterSet::~PictureParameterSet()
	{
		initWithDefaults();
	}

	void PictureParameterSet::initWithDefaults()
	{
		dependent_slice_segments_enabled_flag = true;
		output_flag_present_flag = true;

		sign_data_hiding_enabled_flag = false;

		cabac_init_present_flag = false;

		constrained_intra_pred_flag = false;

		transform_skip_enabled_flag = false;

		init_qp = 30;
		cb_qp_offset = 0;
		cr_qp_offset = 0;
		slice_chroma_qp_offsets_present_flag = false;

		weighted_pred_flag = true;
		weighted_bipred_flag = true;

		transquant_bypass_enabled_flag = false;

		entropy_coding_sync_enabled_flag = false;

		tiles_enabled_flag = false;

		tc_offset = 0;
		beta_offset = 0;
		loop_filter_across_slices_enabled_flag = false;
		loop_filter_across_tiles_enabled_flag = false;
		deblocking_filter_control_present_flag = false;
		deblocking_filter_override_enabled_flag = false;
		deblocking_filter_disabled_flag = false;

		scaling_list_data_present_flag = false;

		parallel_merge_level = 1;

		slice_segment_header_extension_present_flag = false;
		extension_flag = false;
	
		refresh();
	}
	
	void PictureParameterSet::refresh()
	{
		refreshChromaQP();
		log2_parallel_merge_level = Calc::log2(parallel_merge_level);
	}

	int PictureParameterSet::getQP(ImgComp comp /*= ImgComp::Luma*/) const
	{
		if (comp == ImgComp::Luma)
			return init_qp;
		else if (comp == ImgComp::Cb)
			return cb_init_qp;
		else
			return cr_init_qp;
	}

	void PictureParameterSet::refreshChromaQP()
	{
		cb_init_qp = init_qp + cb_qp_offset;
		cr_init_qp = init_qp + cr_qp_offset;
	}
}