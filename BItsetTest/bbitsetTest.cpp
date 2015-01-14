#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BitsetTest
{
	TEST_CLASS(BitsetTest)
	{
	public:

		TEST_METHOD(IndexTest)
		{
			boost::dynamic_bitset<> set;
			set.push_back(false);
			set.push_back(true);
			bool b0 = set[0];
			bool b1 = set[1];
			std::string s1,s2;
			
			to_string_helper(set, s1, true);
			to_string_helper(set, s2, false);
			std::string s3 = set.toString();
		}
	};
}