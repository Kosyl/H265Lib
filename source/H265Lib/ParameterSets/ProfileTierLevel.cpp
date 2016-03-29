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
}