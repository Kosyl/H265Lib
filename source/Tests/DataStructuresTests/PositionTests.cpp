#include "stdafx.h"
#include <Common/Bitset.h>
#include <Data/BlockBase.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace HEVC;

namespace UnitTests
{
	TEST_CLASS(PositionTests)
	{
	public:

		TEST_METHOD(defaultCtorTest)
		{
			Position p;

			Assert::IsTrue(p.x == 0);
			Assert::IsTrue(p.y == 0);
			Assert::IsTrue(p.idx == 0);
		}

		TEST_METHOD(indexingTest)
		{
			std::vector<Position> pos;

			for (int i = 0; i < 10; ++i)
			{
				pos.push_back(Position(i, i));
			}

			auto sps = SequenceParameterSetBank::instance().createNext();
			sps->setPicSize(100, 100);

			for (auto p : pos)
			{
				p.resolveIdx(*sps, Indexing::ZScanByPixel);
				Log::println("(", p.x, ",", p.y, ") -> ", p.idx);
			}
		}
	};
}