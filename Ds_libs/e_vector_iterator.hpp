#pragma once

#include"settings.hpp"
#include"e_vector.hpp"
#include"interface_iterator.hpp"

namespace eds {
template<typename T>
class vector;

template<typename T>
class vector_iterator {
public:
	vector_iterator(unsigned current,const vector<T>* vec_address) :current(current), host(vec_address) {}

	//get the item the iterator is pointing
	T& get() const;

	//move backward
	void backward();

	//move forward
	void forward();

	//*
	T& operator*() { return get(); }

	//++
	vector_iterator<T> operator++(int) { vector_iterator<T> save = *this; forward(); return save; }

	//--
	vector_iterator<T> operator--(int) { vector_iterator<T> save = *this; backward(); return save; }

	//==
	friend bool operator==(const vector_iterator<T>& a, const vector_iterator<T>& b) {
			if (a.current == b.current && a.host == b.host) {
				return true;
			}
			return false;
		}

	//!=
	friend bool operator!=(const vector_iterator<T>& a, const vector_iterator<T>& b) {
			return !(a == b);
		}

private:
	unsigned current;
	const vector<T>* host;
};

	template<typename T>
	inline T& vector_iterator<T>::get() const
	{
		return (*host).get_item(current);
	}
	template<typename T>
	inline void vector_iterator<T>::backward()
	{
		if (current == 0) {
			throw std::runtime_error("From eds::vector_iterator : current = 0, current--");
		}
		current--;
		return;
	}
	template<typename T>
	inline void vector_iterator<T>::forward()
	{
		if (current > (*host).list_length()) {
			throw std::runtime_error("From eds::vector_iterator : n > length, n++");
		}
		current++;
		return;
	}

}