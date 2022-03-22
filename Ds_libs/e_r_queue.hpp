#pragma once

#include"settings.hpp"

namespace eds {

	template<typename T>
	class r_queue {
	public:
		r_queue(unsigned size) noexcept {
			queue_base = new T[size];
			handle_memory_alloc(queue_base);
			cap = size;
		};
		r_queue(unsigned size, std::initializer_list<T> list) {
			queue_base = new T[size];
			handle_memory_alloc(queue_base);
			assert(size <= list.size());
			cap = size;
			for (auto& data : list) {
				assert()
			}
		};
		r_queue(const r_queue& sample);
		r_queue(r_queue&& sample);

		//clear the queue
		void clear_queue() noexcept;

		//if queue is not empty, return true
		bool is_not_empty() const noexcept;

		//return the length of the queue
		unsigned queue_length() const noexcept;

		//get the head element of the queue
		T& get_head() const noexcept;

		//object enter the queue
		void en_queue(T object);

		//object get out of the queue
		T de_queue();

		//call func(element) on each elements in the queue
		void queue_traverse(std::function<void(T& element)> func);

	private:
		T* queue_base = nullptr;
		unsigned head = 0;
		unsigned tail = 0;
		unsigned len = 0;
		unsigned cap = 0;

		auto add_handle = [cap](unsigned& num) {
			if (num == cap - 1) {
				num = 0;
			}
			else {
				num++;
			}
		}
	};
	template<typename T>
	inline void r_queue<T>::clear_queue() noexcept
	{
		length = 0;
		l_node<T>* cur = head;
		for (; head != tail;) {
			head = head->next;
			delete cur;
			cur = head;
		}
		delete tail;
		head = nullptr;
		tail = nullptr;
	}

	template<typename T>
	inline bool r_queue<T>::is_not_empty() const noexcept
	{
		if (length == 0) {
			return false;
		}
		return true;
	}

	template<typename T>
	inline unsigned r_queue<T>::queue_length() const noexcept
	{
		return length;
	}

}