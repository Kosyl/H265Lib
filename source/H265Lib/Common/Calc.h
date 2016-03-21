#pragma once

#include <algorithm>
#include "TypeDef.h"

namespace HEVC
{
	namespace Calc
	{
		template <typename T> 
		inline T clipToRange(T minVal, T maxVal, T a)
		{
			return std::min<T>(std::max<T>(minVal, a), maxVal);
		}

		template <typename T> 
		int log2(const T value)
		{
			if (value == 0)
				return 0;

			int log2 = 0, val = value;
			for (; val != 1; val >>= 1, log2++);
			return log2;
		}

		template <typename T>
		int numBits(const T value)
		{
			return log2(value) + 1;
		}
	}
}