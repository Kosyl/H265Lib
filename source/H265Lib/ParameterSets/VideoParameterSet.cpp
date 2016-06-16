#include "VideoParameterSet.h"

namespace HEVC
{
	void VideoParameterSet::initWithDefaults( )
	{
		base_layer_internal_flag = true;
		base_layer_available_flag = true;

		max_layers = 1;
		max_sub_layers = 1;

		temporal_id_nesting_flag = true;

		profile_tier_level.initWithDefaults( );

		sub_layer_ordering_info_present_flag = true;
		sub_layer_ordering_infos.push_back( SubLayerOrderingInfo( 2, 0, -1 ) );

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

	VideoParameterSet::VideoParameterSet( int idx ): ParameterSetBase( idx )
	{
		initWithDefaults( );
	}

	VideoParameterSet::~VideoParameterSet( )
	{

	}

	void VideoParameterSet::configure( EncoderParameters configuration )
	{
		nlohmann::json& vps = configuration.complete_configuration[ "vps" ];

		base_layer_internal_flag = vps[ "vps_base_layer_internal_flag" ].get<bool>( );
		base_layer_available_flag = vps[ "vps_base_layer_internal_flag" ].get<bool>( );

		max_layers = vps[ "vps_max_layers_minus1" ].get<unsigned int>( ) + 1;
		max_sub_layers = vps[ "vps_max_sub_layers_minus1" ].get<unsigned int>( ) + 1;

		temporal_id_nesting_flag = vps[ "vps_temporal_id_nesting_flag" ].get<bool>( );

		profile_tier_level.initFromJson( vps[ "profileTierLevel" ] );

		sub_layer_ordering_info_present_flag = vps[ "vps_sub_layer_ordering_info_present_flag" ].get<bool>( );
		sub_layer_ordering_infos.clear( );
		if( sub_layer_ordering_info_present_flag )
		{
			for( auto& singleInfo : vps[ "subLayerOrderingInfo" ] )
			{
				sub_layer_ordering_infos.push_back( SubLayerOrderingInfo( singleInfo ) );
			}
		}

		max_layer_id = vps[ "vps_max_layer_id" ].get<unsigned int>( );
		num_layer_sets = vps[ "vps_num_layer_sets_minus1" ].get<unsigned int>( ) + 1;
		timing_info_present_flag = vps[ "vps_timing_info_present_flag" ].get<bool>( );
		if( timing_info_present_flag )
		{
			num_units_in_tick = vps[ "vps_base_layer_internal_flag" ].get<bool>( );
			time_scale = vps[ "vps_base_layer_internal_flag" ].get<bool>( );
			poc_proportional_to_timing_flag = vps[ "vps_base_layer_internal_flag" ].get<bool>( );
			num_ticks_poc_diff_one = vps[ "vps_base_layer_internal_flag" ].get<bool>( );
			num_hrd_parameters = vps[ "vps_base_layer_internal_flag" ].get<bool>( );
		}
		extension_flag = vps[ "vps_extension_flag" ].get<bool>( );
	}
}
