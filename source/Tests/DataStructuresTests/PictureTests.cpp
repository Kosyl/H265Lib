#include "stdafx.h"
#include <Common/Logger.h>
#include <chrono>
#include <Data/Picture.h>
#include <Data/CTU.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace H265Lib;

namespace UnitTests
{
	TEST_CLASS(PictureTests)
	{
	public:

		TEST_METHOD(DefaultCtorTest)
		{
			Picture p;

			Assert::IsTrue(p.getInputSamples(ImgComp::Luma) == nullptr);
		}

		TEST_METHOD(ParametersCtor_SetsCorrectPictureSize)
		{
			auto sps = SequenceParameterSetBank::instance().createNext();
			auto pps = PictureParameterSetBank::instance().createNext();

			sps->setPicSize(40, 60);
			sps->setSubsamplingFormat(SubsamplingFormat::Mode_420);

			Picture p(ParametersBundle(sps,pps));

			Assert::IsTrue(p.getInputSamples(ImgComp::Luma) != nullptr);
			Assert::IsTrue(p.getInputSamples(ImgComp::Luma)->height() == 60);
			Assert::IsTrue(p.getInputSamples(ImgComp::Luma)->width() == 40);
			Assert::IsTrue(p.getInputSamples(ImgComp::Cr)->height() == 30);
			Assert::IsTrue(p.getInputSamples(ImgComp::Cr)->width() == 20);
			Assert::IsTrue(p.getInputSamples(ImgComp::Cb)->height() == 30);
			Assert::IsTrue(p.getInputSamples(ImgComp::Cb)->width() == 20);
		}

		TEST_METHOD(getCTUBySamplePosition_ReturnsCorrectCTU)
		{
			auto sps = SequenceParameterSetBank::instance().createNext();
			auto pps = PictureParameterSetBank::instance().createNext();

			sps->setPicSize(200, 200);
			sps->setSubsamplingFormat(SubsamplingFormat::Mode_420);
			sps->setMaxCUSize(64);

			Picture p(ParametersBundle(sps, pps));

			auto ctu00 = p.getCTUBySamplePosition(0, 0);
			Assert::IsTrue(ctu00->PositionInPicture.X == 0);
			Assert::IsTrue(ctu00->PositionInPicture.Y == 0);

			auto ctu10 = p.getCTUBySamplePosition(75, 0);
			Assert::IsTrue(ctu10->PositionInPicture.X == 64);
			Assert::IsTrue(ctu10->PositionInPicture.Y == 0);

			auto ctu01 = p.getCTUBySamplePosition(63, 64);
			Assert::IsTrue(ctu01->PositionInPicture.X == 0);
			Assert::IsTrue(ctu01->PositionInPicture.Y == 64);

			auto ctu11 = p.getCTUBySamplePosition(64, 126);
			Assert::IsTrue(ctu11->PositionInPicture.X == 64);
			Assert::IsTrue(ctu11->PositionInPicture.Y == 64);
		}
	};
}