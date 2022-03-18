#pragma once

#include"settings.hpp"
#include"interface_iterator.hpp"

namespace eds {

template<typename T>
class intf_linear_list {
public:
	//clear the data in the linear list
	virtual void clear_list() noexcept = 0;

	//if the list is not empty, return true
	virtual bool is_not_empty() const noexcept = 0;

	//return the length of the list
	virtual unsigned list_length() const noexcept = 0;

	//get the Lvalue reference of the linear list
	//make sure len > n >= 0
	virtual T& get_item(unsigned n) const = 0;

	//find the first element causing func(sample, tested) to return true, and return its serial number.
	//status == OK  :  founded
	//status == INFEASIBLE  :  not found
	virtual std::tuple<unsigned, status> locate_item(const T& sample, std::function<bool(const T& sample, const T& tested)> func) const = 0;

	//return the former element of the sample
	//status == OK  :  founded
	//status == INFEASIBLE  :  not found
	virtual std::tuple<T&, status> prior_item(const T& sample) const noexcept = 0;

	//return the latter element of the sample
	//status == OK  :  founded
	//status == INFEASIBLE  :  not found
	virtual std::tuple<T&, status> next_item(const T& sample) const noexcept = 0;

	//insert the element to linear_list[n]
	//make sure len >= n >= 0
	virtual void list_insert(T inserted, unsigned n) = 0;

	//delete the element linear_list[n]
	//make sure len > n >= 0
	virtual void list_delete(unsigned n) = 0;

	//call func(element) on each element in the linear list
	virtual void list_traverse(std::function<void(T& element)> func) = 0;

	//insert the element to the tail of the linear list
	virtual void push_back(T inserted) = 0;

	//insert the element to the head of the linear list
	virtual void insert_front(T inserted) = 0;

	//print the list to the console
	virtual void print_linear_list(std::ostream& out) const noexcept = 0;

	//get the iterator at the beginning of the linear list
	//get the iterator at the ending of the linear list

	virtual T& operator[](unsigned n) = 0;

	//destructor
	virtual ~intf_linear_list() {};
};

}