#include "stdafx.h"
#include <Common/Logger.h>
#include <chrono>
#include <Common/Matrix.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace HEVC;

namespace UnitTests
{
	TEST_CLASS(MatrixTests)
	{
	public:

		TEST_METHOD(DefaultValuesAreTheSame)
		{
			Log::println(__FUNCTION__);

			auto m = HEVC::Matrix<int>(2, 2);

			Assert::AreEqual(m.at(0, 0), 0);
			Assert::AreEqual(m.at(1, 0), 0);
			Assert::AreEqual(m.at(0, 1), 0);
			Assert::AreEqual(m.at(1, 1), 0);
		}

		TEST_METHOD(StringifyTest)
		{
			Log::println(__FUNCTION__);

			auto m = HEVC::Matrix<int>(3, 3);

			m.at(1, 1) = 5;
			Log::printMatrix(m);
		}

		TEST_METHOD(IndexingTest)
		{
			Log::println(__FUNCTION__);

			auto m = HEVC::Matrix<int>(3, 3);

			for (auto i = 0; i < 3; ++i)
			{
				for (auto j = 0; j < 3; ++j)
				{
					m.at(j, i) = 3 * i + j;
				}
			}
			Log::printMatrix(m);
			Assert::AreEqual(m.at(0, 0), 0);
			Assert::AreEqual(m.at(1, 1), 4);
			Assert::AreEqual(m.at(2, 2), 8);
			Assert::AreEqual(m.at(2, 0), 2);
		}

		TEST_METHOD(CopyCtorTest)
		{
			Log::println(__FUNCTION__);

			auto m = HEVC::Matrix<int>(3, 3);
			auto m2(m);

			Assert::AreEqual(m.at(1, 1), 0);
			Assert::AreEqual(m2.at(1, 1), 0);

			m2.at(1, 1) = 3;

			Assert::AreEqual(m.at(1, 1), 3);
			Assert::AreEqual(m2.at(1, 1), 3);
		}

		TEST_METHOD(RectangleMatrixTest)
		{
			Log::println(__FUNCTION__);

			auto m = HEVC::Matrix<int>(3, 5);

			m.at(1, 1) = 3;
			m.at(2, 4) = 8;

			Assert::AreEqual(m.at(1, 1), 3);
			Assert::AreEqual(m(2,4), 8);
			Log::printMatrix(m);
		}

		TEST_METHOD(IndexingPerformance)
		{
			Log::println(__FUNCTION__);

			auto m = HEVC::Matrix<int>(3, 3);

			auto start = std::chrono::system_clock::now();
			for (auto i = 0; i < 1000000; ++i)
			{
				m.at(1, 1) = i;
			}
			auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start);
			Log::println("at(): ", duration.count());


			start = std::chrono::system_clock::now();
			for (auto i = 0; i < 1000000; ++i)
			{
				m(1,1) = i;
			}
			duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start);
			Log::println("(): ", duration.count());
		}

		TEST_METHOD(IteratorTest)
		{
			auto m = HEVC::Matrix<int>(3, 3);

			for (auto i = 0; i < 3; ++i)
			{
				for (auto j = 0; j < 3; ++j)
				{
					m.at(j, i) = 3 * i + j;
				}
			}
			Log::printMatrix(m);
			Log::println("----------");

			for (auto i = m.begin(); i != m.end(); ++i)
			{
				Log::println(*i, " ");
			}
		}

		TEST_METHOD(RangeForLoopIteratorTest_Int)
		{
			auto m = HEVC::Matrix<int>(3, 3);

			for (auto i = 0; i < 3; ++i)
			{
				for (auto j = 0; j < 3; ++j)
				{
					m.at(j, i) = 3 * i + j;
				}
			}
			Log::printMatrix(m);
			Log::println("----------");

			for (auto i:m)
			{
				Log::println(i, " ");
			}
		}

		TEST_METHOD(RangeForLoopIteratorTest_Char)
		{
			auto m2 = HEVC::Matrix<char>(3, 3);

			for (auto i = 0; i < 3; ++i)
			{
				for (auto j = 0; j < 3; ++j)
				{
					m2.at(j, i) = 65 + (3 * i + j);
				}
			}
			Log::printMatrix(m2);
			Log::println("----------");

			for (auto i : m2)
			{
				Log::println(i, " ");
			}
		}

		TEST_METHOD(RangeForLoopIteratorTest_Bool)
		{
			auto m3 = HEVC::Matrix<bool>(3, 3);

			for (auto i = 0; i < 3; ++i)
			{
				for (auto j = 0; j < 3; ++j)
				{
					m3.at(j, i) = ((3 * i + j) % 2) == 0;
				}
			}
			Log::printMatrix(m3);
			Log::println("----------");

			for (auto i : m3)
			{
				Log::println(i, " ");
			}
		}

		TEST_METHOD(SubmatrixTest)
		{
			Log::println(__FUNCTION__);

			auto m = Matrix<int>(5, 5);

			for (auto i = 0; i < 5; ++i)
			{
				for (auto j = 0; j < 5; ++j)
				{
					m(j, i) = 5 * i + j;
				}
			}
			Log::printMatrix(m);
			Log::println("----------");

			auto m2 = m.submatrix(1, 1, 2, 2);

			Log::printMatrix(m2);
			Log::println("----------");

			auto m3 = m.submatrix(2, 0, 2, 4);

			for (auto i = 0u; i < m3.width(); ++i)
			{
				for (auto j = 0u; j < m3.height(); ++j)
				{
					m3(i, j) = -1;
				}
			}

			Log::printMatrix(m);
			Log::println("----------");

			for (auto& i:m3)
			{
				i = 1;
			}

			Log::printMatrix(m);
		}
	};
}