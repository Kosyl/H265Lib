#include "stdafx.h"
#include <Common/Bitset.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace H265Lib;

namespace UnitTests
{
	TEST_CLASS(BitsetTests)
	{
	public:
		
		TEST_METHOD(push_backTest)
		{
			H265Lib::Bitset b(5);

			b.push_back(1);

			Assert::AreEqual(b.size(), 6u);
			Assert::AreEqual(static_cast<bool>(b[5]), true);
		}

		TEST_METHOD(emptyBitsetTest)
		{
			H265Lib::Bitset b(0);

			Assert::AreEqual(b.size(), 0u);
			b.push_back(1);

			Assert::AreEqual(b.size(), 1u);
			Assert::AreEqual(static_cast<bool>(b[0]), true);
		}

		TEST_METHOD(StringifyTest)
		{
			H265Lib::Bitset b(5);

			b[2] = 1;

			auto s = b.str();
			Log::println("s: ", b.str());
			Assert::AreEqual(std::string("00100"), s);

			b[3] = 1;

			s = b.str();
			Log::println("s: ", b.str());
			Assert::AreEqual(std::string("00110"), s);

			b.push_back(1);

			s = b.str();
			Log::println("s: ", b.str());
			Assert::AreEqual(std::string("001101"), s);
		}

		TEST_METHOD(CopyCtorTest)
		{
			H265Lib::Bitset b(5);

			b[2] = 1;

			auto copy(b);
			copy.push_back(1);

			Assert::AreEqual(copy.size(), 6u);
			Assert::AreEqual(b.size(), 5u);
		}
	};
}