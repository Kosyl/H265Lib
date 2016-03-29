#pragma once
#include <Common/Constants.h>

namespace HEVC
{
	class ProfileTierLevel
	{
	public:
		
		int general_profile_space;
		int general_tier_flag;
		int general_profile_idc;

		bool general_profile_compatibility_flag[Constants::NUM_PROFILES];

		bool general_progressive_source_flag;
		bool general_interlaced_source_flag;
		bool general_non_packed_constraint_flag;
		bool general_frame_only_constraint_flag;

		int general_level_idc;

		ProfileTierLevel() = default;
		void initWithDefaults();
	};
}
