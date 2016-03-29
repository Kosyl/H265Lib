#pragma once
#include <Common\TypeDef.h>
#include <Common/Enums.h>
#include "ParameterSetBase.h"
#include "ScalingListData.h"
#include "ParameterBank.h"
#include "SequenceParameterSet.h"

namespace HEVC
{
	class PictureParameterSet : public ParameterSetBase
	{
	private:

		void refreshChromaQP();

	public:
		std::shared_ptr<SequenceParameterSet> sps;
		std::shared_ptr<VideoParameterSet> vps;

		bool dependent_slice_segments_enabled_flag;
		bool  output_flag_present_flag;
		int num_extra_slice_header_bits;

		bool sign_data_hiding_enabled_flag;
		bool  cabac_init_present_flag;

		int num_ref_idx_l0_default_active;
		int num_ref_idx_l1_default_active;

		int init_qp;

		bool constrained_intra_pred_flag;
		bool transform_skip_enabled_flag;

		bool cu_qp_delta_enabled_flag;
		int  diff_cu_qp_delta_depth;

		int cb_qp_offset, cr_qp_offset;
		int cb_init_qp, cr_init_qp;
		bool slice_chroma_qp_offsets_present_flag;

		bool weighted_pred_flag;
		bool weighted_bipred_flag;

		bool transquant_bypass_enabled_flag;

		bool tiles_enabled_flag;
		bool entropy_coding_sync_enabled_flag;

		int num_tile_columns;
		int num_tile_rows;
		bool uniform_spacing_flag;
		std::vector<int> column_widths;
		std::vector<int> row_heights;
		bool loop_filter_across_tiles_enabled_flag;

		bool loop_filter_across_slices_enabled_flag;

		bool deblocking_filter_control_present_flag;
		bool deblocking_filter_override_enabled_flag;
		bool deblocking_filter_disabled_flag;
		bool beta_offset;
		bool tc_offset;

		bool scaling_list_data_present_flag;
		ScalingListData scaling_list_data;

		bool lists_modification_present_flag;
		int parallel_merge_level, log2_parallel_merge_level, slice_segment_header_extension_present_flag;

		bool extension_flag;

		bool extension_data_flag;

		void refresh();

		PictureParameterSet() = delete;
		PictureParameterSet(int idx);

		virtual ~PictureParameterSet() override;
		virtual void initWithDefaults() override;

		int getQP(ImgComp comp = ImgComp::Luma) const;
		void configure(EncoderParameters configuration);
	};

	class PictureParameterSetBank : public ParameterBank < PictureParameterSet >
	{

	};
}
