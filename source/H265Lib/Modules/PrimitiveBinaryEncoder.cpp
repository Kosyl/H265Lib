#include <Modules/PrimitiveBinaryEncoder.h>
#include <cassert>

namespace HEVC
{
	HEVC::Bitset PrimitiveBinaryEncoder::unsigned_fixed_length(int value, int length)
	{
		return Bitset(length, value);
	}


	HEVC::Bitset PrimitiveBinaryEncoder::unsigned_exp_golomb(int value, int *out_len /* = nullptr*/)
	{
		int len = 1;
		int tmp = ++value;

		assert(tmp);

		while (1 != tmp)
		{
			tmp >>= 1;
			len += 2;
		}

		if (out_len != nullptr)
			*out_len = len;

		return Bitset(len >> 1, 0) + Bitset((len + 1) >> 1, value);
	}
}