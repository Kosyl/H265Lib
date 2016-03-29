#include "stdafx.h"
#include <Common/Bitset.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace HEVC;

namespace UnitTests
{
	TEST_CLASS(BitsetTests)
	{
	public:
		
		TEST_METHOD(push_backTest)
		{
			HEVC::Bitset b(5);

			b.push_back(1);

			Assert::AreEqual(b.size(), 6u);
			Assert::AreEqual(static_cast<bool>(b[5]), true);
		}

		TEST_METHOD(emptyBitsetTest)
		{
			HEVC::Bitset b(0);

			Assert::AreEqual(b.size(), 0u);
			b.push_back(1);

			Assert::AreEqual(b.size(), 1u);
			Assert::AreEqual(static_cast<bool>(b[0]), true);
		}

		TEST_METHOD(StringifyTest)
		{
			Log::println(__FUNCTION__);

			HEVC::Bitset b(5);

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
			HEVC::Bitset b(5);

			b[2] = 1;

			auto copy(b);
			copy.push_back(1);

			Assert::AreEqual(copy.size(), 6u);
			Assert::AreEqual(b.size(), 5u);
		}

		TEST_METHOD(AddOperatorTest)
		{
			Log::println(__FUNCTION__);

			HEVC::Bitset b(5, 0b01010);
			HEVC::Bitset b2(3, 0b110);

			auto b3 = b + b2;

			Assert::AreEqual(b3.size(), 8u);
			Log::println("01010 + 110: ", b3.str());
		}

		TEST_METHOD(AddAssignmentOperatorTest)
		{
			Log::println(__FUNCTION__);

			HEVC::Bitset b(5, 0b01010);
			HEVC::Bitset b2(3, 0b110);

			b2 += b;

			Assert::AreEqual(b2.size(), 8u);
			Log::println("110 + 01010 = ", b2.str());
		}

		TEST_METHOD(IndexerTest)
		{
			Log::println(__FUNCTION__);

			HEVC::Bitset b(5, 0b01011);

			Assert::IsTrue(b[0] == 0);
			Assert::IsTrue(b[1] == 1);
			Assert::IsTrue(b[2] == 0);
			Assert::IsTrue(b[3] == 1);
			Assert::IsTrue(b[4] == 1);

			Log::println("bitset:", b.str());
			Log::println("bitset[2]:", b[2].str());
			Log::println("bitset[3]:", b[3].str());
		}

		TEST_METHOD(TemplateCtorTest)
		{
			Log::println(__FUNCTION__);

			Log::println("23: ", HEVC::Bitset(5, 23).str());
			Log::println("16: ", HEVC::Bitset(5, 16).str());
			Log::println("8: ", HEVC::Bitset(5, 8).str());
			Log::println("31: ", HEVC::Bitset(5, 31).str());
		}
	};
}