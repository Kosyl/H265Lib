#include "stdafx.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Logger.h"
#include "ParameterSets/SequenceParameterSet.h"
#include <chrono>

using namespace testing;
using namespace H265Lib;

class SeqParamsTests : public testing::Test
{
};

TEST_F(SeqParamsTests, SetsDoNotOverlap)
{
	auto params1 = SequenceParametersBank::instance().createNext();
	auto params2 = SequenceParametersBank::instance().createNext();
	auto params3 = SequenceParametersBank::instance().createNext();

	params1->setBitDepth(ImgComp::Luma, 8);
	params2->setBitDepth(ImgComp::Luma, 9);
	params3->setBitDepth(ImgComp::Luma, 10);

	ASSERT_THAT(params1->getBitDepthLuma(), Eq(8));
	ASSERT_THAT(SequenceParametersBank::instance().getSetByIdx(1)->getBitDepthLuma(), Eq(9));
	ASSERT_THAT(params3->getBitDepthLuma(), Eq(10));
}


TEST_F(SeqParamsTests, ZArrayTest)
{
	SequenceParametersBank::instance().createNext();

	auto params = SequenceParametersBank::instance().getCurrent();

	params->initWithDefaults();
	params->setPicHeight(128);
	params->setPicWidth(128);
	params->resetZScanArray();

	auto array = params->getZScanArrayPtr();
	LOGLN(Logs::Dump, "zScanArray:");
	LOG_MATRIX(Logs::Dump, array);

	Matrix<size_t> referenceMatrix(128, 128);
	Matrix<size_t> pixelReferenceMatrix(128, 128);

	for (size_t x = 0; x < 128; ++x)
	{
		for (size_t y = 0; y < 128; ++y)
		{
			referenceMatrix.at(x, y) = params->calcZScanIdxOf4x4BlockIn64x64BlockByPixel(x, y);
			pixelReferenceMatrix.at(x, y) = params->getSmallestBlockZScanIdxByPixel(x, y);
		}
	}

	LOGLN(Logs::Dump, "reference:");
	LOG_MATRIX(Logs::Dump, referenceMatrix);
	LOGLN(Logs::Dump, "pixel reference:");
	LOG_MATRIX(Logs::Dump, pixelReferenceMatrix);
}