#pragma once

#include <Common/TypeDef.h>
#include <vector>

namespace HEVC
{
	struct ReferenceSamples
	{
		size_t block_size;
		Sample Corner;
		std::vector<Sample> Upper;
		std::vector<Sample> Left;

		ReferenceSamples(size_t blockSize) :
			block_size(blockSize),
			Corner(0),
			Upper(blockSize * 2),
			Left(blockSize * 2)
		{

		}
	};
}
