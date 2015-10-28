#pragma once

#include <algorithm>
#include "TypeDef.h"

namespace H265Lib
{
	namespace Calc
	{
		template <typename T> inline T clipToRange(T minVal, T maxVal, T a)
		{
			return std::min<T>(std::max<T>(minVal, a), maxVal);
		}

		Int log2Int(const Int val);

		size_t numBits(const Int value);

		size_t numBits(const UInt value);
	}
}