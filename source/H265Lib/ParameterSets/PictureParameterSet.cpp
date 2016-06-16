#include "PictureParameterSet.h"
#include <Common/Calc.h>

namespace HEVC
{
	PictureParameterSet::PictureParameterSet( int idx, std::shared_ptr<SequenceParameterSet> sps ):
		ParameterSetBase( idx ),
		sps( sps )
	{
		initWithDefaults( );
	}

	PictureParameterSet::~PictureParameterSet( )
	{
		initWithDefaults( );
	}

	void PictureParameterSet::configure( EncoderParameters configuration )
	{
		nlohmann::json& sps = configuration.complete_configuration[ "pps" ];

		dependent_slice_segments_enabled_flag = sps["dependent_slice_segments_enabled_flag"].get<bool>();
		output_flag_present_flag = sps[ "output_flag_present_flag" ].get<bool>( );
		num_extra_slice_header_bits = sps[ "num_extra_slice_header_bits" ].get<bool>( );

		sign_data_hiding_enabled_flag = sps[ "sign_data_hiding_flag" ].get<bool>( );

		cabac_init_present_flag = sps[ "cabac_init_present_flag" ].get<bool>( );
		num_ref_idx_l0_default_active = sps[ "num_ref_idx_l0_default_active_minus1" ].get<unsigned int>( ) + 1;
		num_ref_idx_l1_default_active = sps[ "num_ref_idx_l1_default_active_minus1" ].get<unsigned int>( ) + 1;

		init_qp = sps[ "init_qp_minus26" ].get<int>( ) + 26;
		constrained_intra_pred_flag = sps[ "constrained_intra_pred_flag" ].get<bool>( );
		transform_skip_enabled_flag = sps[ "transform_skip_enabled_flag" ].get<bool>( );
		cu_qp_delta_enabled_flag = sps[ "cu_qp_delta_enabled_flag" ].get<bool>( );
		if(cu_qp_delta_enabled_flag )
		{
			diff_cu_qp_delta_depth = sps[ "diff_cu_qp_delta_depth " ].get<unsigned int>( );
		}

		cb_qp_offset = sps[ "pps_cb_qp_offset" ].get<int>( );
		cr_qp_offset = sps[ "pps_cr_qp_offset" ].get<int>( );
		slice_chroma_qp_offsets_present_flag = sps[ "pps_slice_chroma_qp_offsets_present_flag" ].get<bool>( );

		weighted_pred_flag = sps[ "weighted_pred_flag" ].get<bool>( );
		weighted_bipred_flag = sps[ "weighted_bipred_flag" ].get<bool>( );

		transquant_bypass_enabled_flag = sps[ "transquant_bypass_enable_flag" ].get<bool>( );

		tiles_enabled_flag = sps[ "tiles_enabled_flag" ].get<bool>( );
		entropy_coding_sync_enabled_flag = sps[ "entropy_coding_sync_enabled_flag" ].get<bool>( );
		if(tiles_enabled_flag)
		{
			loop_filter_across_tiles_enabled_flag = sps[ "loop_filter_across_tiles_enabled_flag" ].get<bool>( );
		}

		loop_filter_across_slices_enabled_flag = sps[ "pps_loop_filter_across_slices_enabled_flag" ].get<bool>( );

		deblocking_filter_control_present_flag = sps[ "deblocking_filter_control_present_flag" ].get<bool>( );
		if( deblocking_filter_control_present_flag )
		{
			deblocking_filter_override_enabled_flag = sps[ "deblocking_filter_override_enabled_flag" ].get<bool>( );
			deblocking_filter_disabled_flag = sps[ "deblocking_filter_disabled_flag" ].get<bool>( );
			tc_offset = sps[ "tc_offset" ].get<bool>( );
			beta_offset = sps[ "beta_offset" ].get<bool>( );
		}

		scaling_list_data_present_flag = sps[ "pps_scaling_list_data_present_flag" ].get<bool>( );
		lists_modification_present_flag = sps[ "lists_modification_present_flag" ].get<bool>( );

		parallel_merge_level = 1 << ( sps[ "log2_parallel_merge_level_minus2" ].get<unsigned int>( ) + 2);

		slice_segment_header_extension_present_flag = sps[ "slice_segment_header_extension_present_flag" ].get<bool>( );
		extension_flag = sps[ "pps_extension_present_flag" ].get<bool>( );

		refresh( );
	}

	void PictureParameterSet::initWithDefaults( )
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

		refresh( );
	}

	void PictureParameterSet::refresh( )
	{
		refreshChromaQP( );
		if( sps != nullptr )
			resetZScanArray( );
		log2_parallel_merge_level = Calc::log2( parallel_merge_level );
	}

	int PictureParameterSet::getQP( ImgComp comp /*= ImgComp::Luma*/ ) const
	{
		if( comp == ImgComp::Luma )
			return init_qp;
		else if( comp == ImgComp::Cb )
			return cb_init_qp;
		else
			return cr_init_qp;
	}

	void PictureParameterSet::refreshChromaQP( )
	{
		cb_init_qp = init_qp + cb_qp_offset;
		cr_init_qp = init_qp + cr_qp_offset;
	}

	void PictureParameterSet::resetZScanArray( )
	{
		int y, x, m, p, i, tbX, tbY, ctbAddrRs;
		int minTUSize = sps->log2_min_transform_block_size, CTUSize = sps->log2_ctu_size;
		int PicWidthInCTUs = sps->pic_width_in_ctus, PicHeightInCTUs = sps->pic_height_in_ctus;

		if( minTUSize < 2 || CTUSize < 4 || PicHeightInCTUs <= 0 || PicHeightInCTUs <= 0 )
			return;

		int yLimit = PicHeightInCTUs << ( CTUSize - minTUSize );
		int xLimit = PicWidthInCTUs << ( CTUSize - minTUSize );

		z_scan_array = Matrix<size_t>( xLimit, yLimit );

		for( y = 0; y < yLimit; ++y )
		{
			for( x = 0; x < xLimit; ++x )
			{
				tbX = ( x << minTUSize ) >> CTUSize;
				tbY = ( y << minTUSize ) >> CTUSize;
				ctbAddrRs = PicWidthInCTUs * tbY + tbX;
				z_scan_array( x, y ) = ctbAddrRs << ( ( CTUSize - minTUSize ) * 2 );
				for( i = 0, p = 0; i < ( CTUSize - minTUSize ); i++ )
				{
					m = 1 << i;
					p += ( m & x ? m * m : 0 ) + ( m & y ? 2 * m * m : 0 );
				}
				z_scan_array( x, y ) += p;
			}
		}
	}
}
