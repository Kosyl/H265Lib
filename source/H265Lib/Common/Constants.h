#pragma once
#include <Common\TypeDef.h>

namespace H265Lib
{
	namespace Constants
	{
		static const Int CG_SIZE = 4;

		static const Int NUM_COEFFS_IN_CG = CG_SIZE * CG_SIZE;

		static const Int COEFF_REMAIN_THRESHOLD = 3;

		static const Int SDH_THRESHOLD = 4;
	};
}
