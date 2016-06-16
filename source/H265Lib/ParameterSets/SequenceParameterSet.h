#pragma once
#include <Common/TypeDef.h>
#include <Common/Enums.h>
#include <Common/Matrix.h>
#include <ParameterSets/ParameterBank.h>
#include "ParameterSetBase.h"
#include "ProfileTierLevel.h"
#include "SubLayerOrderingInfo.h"
#include "ScalingListData.h"
#include "ShortTermReferencePictureSet.h"
#include "VUIParameters.h"
#include "ParameterSets/VideoParameterSet.h"

namespace HEVC
{
	class SequenceParameterSet: public ParameterSetBase
	{
	public:

		std::shared_ptr<VideoParameterSet> vps;

		int max_sub_layers;
		bool temporal_id_nesting_flag;
		ProfileTierLevel profile_tier_level;

		SubsamplingFormat chroma_format_idc;
		bool separate_colour_plane_flag;

		int pic_width_in_luma_samples, pic_height_in_luma_samples;

		bool conformance_window_flag;
		int conf_win_left_offset, conf_win_right_offset, conf_win_top_offset, conf_win_bottom_offset;

		int bit_depth_luma, bit_depth_chroma;

		int max_pic_order_cnt, log2_max_pic_order_cnt;

		bool sub_layer_ordering_info_present_flag;
		std::vector<SubLayerOrderingInfo> sub_layer_ordering_infos;

		int log2_ctu_size, log2_max_coding_block_size, log2_min_coding_block_size;
		int ctu_size;
		size_t max_luma_coding_block_size, min_luma_coding_block_size; //def. 64 i 8
		int max_luma_transform_block_size, min_luma_transform_block_size; // def. 32 i 4
		int pic_width_in_ctus, pic_height_in_ctus;

		int log2_max_transform_block_size, log2_min_transform_block_size;
		int max_transform_hierarchy_depth_inter, max_transform_hierarchy_depth_intra;

		bool scaling_list_enabled_flag;
		bool scaling_list_data_present_flag;
		ScalingListData scaling_list_data;

		bool amp_enabled_flag;
		bool sample_adaptive_offset_enabled_flag;

		bool pcm_enabled_flag;
		int pcm_sample_bit_depth_luma, pcm_sample_bit_depth_chroma;
		int log2_min_pcm_luma_coding_block_size, log2_max_pcm_luma_coding_block_size;
		int min_cu_size_pcm, max_cu_size_pcm;
		bool pcm_loop_filter_disabled_flag;

		int num_short_term_ref_pic_sets;
		std::vector<ShortTermReferencePictureSet> short_term_ref_pic_set;

		bool long_term_ref_pics_present_flag;
		int num_long_term_ref_pics;
		std::vector<int> lt_ref_pic_poc_lsb_sps;
		std::vector<bool> used_by_curr_pic_lt_sps_flag;

		bool temporal_mvp_enabled_flag;
		bool strong_intra_smoothing_enabled_flag;

		bool vui_parameters_present_flag;
		VUIParameters vui_parameters;

		bool sps_extension_flag;
		bool sps_extension_data_flag;

		void refresh( );

		SequenceParameterSet( ) = delete;
		SequenceParameterSet( int idx );

		virtual ~SequenceParameterSet( ) override;

		void initWithDefaults( ) override;

		size_t getPicWidth( ImgComp plane = ImgComp::Luma ) const;
		size_t getPicHeight( ImgComp plane = ImgComp::Luma ) const;
		void setPicSize( size_t width, size_t height );
		void refreshPicSizeInCTUs( );

		void setConformanceWindow( int top, int bottom, int left, int right );

		int getBitDepth( ImgComp comp ) const;
		Sample clip( ImgComp comp, Sample value );
		Sample getDefaultSampleValue( ImgComp comp );
		void configure( EncoderParameters configuration );
		int chromaScaleFactorX( ) const;

		int chromaScaleFactorY( ) const;
	};

	class SequenceParameterSetBank: public ParameterBank < SequenceParameterSet, SequenceParameterSetBank >
	{
	public:

		std::shared_ptr<SequenceParameterSet> createNext( )
		{
			int idx = getNextIdx( );
			auto set = std::make_shared<SequenceParameterSet>( idx );
			parameter_sets[ idx ] = set;
			return set;
		}
	};
}
