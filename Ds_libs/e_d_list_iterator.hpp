#pragma once

#include"settings.hpp"
#include"e_d_list.hpp"
#include"interface_iterator.hpp"

namespace eds {
	template<typename T>
	class d_list;

	template<typename T>
	class d_list_iterator {
	public:
		d_list_iterator(unsigned current, const d_list<T>* vec_address) :host(vec_address) {
			unsigned len = vec_address->list_length();
			assert(current <= len);
			if (current < len / 2) {
				//从头遍历
				auto cur = vec_address->head;
				for (unsigned i = 0; i < current; i++) {
					cur = cur->next;
				}
				cur_node = cur;
			}
			else {
				//从尾遍历
				auto cur = vec_address->tail;
				for (unsigned i = len - 1; i < current; i++) {
					cur = cur->prior;
				}
				cur_node = cur;
			}
		}

		//get the item the iterator is pointing
		T& get() const;

		//move backward
		void backward();

		//move forward
		void forward();

		//*
		T& operator*() { return get(); }

		//++
		d_list_iterator<T> operator++(int) { d_list_iterator<T> save = *this; forward(); return save; }

		//--
		d_list_iterator<T> operator--(int) { d_list_iterator<T> save = *this; backward(); return save; }

		//==
		friend bool operator==(const d_list_iterator<T>& a, const d_list_iterator<T>& b) {
			if (a.cur_node == b.cur_node && a.host == b.host) {
				return true;
			}
			return false;
		}

		//!=
		friend bool operator!=(const d_list_iterator<T>& a, const d_list_iterator<T>& b) {
			return !(a == b);
		}

	private:
		d_node<T>* cur_node;
		const d_list<T>* host;
	};

	template<typename T>
	inline T& d_list_iterator<T>::get() const
	{
		return cur_node->data;
	}
	template<typename T>
	inline void d_list_iterator<T>::backward()
	{
		if (cur_node->prior == nullptr) {
			throw std::runtime_error("From eds::vector_iterator : cur_node = 0, cur_node--");
		}
		cur_node = cur_node->prior;
		return;
	}
	template<typename T>
	inline void d_list_iterator<T>::forward()
	{
		if (cur_node == nullptr) {
			throw std::runtime_error("From eds::vector_iterator : n > length, n++");
		}
		cur_node = cur_node->next;
		return;
	}

}