#include "SequenceParameterSet.h"
#include <Common/Matrix.h>
#include <Common/Calc.h>

namespace HEVC
{
	SequenceParameterSet::SequenceParameterSet( int idx ):
		ParameterSetBase( idx )
	{
		initWithDefaults( );
	}

	SequenceParameterSet::~SequenceParameterSet( )
	{
		initWithDefaults( );
	}

	void SequenceParameterSet::configure( EncoderParameters configuration )
	{
		nlohmann::json& sps = configuration.complete_configuration[ "sps" ];

		max_sub_layers = sps["sps_max_sub_layers_minus1"].get<unsigned int>() + 1;
		temporal_id_nesting_flag = sps[ "sps_temporal_id_nesting_flag" ].get<bool>( );

		profile_tier_level.initFromJson( sps[ "profileTierLevel" ] );

		chroma_format_idc = SubsamplingFormat( sps[ "chroma_format_idc" ].get<unsigned int>( ) );
		if( chroma_format_idc == SubsamplingFormat::Mode_444 )
		{
			separate_colour_plane_flag = sps[ "sps_max_sub_layers_minus1" ].get<bool>( );
		}

		setPicSize( sps[ "pic_width_in_luma_samples" ].get<size_t>( ),
								sps[ "pic_height_in_luma_samples" ].get<size_t>( ) );

		conformance_window_flag = sps[ "conformance_window_flag" ].get<bool>( );
		if( conformance_window_flag )
		{
			setConformanceWindow( sps[ "conf_win_top_offset" ].get<unsigned int>( ),
														sps[ "conf_win_bottom_offset" ].get<unsigned int>( ),
														sps[ "conf_win_left_offset" ].get<unsigned int>( ),
														sps[ "conf_win_right_offset" ].get<unsigned int>( ) );
		}

		bit_depth_luma = sps[ "bit_depth_luma_minus8" ].get<unsigned int>( ) + 8;
		bit_depth_chroma = sps[ "bit_depth_chroma_minus8" ].get<unsigned int>( ) + 8;

		max_pic_order_cnt = 1 << ( sps[ "log2_max_pic_order_cnt_lsb_minus4" ].get<unsigned int>( ) + 4 );

		sub_layer_ordering_info_present_flag = sps[ "sps_sub_layer_ordering_info_present_flag" ].get<bool>( );
		if( sub_layer_ordering_info_present_flag )
		{
			for( auto& singleInfo : sps[ "sub_layer_ordering_info" ] )
			{
				sub_layer_ordering_infos.push_back( SubLayerOrderingInfo( singleInfo ) );
			}
		}

		unsigned int log2_min_cu = sps[ "log2_min_luma_coding_block_size_minus3" ].get<unsigned int>( ) + 3;
		unsigned int log2_cu_diff = sps[ "log2_diff_max_min_luma_coding_block_size" ].get<unsigned int>( );
		unsigned int log2_min_tu = sps[ "log2_min_luma_transform_block_size_minus2" ].get<unsigned int>( ) + 2;
		unsigned int log2_tu_diff = sps[ "log2_diff_max_min_luma_transform_block_size" ].get<unsigned int>( );

		min_luma_coding_block_size = 1 << log2_min_cu;
		max_luma_coding_block_size = min_luma_coding_block_size << log2_cu_diff;
		min_luma_transform_block_size = 1 << log2_min_tu;
		max_luma_transform_block_size = min_luma_transform_block_size << log2_tu_diff;
		refresh( );

		max_transform_hierarchy_depth_intra = sps[ "max_transform_hierarchy_depth_inter" ].get<unsigned int>( );
		max_transform_hierarchy_depth_inter = sps[ "max_transform_hierarchy_depth_intra" ].get<unsigned int>( );

		scaling_list_enabled_flag = sps[ "scaling_list_enabled_flag" ].get<bool>( );
		if( scaling_list_enabled_flag )
		{
			scaling_list_data_present_flag = sps[ "scaling_list_data_present_flag" ].get<bool>( );
		}

		amp_enabled_flag = sps[ "amp_enabled_flag" ].get<bool>( );

		sample_adaptive_offset_enabled_flag = sps[ "sample_adaptive_offset_enabled_flag" ].get<bool>();

		pcm_enabled_flag = sps[ "pcm_enabled_flag" ].get<bool>( );
		num_short_term_ref_pic_sets = sps[ "num_short_term_ref_pic_sets" ].get<unsigned int>( );
		long_term_ref_pics_present_flag = sps[ "long_term_ref_pics_present_flag" ].get<bool>( );

		temporal_mvp_enabled_flag = sps[ "sps_temporal_mvp_enable_flag" ].get<bool>( );

		strong_intra_smoothing_enabled_flag = sps[ "sps_strong_intra_smoothing_enable_flag" ].get<bool>( );

		vui_parameters_present_flag = sps[ "vui_parameters_present_flag" ].get<bool>( );

		sps_extension_flag = sps[ "sps_extension_present_flag" ].get<bool>( );
	}

	int SequenceParameterSet::chromaScaleFactorX( ) const
	{
		return ( chroma_format_idc == SubsamplingFormat::Mode_420 || chroma_format_idc == SubsamplingFormat::Mode_422 ) ? 2 : 1;
	}

	int SequenceParameterSet::chromaScaleFactorY( ) const
	{
		return ( chroma_format_idc == SubsamplingFormat::Mode_420 ) ? 2 : 1;
	}

	void SequenceParameterSet::initWithDefaults( )
	{
		max_sub_layers = 1;

		chroma_format_idc = SubsamplingFormat::Mode_420;
		separate_colour_plane_flag = false;

		setConformanceWindow( 0, 0, 0, 0 );
		conformance_window_flag = false;

		bit_depth_luma = 8;
		bit_depth_chroma = 8;

		max_pic_order_cnt = 1;

		sub_layer_ordering_info_present_flag = false;

		max_luma_coding_block_size = 64;
		min_luma_coding_block_size = 8;
		max_luma_transform_block_size = 32;
		min_luma_transform_block_size = 4;
		setPicSize( 0, 0 );

		max_transform_hierarchy_depth_intra = 3;
		max_transform_hierarchy_depth_inter = 3;

		scaling_list_enabled_flag = false;
		scaling_list_data_present_flag = false;

		amp_enabled_flag = true;

		sample_adaptive_offset_enabled_flag = false;

		pcm_enabled_flag = false;

		temporal_mvp_enabled_flag = false;

		strong_intra_smoothing_enabled_flag = true;

		vui_parameters_present_flag = false;

		sps_extension_flag = false;

		refresh( );
	}

	void SequenceParameterSet::refresh( )
	{
		log2_max_transform_block_size = Calc::log2( max_luma_transform_block_size );
		log2_min_transform_block_size = Calc::log2( min_luma_transform_block_size );
		log2_max_coding_block_size = Calc::log2( max_luma_coding_block_size );
		log2_min_coding_block_size = Calc::log2( min_luma_coding_block_size );
		log2_ctu_size = Calc::log2( max_luma_coding_block_size );
		ctu_size = max_luma_coding_block_size;

		log2_max_pic_order_cnt = Calc::log2( max_pic_order_cnt );

		refreshPicSizeInCTUs( );
	}

#pragma region PicSize

	size_t SequenceParameterSet::getPicWidth( ImgComp plane ) const
	{
		if( chroma_format_idc == SubsamplingFormat::Mode_444 )
		{
			return pic_width_in_luma_samples;
		}
		else //422 albo 420
		{
			if( plane == ImgComp::Luma )
				return pic_width_in_luma_samples;
			else //chroma
				return pic_width_in_luma_samples / 2;
		}
	}

	size_t SequenceParameterSet::getPicHeight( ImgComp plane ) const
	{
		if( chroma_format_idc == SubsamplingFormat::Mode_444 || chroma_format_idc == SubsamplingFormat::Mode_422 )
		{
			return pic_height_in_luma_samples;
		}
		else //420
		{
			if( plane == ImgComp::Luma )
				return pic_height_in_luma_samples;
			else //chroma
				return pic_height_in_luma_samples / 2;
		}
	}

	void SequenceParameterSet::setPicSize( size_t width, size_t height )
	{
		assert( (width == 0 || width%min_luma_coding_block_size == 0) && 
						(height == 0 || height%min_luma_coding_block_size == 0) );

		pic_width_in_luma_samples = width;
		pic_height_in_luma_samples = height;

		refreshPicSizeInCTUs( );
	}

	void SequenceParameterSet::refreshPicSizeInCTUs( )
	{
		pic_width_in_ctus = ( pic_width_in_luma_samples - 1 ) / ctu_size + 1;
		pic_height_in_ctus = ( pic_height_in_luma_samples - 1 ) / ctu_size + 1;
	}

#pragma endregion

	void SequenceParameterSet::setConformanceWindow( int top, int bottom, int left, int right )
	{
		conf_win_bottom_offset = bottom;
		conf_win_left_offset = left;
		conf_win_right_offset = right;
		conf_win_top_offset = top;
	}

	int SequenceParameterSet::getBitDepth( ImgComp comp ) const
	{
		return comp == ImgComp::Luma ? bit_depth_luma : bit_depth_chroma;
	}

	Sample SequenceParameterSet::getDefaultSampleValue( ImgComp comp )
	{
		if( comp == ImgComp::Luma )
			return 1 << ( bit_depth_luma - 1 );
		else
			return 1 << ( bit_depth_chroma - 1 );
	}

	Sample SequenceParameterSet::clip( ImgComp comp, Sample value )
	{
		int maxVal = comp == ImgComp::Luma ? 1 << bit_depth_luma : 1 << bit_depth_chroma;
		return Calc::clipToRange<Sample>( 0, maxVal, value );
	}
}