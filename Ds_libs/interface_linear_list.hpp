#pragma once

#include"settings.hpp"

namespace eds {

template<typename T>
class linear_list {
public:
	//clear the data in the linear list
	virtual void clear_list() noexcept = 0;

	//if the list is empty, return false
	virtual bool list_empty() const noexcept = 0;

	//return the length of the list
	virtual unsigned list_length() const noexcept = 0;

	//get the Lvalue reference of the linear list
	//make sure len > n >= 0
	virtual T& get_item(unsigned n) const = 0;

	//find the first element causing func(sample, tested) to return true, and return its serial number.
	//status == eds_m::OK  :  founded
	//status == eds_m::INFEASIBLE  :  not found
	virtual std::tuple<unsigned, status> locate_item(const T& sample, std::function<bool(const T& sample, const T& tested)> func) const = 0;

	//return the former element of the sample
	//status == eds_m::OK  :  founded
	//status == eds_m::INFEASIBLE  :  not found
	virtual std::tuple<T&, status> prior_item(const T& sample) const noexcept = 0;

	//return the latter element of the sample
	//status == eds_m::OK  :  founded
	//status == eds_m::INFEASIBLE  :  not found
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

	//destructor
	virtual ~linear_list() {};
};

}

/*
void clear_list() noexcept;
bool list_empty() const noexcept;
unsigned list_length() const noexcept;
T& get_item(unsigned n) const = 0;
std::tuple<unsigned, status> locate_item(const T& sample, std::function<bool(const T& sample, const T& tested)> func) const noexcept;
std::tuple<T&, status> prior_item(const T& sample) const noexcept;
std::tuple<T&, status> next_item(const T& sample) const noexcept;
void list_insert(T inserted, unsigned n);
void list_delete(unsigned n);
void list_traverse(std::function<void(T& element)> func) noexcept;
void push_back(T inserted);
void insert_front(T inserted);
~linear_list() {};
*/