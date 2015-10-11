#pragma once
#include <vector>

namespace H265Lib
{
	class Bitset : public std::vector < bool >
	{
	public:
		Bitset(size_t size) :
			vector<bool>(size)
		{

		}

		Bitset() :
			vector<bool>(0)
		{

		}

		std::string str()
		{
			std::string s;
			for (size_t i = 0; i < size(); ++i)
			{
				s += (*this).at(i) ? "1" : "0";
			}
			return s;
		}
	};
}