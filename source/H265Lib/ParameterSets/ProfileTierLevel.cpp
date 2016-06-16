#include <ParameterSets/ProfileTierLevel.h>

namespace HEVC
{
	void ProfileTierLevel::initWithDefaults()
	{
		general_profile_space = 0;
		general_tier_flag = 0;
		general_profile_idc = 1;

		general_profile_compatibility_flag[0] = true;
		general_profile_compatibility_flag[1] = true;

		for (int i = 2; i < Constants::NUM_PROFILES; ++i)
		{
			general_profile_compatibility_flag[i] = false;
		}

		general_progressive_source_flag = false;
		general_interlaced_source_flag = false;
		general_non_packed_constraint_flag = false;
		general_frame_only_constraint_flag = false;

		general_level_idc = 0;
	}

	void ProfileTierLevel::initFromJson(nlohmann::json& ptl)
	{
		general_profile_space = ptl["profile_space"].get<unsigned int>();
		general_tier_flag = ptl[ "tier_flag" ].get<bool>( );
		general_profile_idc = ptl[ "profile_idc" ].get<unsigned int>( );

		for( int i = 0; i < Constants::NUM_PROFILES; ++i )
		{
			general_profile_compatibility_flag[ i ] = ptl[ "profile_compatibility_flags" ][i].get<bool>( );
		}

		general_progressive_source_flag = ptl[ "progressive_source_flag" ].get<bool>( );
		general_interlaced_source_flag = ptl[ "interlaced_source_flag" ].get<bool>( );
		general_non_packed_constraint_flag = ptl[ "non_packed_constraint_flag" ].get<bool>( );
		general_frame_only_constraint_flag = ptl[ "frame_only_constraint_flag" ].get<bool>( );

		general_level_idc = ptl[ "general_level_idc" ].get<unsigned int>( );
	}
}
