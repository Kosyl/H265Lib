#include "Calc.h"

namespace H265Lib
{
	namespace Calc
	{
		Int log2Int(const Int value)
		{
			if (value == 0)
				return 0;

			int log2 = 0, val = value;
			for (; val != 1; val >>= 1, log2++);
			return log2;
		}

		size_t numBits(const int value)
		{
			return log2Int(value) + 1;
		}

		size_t numBits(const UInt value)
		{
			return numBits((Int)value);
		}
	}
}