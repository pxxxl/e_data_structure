#include "pch.h"
#include "CppUnitTest.h"
#include "e_vector.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			eds::status errcode = eds::OK;
			eds::vector<int> vec(errcode);
			vec.list_insert(1, 0);
			auto[num, errcode] = vec.get_item(0);
			
		}
	};
}
