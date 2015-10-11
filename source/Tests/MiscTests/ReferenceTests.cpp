#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ParameterSetTests
{
	class ReferenceTarget
	{
	public:
		ReferenceTarget(const ReferenceTarget& other)
		{
			Log::println("Target cctor");
		}

		ReferenceTarget(ReferenceTarget&& other)
		{
			Log::println("Target mctor");
		}

		ReferenceTarget& operator=(const ReferenceTarget& other)
		{
			Log::println("Target operator=&");
			if (this == &other)
				return *this;
			return *this;
		}

		ReferenceTarget& operator=(ReferenceTarget&& other)
		{
			Log::println("Target operator=&&");
			if (this == &other)
				return *this;
			return *this;
		}

		ReferenceTarget()
		{
			Log::println("Target ctor");
		}
	};

	class ReferenceHolder
	{
		ReferenceTarget& ref;

	public:

		void action()
		{
			
		}

		explicit ReferenceHolder(ReferenceTarget& ref)
			: ref(ref)
		{
			Log::println("Holder ctor");
		}

		ReferenceHolder(const ReferenceHolder& other)
			: ref(other.ref)
		{
			Log::println("Holder cctor");
		}

		ReferenceHolder(ReferenceHolder&& other)
			: ref(other.ref)
		{
			Log::println("Holder mctor");
		}

		ReferenceHolder& operator=(const ReferenceHolder& other)
		{
			Log::println("Holder operator=&");
			if (this == &other)
				return *this;
			ref = other.ref;
			return *this;
		}

		ReferenceHolder& operator=(ReferenceHolder&& other)
		{
			Log::println("Holder operator=&&");
			if (this == &other)
				return *this;
			ref = other.ref;
			return *this;
		}
	};

	ReferenceHolder getRvalue()
	{
		ReferenceTarget target;
		return ReferenceHolder(target);
	}

	TEST_CLASS(ReferenceTests)
	{
	public:

		TEST_METHOD(Constructor)
		{
			Log::println(__FUNCTION__);
			ReferenceTarget target;
			ReferenceHolder holder(target);
			holder.action();
		}
		TEST_METHOD(CopyConstructor)
		{
			Log::println(__FUNCTION__);
			ReferenceTarget target1;
			ReferenceHolder holder(target1);
			ReferenceHolder holder2(holder);
			holder2.action();
		}
		TEST_METHOD(MoveConstructor)
		{
			Log::println(__FUNCTION__);
			ReferenceHolder holder(getRvalue());
			holder.action();
		}
		TEST_METHOD(Assignment)
		{
			Log::println(__FUNCTION__);
			ReferenceTarget target1;
			ReferenceHolder holder(target1);
			ReferenceHolder holder2 = holder;
			holder2.action();
		}
		TEST_METHOD(MoveAssignment)
		{
			Log::println(__FUNCTION__);
			ReferenceHolder holder = getRvalue();
			holder.action();
		}
	};
}
