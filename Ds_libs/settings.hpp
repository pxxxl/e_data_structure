#pragma once

#include<tuple>
#include<functional>
#include<optional>
#include<assert.h>
#include<stdexcept>

namespace eds {

	using status = int;

	constexpr int TRUE = 1;
	constexpr int FALSE = 0;

	constexpr int OK = 1;
	constexpr int INFEASIBLE = -1;
	constexpr int FATAL = -2;


}


