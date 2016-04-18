#include "stdafx.h"
#include "CppUnitTest.h"
#include <ParameterSets/SequenceParameterSet.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace HEVC;

namespace ParameterSetTests
{
	TEST_CLASS(SequenceParameterSetTests)
	{
	public:

		TEST_METHOD(DefaultInitTest)
		{
			SequenceParameterSet sps(1);
			Assert::IsTrue(sps.getBitDepth(ImgComp::Luma) == 8);
		}

		TEST_METHOD(SequenceParameterBankTest)
		{
			auto sps = SequenceParameterSetBank::instance().createNext();
			Assert::IsTrue(sps->getBitDepth(ImgComp::Luma) == 8);
		}

		TEST_METHOD(Subsampling420_GivesCorrectPictureSizes)
		{
			auto sps = SequenceParameterSetBank::instance().createNext();
			sps->chroma_format_idc = SubsamplingFormat::Mode_420;
			sps->setPicSize(100, 100);

			Assert::IsTrue(sps->getPicHeight(ImgComp::Luma) == 100);
			Assert::IsTrue(sps->getPicWidth(ImgComp::Luma) == 100);
			Assert::IsTrue(sps->getPicHeight(ImgComp::Cr) == 50);
			Assert::IsTrue(sps->getPicWidth(ImgComp::Cr) == 50);
			Assert::IsTrue(sps->getPicHeight(ImgComp::Cb) == 50);
			Assert::IsTrue(sps->getPicWidth(ImgComp::Cb) == 50);
		}

		TEST_METHOD(Subsampling422_GivesCorrectPictureSizes)
		{
			auto sps = SequenceParameterSetBank::instance().createNext();
			sps->chroma_format_idc = SubsamplingFormat::Mode_422;
			sps->setPicSize(100, 100);

			Assert::IsTrue(sps->getPicHeight(ImgComp::Luma) == 100);
			Assert::IsTrue(sps->getPicWidth(ImgComp::Luma) == 100);
			Assert::IsTrue(sps->getPicHeight(ImgComp::Cr) == 100);
			Assert::IsTrue(sps->getPicWidth(ImgComp::Cr) == 50);
			Assert::IsTrue(sps->getPicHeight(ImgComp::Cb) == 100);
			Assert::IsTrue(sps->getPicWidth(ImgComp::Cb) == 50);
		}

		TEST_METHOD(Subsampling444_GivesCorrectPictureSizes)
		{
			auto sps = SequenceParameterSetBank::instance().createNext();
			sps->chroma_format_idc = SubsamplingFormat::Mode_444;
			sps->setPicSize(100, 100);

			Assert::IsTrue(sps->getPicHeight(ImgComp::Luma) == 100);
			Assert::IsTrue(sps->getPicWidth(ImgComp::Luma) == 100);
			Assert::IsTrue(sps->getPicHeight(ImgComp::Cr) == 100);
			Assert::IsTrue(sps->getPicWidth(ImgComp::Cr) == 100);
			Assert::IsTrue(sps->getPicHeight(ImgComp::Cb) == 100);
			Assert::IsTrue(sps->getPicWidth(ImgComp::Cb) == 100);
		}
		
		TEST_METHOD(ZscanArray_InitializationTest)
		{
			auto sps = SequenceParameterSetBank::instance().createNext();
			
			Log::printMatrix(*sps->_zScanArray);

			int arr[4][4] = 
			{
				{1,2,5,6},
				{3,4,7,8},
				{9,10,13,14},
				{11,12,15,16}
			};

			Log::println(arr[0][1]);
			Log::println(arr[1][0]);
			Log::println(arr[1][1]);
			Log::println(arr[2][3]);
		}
	};
}