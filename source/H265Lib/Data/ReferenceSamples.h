#pragma once

#include <Common/TypeDef.h>
#include <Common/Enums.h>
#include <vector>
#include <cassert>

namespace HEVC
{
	template<typename T>
	struct IntraReferenceData
	{
		size_t block_size;
		T Corner;
		std::vector<T> Top;
		std::vector<T> Left;

		IntraReferenceData(size_t blockSize) :
			block_size(blockSize),
			Corner(0),
			Top(blockSize * 2),
			Left(blockSize * 2)
		{

		}

		std::vector<T>& operator[](IntraDirection dir)
		{
			assert(dir != IntraDirection::Corner);

			return (dir == IntraDirection::Left) ? Left : Top;
		}
		std::vector<T>& side_to(IntraDirection dir)
		{
			assert(dir != IntraDirection::Corner);

			return (dir == IntraDirection::Left) ? Top : Left;
		}
	};

	using IntraReferenceSamples = IntraReferenceData<Sample>;
	using IntraReferenceAvailability = IntraReferenceData<bool>;
}
