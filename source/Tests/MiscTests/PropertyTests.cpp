#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

class TestImpl
{
private:

	int i;

public:
	void putprop(int j) {
		i = j;
	}

	int getprop() {
		return i;
	}

	__declspec(property(get = getprop, put = putprop)) int the_prop;
};

namespace ParameterSetTests
{
	TEST_CLASS(PropertyTests)
	{
	public:

		TEST_METHOD(BasicPropertyTest)
		{
			TestImpl t;
			t.the_prop = 4;
			Log::println( t.the_prop );
		}
	};
}
