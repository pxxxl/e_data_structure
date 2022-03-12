#pragma once

#include<tuple>
#include<functional>

namespace eds {
	using status = int;

	constexpr int TRUE = 1;
	constexpr int FALSE = 0;
	constexpr int OK = 1;
	constexpr int ERROR = 0;
	constexpr int INFEASIBLE = -1;
	constexpr int OVERFLOWED = -2;
}


