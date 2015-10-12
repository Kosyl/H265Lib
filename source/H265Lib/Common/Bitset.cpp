#include "Common/Bitset.h"

namespace H265Lib
{
	Bitset::Bitset(size_t size) :
		vector<bool>(size)
	{

	}
	Bitset::Bitset() :
		vector<bool>(0)
	{

	}

	std::string Bitset::str()
	{
		std::string s;
		for (size_t i = 0; i < size(); ++i)
		{
			s += (*this).at(i) ? "1" : "0";
		}
		return s;
	}
}