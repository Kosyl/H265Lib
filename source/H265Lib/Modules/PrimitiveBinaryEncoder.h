#pragma once
#include <Common/Bitset.h>

namespace HEVC
{
	class PrimitiveBinaryEncoder
	{
	public:

		static Bitset unsigned_fixed_length(int value, int length);
		static Bitset unsigned_exp_golomb(int value, int *out_len = nullptr);
	};
}