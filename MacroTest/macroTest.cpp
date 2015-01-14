#include "stdafx.h"
#include "CppUnitTest.h"
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>

#define		FORWARD_TRANSFORM_LOGGING_ON 1
#define		FORWARD_TRANSFORM_PRINT_VERTICAL 		FORWARD_TRANSFORM_LOGGING_ON &&	1
#define		FORWARD_TRANSFORM_INCLUDE_DETAILS 		FORWARD_TRANSFORM_LOGGING_ON &&	0

#if FORWARD_TRANSFORM_INCLUDE_DETAILS
#define		LOG_FT_DETAILS(x) x
#else
#define		LOG_FT_DETAILS(x)
#endif

#if FORWARD_TRANSFORM_PRINT_VERTICAL
#define		LOG_FT_VERT(x) x
#else
#define		LOG_FT_VERT(x)
#endif

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace DefinitionTest
{
	TEST_CLASS(DefinitionTest)
	{
	public:
		TEST_METHOD(DefTest)
		{
			int x = 0;

			LOG_FT_DETAILS(++x);
			Assert::IsTrue(x == 0);

			LOG_FT_VERT(++x);

			Assert::IsTrue(x == 1);
		}
	};
}