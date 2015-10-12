#pragma once

#include <algorithm>
#include "TypeDef.h"

namespace H265Lib
{
	namespace Calc
	{
		template <typename T> inline T clipToRange(T minVal, T maxVal, T a);

		Int log2Int(const Int val);

		size_t numBits(const Int value);

		size_t numBits(const UInt value);

#include "CalcImpl.cpp"
	}
}