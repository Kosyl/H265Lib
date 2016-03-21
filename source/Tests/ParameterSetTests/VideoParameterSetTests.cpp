#include "stdafx.h"
#include "CppUnitTest.h"
#include "ParameterSets/VideoParameterSet.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace HEVC;

namespace ParameterSetTests
{
	TEST_CLASS(VideoParameterSetTests)
	{
	public:

		TEST_METHOD(DefaultInitTest)
		{
			HEVC::VideoParameterSet vps(1);
			Assert::IsTrue(vps.max_layers== 1);
		}

		TEST_METHOD(VideoParameterBankTest)
		{
			auto vps = HEVC::VideoParameterSetBank::instance().createNext();
			Assert::IsTrue(vps->max_layers == 1);
		}
	};
}