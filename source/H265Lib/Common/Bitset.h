#pragma once
#include <vector>

#define BOOL(x) (!(!(x)))
#define BIT_POS(x)  (7- (x% 8))

namespace HEVC
{
	class Bitset
	{
	private:
		std::vector<uint8_t> data{ 0 };
		size_t m_size{ 0 };

		void setBit(size_t position, bool value)
		{
			auto idx = position / 8;
			uint8_t mask = ~(1 << BIT_POS(position));
 			data[idx] = (data[idx] & mask) | (uint8_t(value) << BIT_POS(position));
		}

	public:
		Bitset() = default;
		Bitset(size_t size) :
			m_size(size),
			data((size + 7) / 8, 0)
		{

		}

		template<typename T>
		Bitset(size_t size, T value) :
			Bitset(size)
		{
			for (size_t i = 0; i < size; ++i)
			{
#pragma warning( push )
#pragma warning( disable : 4800 )
				auto bit = value & (1 << (size - i - 1));
				setBit(i, BOOL(bit));
#pragma warning( pop )
			}
		}

		Bitset operator +(const Bitset& other)
		{
			Bitset res(*this);

			for (size_t i = 0; i < other.m_size; ++i)
			{
				res.push_back(other[i]);
			}
			return res;
		}

		Bitset& operator +=(const Bitset & other)
		{
			for (size_t i = 0; i < other.m_size; ++i)
			{
				push_back(other[i]);
			}
			return *this;
		}

		template<class Tparent>
		class bit_reference
		{
		private:
			Tparent* parent;
			size_t bit_position;
		public:
			bit_reference(Tparent* b, size_t pos) :
				parent(b),
				bit_position(pos)
			{

			}

			template<typename T>
			bit_reference& operator=(T value)
			{
				parent->setBit(bit_position, BOOL(value));
				return *this;
			}

			operator bool() const
			{
				return !(!(parent->data[bit_position / 8] & (1 << BIT_POS( bit_position))));
			}

			char str()
			{
				return bool(*this) ? '1' : '0';
			}
		};

		friend class bit_reference < Bitset >;
		friend class bit_reference < const Bitset >;

		typedef bit_reference<Bitset> bit_ref;
		typedef bit_reference<const Bitset> cbit_ref;

		bit_ref at(size_t position)
		{
			return bit_ref(this, position);
		}

		cbit_ref at(size_t position) const
		{
			return cbit_ref(this, position);
		}

		bit_ref operator[](size_t position)
		{
			return at(position);
		}

		cbit_ref operator[](size_t position) const
		{
			return at(position);
		}

		std::vector<uint8_t>::const_iterator byte_cbegin()
		{
			return data.cbegin();
		}
		std::vector<uint8_t>::const_iterator byte_cend()
		{
			return data.cend();
		}

		size_t size()
		{
			return m_size;
		}

		template<typename T>
		void push_back(T value)
		{
			++m_size;
			if ((m_size+7)/8 > data.size())
				data.push_back(0u);

			setBit(m_size - 1, BOOL(value));
		}

		std::string str()
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

		bool is_byte_aligned() { return m_size % 8 == 0; }
	};
}

