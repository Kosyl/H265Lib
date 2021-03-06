#include "stdafx.h"
#include <Common/Logger.h>
#include <chrono>
#include <Data/Picture.h>
#include <Data/CTU.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace HEVC;

namespace UnitTests
{
	TEST_CLASS(PictureTests)
	{
	public:

		TEST_METHOD(DefaultCtorTest)
		{
			Picture p;

			//Assert::IsTrue(p.getInputSamples(ImgComp::Luma) == nullptr);
		}

		TEST_METHOD(ParametersCtor_SetsCorrectPictureSize)
		{
			ParametersBundle parameters = ParametersBundle::getDefaultParameters( 40, 60 );

			Picture p;
			p.initFromParameters(parameters );

			Assert::IsTrue(p.input_samples[Luma].height() == 60);
			Assert::IsTrue(p.input_samples[Luma].width() == 40);
			Assert::IsTrue(p.input_samples[Cr].height() == 30);
			Assert::IsTrue(p.input_samples[Cr].width() == 20);
			Assert::IsTrue(p.input_samples[Cb].height() == 30);
			Assert::IsTrue(p.input_samples[Cb].width() == 20);
		}

		TEST_METHOD(getCTUBySamplePosition_ReturnsCorrectCTU)
		{
			ParametersBundle parameters = ParametersBundle::getDefaultParameters( 200,200 );

			parameters.Sps->chroma_format_idc = SubsamplingFormat::Mode_420;
			parameters.Sps->max_luma_coding_block_size = 64;

			Picture p;
			p.initFromParameters(parameters);

			auto ctu00 = p.getCTUBySamplePosition(0, 0);
			Assert::IsTrue(ctu00->pos.x == 0);
			Assert::IsTrue(ctu00->pos.y == 0);

			auto ctu10 = p.getCTUBySamplePosition(75, 0);
			Assert::IsTrue(ctu10->pos.x == 64);
			Assert::IsTrue(ctu10->pos.y == 0);

			auto ctu01 = p.getCTUBySamplePosition(63, 64);
			Assert::IsTrue(ctu01->pos.x == 0);
			Assert::IsTrue(ctu01->pos.y == 64);

			auto ctu11 = p.getCTUBySamplePosition(64, 126);
			Assert::IsTrue(ctu11->pos.x == 64);
			Assert::IsTrue(ctu11->pos.y == 64);
		}
	};
}