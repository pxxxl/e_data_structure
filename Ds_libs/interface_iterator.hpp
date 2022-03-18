#pragma once

#include"settings.hpp"

namespace eds {

template<typename T>
class intf_iterator {
public:
	//get the item the iterator is pointing
	virtual T& get() const = 0;

	//move backward
	virtual T& backward() = 0;

	//move forward
	virtual T& forward() = 0;

	//*
	virtual T& operator*() { return get(); }

	//++
	virtual intf_iterator<T>& operator++() { forward(); return *this; }

	//--
	virtual intf_iterator<T>& operator--() { backward(); return *this; }
};
}
