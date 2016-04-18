#pragma once
#include <vector>

#define AS_BOOL(x) (!(!(x)))
#define BIT_POS(x) (7- (x% 8))

namespace HEVC
{
	class Bitset
	{
	private:
		std::vector<uint8_t> m_data{ 0 };
		size_t m_size{ 0 };

		void setBit(size_t position, bool value);

	public:
		Bitset() = default;
		Bitset(size_t size);

		template<typename T>
		Bitset(size_t size, T value) :
			Bitset(size)
		{
			for (size_t i = 0; i < size; ++i)
			{
#pragma warning( push )
#pragma warning( disable : 4800 )
				auto bit = value & (1 << (size - i - 1));
				setBit(i, AS_BOOL(bit));
#pragma warning( pop )
			}
		}

		Bitset operator +(const Bitset& other);

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
				parent->setBit(bit_position, AS_BOOL(value));
				return *this;
			}

			operator bool() const
			{
				return !(!(parent->m_data[bit_position / 8] & (1 << BIT_POS( bit_position))));
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
			return m_data.cbegin();
		}
		std::vector<uint8_t>::const_iterator byte_cend()
		{
			return m_data.cend();
		}

		size_t size()
		{
			return m_size;
		}

		template<typename T>
		void push_back(T value)
		{
			++m_size;
			if ((m_size+7)/8 > m_data.size())
				m_data.push_back(0u);

			setBit(m_size - 1, AS_BOOL(value));
		}

		std::string str();

		bool is_byte_aligned() { return m_size % 8 == 0; }
	};
}

