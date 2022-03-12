#pragma once

#include<tuple>
#include<functional>

namespace eds {
	using status = int;

	constexpr int TRUE = 1;
	constexpr int FALSE = 0;
	constexpr int OK = 1;
	//宽泛的错误类型
	constexpr int ERROR = 0;
	//无法完成操作
	constexpr int INFEASIBLE = -1;
	//空间不足
	constexpr int OVERFLOWED = -2;
}


