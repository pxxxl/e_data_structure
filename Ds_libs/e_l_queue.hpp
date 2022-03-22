#pragma once

#include"settings.hpp"

namespace eds {

	template<typename T>
	class l_queue {
	public:
		l_queue() noexcept{};
		l_queue(std::initializer_list<T> list) {
			l_node<T>* cur = head;
			unsigned serial = 0;
			for (auto data : list) {
				if (serial == 0) {
					head = new l_node<T>;
					handle_memory_alloc(head);
					head->data = data;
					serial++;
					length++;
				}
				else {
					cur->next = new l_node<T>;
					handle_memory_alloc(cur->next);
					cur->next->data = data;
					cur->next->next = nullptr;
					cur = cur->next;
					length++;
				}
			}
			tail = cur;
			return;
		};
		l_queue(const l_queue& sample);
		l_queue(l_queue&& sample);

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
		l_node<T>* head = nullptr;
		l_node<T>* tail = nullptr;
		unsigned length = 0;
	};
	template<typename T>
	inline void l_queue<T>::clear_queue() noexcept
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
	inline bool l_queue<T>::is_not_empty() const noexcept
	{
		if (length == 0) {
			return false;
		}
		return true;
	}

	template<typename T>
	inline unsigned l_queue<T>::queue_length() const noexcept
	{
		return length;
	}

}