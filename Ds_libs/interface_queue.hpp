#pragma once

#include"settings.hpp"

namespace eds {

template<typename T>
class intf_queue {
public:
	//clear the queue
	virtual void clear_queue() noexcept = 0;

	//if queue is empty, return false
	virtual bool queue_empty() const noexcept = 0;

	//return the length of the queue
	virtual unsigned queue_length() const noexcept = 0;

	//get the head element of the queue
	virtual T& get_head() const noexcept = 0;

	//object enter the queue
	virtual void en_queue(T object) = 0;

	//object get out of the queue
	virtual T de_queue() = 0;

	//call func(element) on each elements in the queue
	virtual void stack_traverse(std::function<void(T& element)> func) = 0;

	//destructor
	virtual ~intf_queue() {};
};
}