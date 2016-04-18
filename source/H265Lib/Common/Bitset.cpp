#include "Common/Bitset.h"

namespace HEVC
{

	void Bitset::setBit(size_t position, bool value)
	{
		auto idx = position / 8;
		uint8_t mask = ~(1 << BIT_POS(position));
		m_data[idx] = (m_data[idx] & mask) | (uint8_t(value) << BIT_POS(position));
	}

	Bitset::Bitset(size_t size) :
		m_size(size),
		m_data((size + 7) / 8, 0)
	{

	}

	Bitset Bitset::operator+(const Bitset& other)
	{
		Bitset res(*this);

		for (size_t i = 0; i < other.m_size; ++i)
		{
			res.push_back(other[i]);
		}
		return res;
	}

	std::string Bitset::str()
	{
		std::string s;
		for (size_t i = 0; i < m_size; ++i)
		{
			s += at(i) ? "1" : "0";
			if ((i+1) % 8 == 0)
				s += " ";
		}
		return s;
	}
}
