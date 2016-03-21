#pragma once
#include <Common\TypeDef.h>

namespace HEVC
{
	namespace Constants
	{
		static const int CG_SIZE = 4;

		static const int NUM_COEFFS_IN_CG = CG_SIZE * CG_SIZE;

		static const int COEFF_REMAIN_THRESHOLD = 3;

		static const int SDH_THRESHOLD = 4;
	};
}
