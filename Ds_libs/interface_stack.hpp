#pragma once

#include"settings.hpp"

namespace eds {

template<typename T>
class stack {
public:
	//clear the stack
	virtual void clear_stack() noexcept = 0;

	//if stack is empty, return false
	virtual bool stack_empty() const noexcept = 0;

	//return the length of the stack
	virtual unsigned stack_length() const noexcept = 0;

	//get the top element of the stack
	virtual T& get_top() const noexcept = 0;

	//push the object into the stack
	virtual void push(T object) = 0;

	//pop the object out of the stack
	virtual T pop() = 0;

	//call func(element) on each elements in the stack
	virtual void stack_traverse(std::function<void(T& element)> func) = 0;

	//destructor
	virtual ~stack() {};
};

}

/*
void clear_stack() noexcept;
bool stack_empty() const noexcept;
unsigned stack_length() const noexcept;
const T& get_top() const noexcept;
void push(T) = 0;
T pop() noexcept = 0;
 void stack_traverse(std::function<void(T& element)> func);
~stack() = 0;
*/
