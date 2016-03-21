#pragma once

#include <Common/TypeDef.h>
#include <vector>

namespace HEVC
{
	struct ReferenceSamples
	{
		Sample Corner;
		std::vector<Sample> Upper;
		std::vector<Sample> Left;

		ReferenceSamples(size_t blockSize) :
			Corner(0),
			Upper(blockSize * 2),
			Left(blockSize * 2)
		{

		}
	};
}
