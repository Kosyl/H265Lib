#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

interface ITestInterface
{
public:
	virtual int testMethod() = 0;
};

class TestImpl : public ITestInterface
{
public:
	int testMethod() override
	{
		return 42;
	}
	virtual ~TestImpl(){}
};

class Base
{
public:
	virtual ~Base(){}
	virtual std::string foo()
	{
		return "base";
	}
};

class Derived: public Base
{
public:
	virtual ~Derived(){}
	virtual std::string foo() override
	{
		return"derived";
	}
};

namespace ParameterSetTests
{
	TEST_CLASS(InterfaceTests)
	{
	public:

		TEST_METHOD(BasicInterfaceLogTest)
		{
			std::shared_ptr<ITestInterface> ptr = std::make_unique<TestImpl>();
			Log::println( "42: ", ptr->testMethod());
		}

		TEST_METHOD(DerivedByValueTest)
		{
			Base b;
			Derived d;

			Base b2 = Derived();

			Log::println("base: ", b.foo());
			Log::println("derived: ", d.foo());
			Log::println("derived by value: ", b2.foo());
		}
	};
}
