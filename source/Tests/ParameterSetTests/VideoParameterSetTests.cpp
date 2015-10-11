#include "stdafx.h"
#include "CppUnitTest.h"
#include "ParameterSets/VideoParameterSet.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace H265Lib;

namespace ParameterSetTests
{
	TEST_CLASS(VideoParameterSetTests)
	{
	public:

		TEST_METHOD(DefaultInitTest)
		{
			H265Lib::VideoParameterSet vps(1);
			Assert::IsTrue(vps.getMaxLayers() == 1);
		}

		TEST_METHOD(VideoParameterBankTest)
		{
			auto vps = H265Lib::VideoParameterSetBank::instance().createNext();
			Assert::IsTrue(vps->getMaxLayers() == 1);
		}
	};
}