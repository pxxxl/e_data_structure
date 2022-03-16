#pragma once

#include"settings.hpp"
#include"interface_stack.hpp"

namespace eds {

template<typename T>
struct stack_node {
	T data;
	stack_node* under;
};

template<typename T>
class l_stack : public stack<T> {
public:
	l_stack(std::initializer_list<T> list);
	l_stack() = default;

	//clear the stack
	void clear_stack() noexcept;

	//if stack is empty, return false
	bool stack_empty() const noexcept;

	//return the length of the stack
	unsigned stack_length() const noexcept;

	//get the top element of the stack
	T& get_top() const noexcept;

	//push the object into the stack
	void push(T object);

	//pop the object out of the stack
	T pop();

	//call func(element) on each elements in the stack
	void stack_traverse(std::function<void(T& element)> func);

	//destructor
	~l_stack();

private:
	stack_node<T>* top_cur = nullptr;
	unsigned size = 0;
};

template<typename T>
inline l_stack<T>::l_stack(std::initializer_list<T> list)
{
	auto be = list.begin();
	auto en = list.end();
	auto holder = top_cur;
	for (; be != en; be++) {
		top_cur = new stack_node<T>;
		handle_memory_alloc(top_cur);
		top_cur->data = *be;
		top_cur->under = holder;
		holder = top_cur;
	}
	size = list.size();
	return;
}

template<typename T>
inline void l_stack<T>::clear_stack() noexcept
{
	auto prior = top_cur;
	for (; prior != nullptr;) {
		prior = prior->under;
		free(top_cur);
		top_cur = prior;
	}
	size = 0;
	return;
}

template<typename T>
inline bool l_stack<T>::stack_empty() const noexcept
{
	if (top_cur == nullptr) {
		return false;
	}
	return true;
}

template<typename T>
inline unsigned l_stack<T>::stack_length() const noexcept
{
	return size;
}

template<typename T>
inline T& l_stack<T>::get_top() const noexcept
{
	return top_cur->data;
}

template<typename T>
inline void l_stack<T>::push(T object)
{
	auto holder = top_cur;
	top_cur = new stack_node<T>;
	handle_memory_alloc(top_cur);
	top_cur->data = object;
	top_cur->under = holder;
	size++;
	return;
}

template<typename T>
inline T l_stack<T>::pop()
{
	if (size == 0) {
		throw std::runtime_error("From l_stack::pop : stack empty");
	}
	auto holder = top_cur;
	top_cur = top_cur->under;
	size--;
	T ref = holder->data;
	free(holder);
	return ref;
}

template<typename T>
inline void l_stack<T>::stack_traverse(std::function<void(T& element)> func)
{
	auto traveller = top_cur;
	for (; traveller != nullptr;) {
		func(traveller->data);
		traveller = traveller->under;
	}
	return;
}

template<typename T>
inline l_stack<T>::~l_stack()
{
	clear_stack();
}

}