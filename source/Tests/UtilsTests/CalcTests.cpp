#include "stdafx.h"
#include <Common/Calc.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace HEVC;

namespace UnitTests
{
	TEST_CLASS(CalcTests)
	{
	public:

		TEST_METHOD(clipToRange_withinRange_leavesUnchanged)
		{
			int i = 40;

			auto result = Calc::clipToRange(0, 255, i);

			Assert::AreEqual(i, result);
		}

		TEST_METHOD(clipToRange_lowerThanRange_returnsLowerBound)
		{
			int i = 40;

			auto result = Calc::clipToRange(90, 255, i);

			Assert::AreEqual(90, result);
		}

		TEST_METHOD(clipToRange_higherThanRange_returnsUpperBound)
		{
			unsigned int i = 400;

			auto result = Calc::clipToRange(90u, 255u, i);

			Assert::AreEqual(255u, result);
		}

		TEST_METHOD(log2Int_valuesTest)
		{
			Assert::AreEqual(Calc::log2(0), 0);
			Assert::AreEqual(Calc::log2(1), 0);
			Assert::AreEqual(Calc::log2(2), 1);
			Assert::AreEqual(Calc::log2(3), 1);
			Assert::AreEqual(Calc::log2(4), 2);
			Assert::AreEqual(Calc::log2(5), 2);
			Assert::AreEqual(Calc::log2(8), 3);
		}

		TEST_METHOD(numBits_valuesTest)
		{
			Assert::AreEqual(Calc::numBits(0), 1);
			Assert::AreEqual(Calc::numBits(1), 1);
			Assert::AreEqual(Calc::numBits(2), 2);
			Assert::AreEqual(Calc::numBits(3), 2);
			Assert::AreEqual(Calc::numBits(4), 3);
			Assert::AreEqual(Calc::numBits(5), 3);
			Assert::AreEqual(Calc::numBits(8), 4);
		}

		TEST_METHOD(zscan_idx_valuesTest)
		{
			Assert::AreEqual(Calc::zscan_idx(0, 0), 0u);
			Assert::AreEqual(Calc::zscan_idx(1, 0), 1u);
			Assert::AreEqual(Calc::zscan_idx(0, 1), 2u);
			Assert::AreEqual(Calc::zscan_idx(1, 1), 3u);
			Assert::AreEqual(Calc::zscan_idx(2, 0), 4u);
			Assert::AreEqual(Calc::zscan_idx(3, 0), 5u);
			Assert::AreEqual(Calc::zscan_idx(2, 1), 6u);
			Assert::AreEqual(Calc::zscan_idx(3, 1), 7u);
			Assert::AreEqual(Calc::zscan_idx(62,62), 4092u);
			Assert::AreEqual(Calc::zscan_idx(63, 62), 4093u);
			Assert::AreEqual(Calc::zscan_idx(62, 63), 4094u);
			Assert::AreEqual(Calc::zscan_idx(63,63), 4095u);
		}
	};
}