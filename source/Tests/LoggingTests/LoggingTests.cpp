#include "stdafx.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Common\Matrix.h"
#include "Logger.h"
#include <chrono>
#include <Common/Bitset.h>

using namespace testing;
using namespace H265Lib;

class LoggingTests : public testing::Test
{
};

TEST_F(LoggingTests, BasicConsoleLogTest)
{
	LOGLN(Logs::Console, "test print");
}

TEST_F(LoggingTests, BitsetLogTest)
{
	Bitset bitset(5);

	bitset[2] = 1;
	bitset[3] = 1;

	LOGLN(Logs::Console, "00110: ", bitset.str());
}

TEST_F(LoggingTests, MatrixLogTest)
{
	H265Lib::Matrix<int> m(4, 4);
	m.at(1, 1) = 3;
	m.at(3, 0) = 10;
	

	LOG_MATRIX(Logs::Console, m);
}

TEST_F(LoggingTests, MatrixPointerLogTest)
{
	auto m = std::make_shared<H265Lib::Matrix<int> >(4, 4);
	m->at(1, 1) = 3;
	m->at(3, 0) = 10;
	

	LOG_MATRIX(Logs::Console, m);
}