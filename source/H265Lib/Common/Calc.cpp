#include "Calc.h"
#include <Common/Constants.h>

namespace HEVC
{
	namespace Calc
	{
		size_t zscan(size_t x, size_t y, size_t range, size_t base)
		{
			if (range == 1)
			{
				return base;
			}

			range >>= 1;

			if (x >= range)
			{
				x -= range;
				base += range*range;
			}
			if (y >= range)
			{
				y -= range;
				base += 2 * range * range;
			}

			return zscan(x, y, range, base);
		}

		size_t zscan_idx(size_t x, size_t y, size_t range /* = Constants::MAX_CU_SIZE */)
		{
			return zscan(x, y, range, 0);
		}
	}
}