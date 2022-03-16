#pragma once

#include<tuple>
#include<functional>
#include<optional>
#include<assert.h>
#include<stdexcept>

namespace eds_m {
	constexpr int TRUE = 1;
	constexpr int FALSE = 0;

	constexpr int OK = 1;
	constexpr int INFEASIBLE = -1;
	constexpr int FATAL = -2;

	class null_class{};
}

namespace eds {

	using status = int;

	inline void handle_memory_alloc(void* point_to_allocated_memory) {
		assert(!(point_to_allocated_memory == nullptr));
	}

}


