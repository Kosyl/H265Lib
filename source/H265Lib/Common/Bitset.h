#pragma once
#include <vector>

namespace H265Lib
{
	class Bitset : public std::vector < bool >
	{
	public:
		Bitset(size_t size);

		Bitset();

		std::string str();
	};
}