#pragma once

#include"settings.hpp"
#include"interface_linear_list.hpp"
#include"e_vector_iterator.hpp"

namespace eds {

constexpr unsigned INIT_SIZE = 4;
constexpr double EXPAND_RATE = 1.5;

template<typename T>
class vector_iterator;

template<typename T>
class vector : public intf_linear_list<T>{
public:
	vector(std::initializer_list<T> list);
	vector();

	//clear the data in the linear list
	void clear_list() noexcept;

	//if the list is not empty, return true
	bool is_not_empty() const noexcept;

	//return the length of the list
	unsigned list_length() const noexcept;

	//get the Lvalue reference of the linear list
	//make sure len > n >= 0
	T& get_item(unsigned n) const;

	//find the first element causing func(sample, tested) to return true, and return its serial number.
	//status == OK  :  founded
	//status == INFEASIBLE  :  not found
	std::tuple<unsigned, status> locate_item(const T& sample, std::function<bool(const T& sample, const T& tested)> func) const ;

	//return the former element of the sample
	//status == OK  :  founded
	//status == INFEASIBLE  :  not found
	std::tuple<T&, status> prior_item(const T& sample) const noexcept;

	//return the latter element of the sample
	//status == OK  :  founded
	//status == INFEASIBLE  :  not found
	std::tuple<T&, status> next_item(const T& sample) const noexcept;

	//insert the element to linear_list[n]
	//make sure len >= n >= 0
	void list_insert(T inserted, unsigned n);

	//delete the element linear_list[n]
	//make sure len > n >= 0
	void list_delete(unsigned n);

	//call func(element) on each element in the linear list
	void list_traverse(std::function<void(T& element)> func);

	//insert the element to the tail of the linear list
	void push_back(T inserted);

	//insert the element to the head of the linear list
	void insert_front(T inserted);

	//print the list to the console
	void print_linear_list(std::ostream& out) const noexcept;

	//get the iterator at the beginning of the linear list
	vector_iterator<T> begin() const noexcept;
	
	//get the iterator at the ending of the linear list
	vector_iterator<T> end() const noexcept;

	T& operator[](unsigned n) { return get_item(n); }

	friend std::ostream& operator<<(std::ostream& out, const vector<T>& sample) { sample.print_linear_list(out); return out; }

	//destructor
	~vector();
private:
	unsigned cap = 0;
	unsigned len = 0;
	T* head = nullptr;
	T& n_obj;

	vector(const vector&);

};
//ÉùÃ÷½áÊø

template<typename T>
inline vector<T>::vector():n_obj((T&)eds_m::null_class()) {
	head = new T[INIT_SIZE];
	if (head == nullptr) {
		throw std::runtime_error("from vector() : cannot alloc memory");
		return;
	}
	cap = INIT_SIZE;
	len = 0;
	return;
}

template<typename T>
inline vector<T>::vector(std::initializer_list<T> list):n_obj((T&)eds_m::null_class())
{
	len = list.size();
	cap = list.size();
	if (len == 0) {
		vector();
		return;
	}
	head = new T[list.size()];
	auto be = list.begin();
	auto en = list.end();
	for (unsigned i = 0; be != en;i++) {
		head[i] = *be;
		be++;
	}
	return;
}

template<typename T>
inline void vector<T>::clear_list() noexcept {
	this->len = 0;
	return;
}

template<typename T>
inline bool vector<T>::is_not_empty() const noexcept{
	if (len == 0) {
		return false;
	}
	else {
		return true;
	}
}

template<typename T>
inline unsigned vector<T>::list_length() const noexcept {
	return len;
}

template<typename T>
inline T& vector<T>::get_item(unsigned n) const{
	if (n > len) {
		throw std::out_of_range("From eds::vector::get_item : n > len");
	}
	if (n < len && n >= 0) {
		return head[n];
	}
	return n_obj;
}

template<typename T>
inline std::tuple<unsigned, status> vector<T>::locate_item(const T& sample, std::function<bool(const T&, const T&)> func) const {
	for (unsigned i = 0; i < len; i++) {
		if (func(sample, head[i])) {
			return std::tuple(i, OK);
		}
	}
	return std::tuple(0u, INFEASIBLE);
}

template<typename T>
inline std::tuple<T&, status> vector<T>::prior_item(const T& sample) const noexcept {
	if (len == 0) {
		return std::tuple<T&, status>(n_obj, INFEASIBLE);
	}
	for (unsigned i = 1; i < len; i++) {
		if (sample == (head)[i]) {
			return std::tuple<T&, status>(head[i - 1], OK);
		}
	}
	return std::tuple<T&, status>(n_obj, INFEASIBLE);
}

template<typename T>
inline std::tuple<T&, status> vector<T>::next_item(const T& sample) const noexcept{
	if (len == 0) {
		return std::tuple<T&, status>(n_obj, INFEASIBLE);
	}
	for (unsigned i = 0; i < len - 1; i++) {
		if (sample == head[i]) {
			return std::tuple<T&, status>(head[i + 1], OK);
		}
	}
	return std::tuple<T&, status>(n_obj, INFEASIBLE);
}

template<typename T>
inline void vector<T>::list_insert(T sample, unsigned n){
	if (n > len) {
		throw std::out_of_range("from vector::list_insert : n > len");
	}
	if (len == cap) {
		unsigned new_cap = static_cast<unsigned>(cap * EXPAND_RATE);
		if (new_cap < cap) {
			throw std::overflow_error("from vector::list_insert : multiple overflow");
		}
		T* new_head = new T[new_cap];
		handle_memory_alloc(new_head);
		for (unsigned i = len - 1; i >= n; i--) {
			new_head[i + 1] = head[i];
		}
		new_head[n] = sample;
		for (unsigned i = n - 1; i > 0; i--) {
			new_head[i] = head[i];
		}
		new_head[0] = head[0];
		delete[] head;
		head = new_head;
		cap = new_cap;
	}
	else {
		for (unsigned i = len; i > n; i--) {
			head[i] = head[i - 1];
		}
		head[n] = sample;
	}
	len++;
	return;
}

template<typename T>
inline void vector<T>::list_delete(unsigned n)
{
	if (n >= len || n < 0) {
		return;
	}
	T& copy = head[n];
	for (unsigned i = n; i < len - 1; i++) {
		head[i] = head[i + 1];
	}
	len--;
	return;
}

template<typename T>
inline void vector<T>::list_traverse(std::function<void(T& element)> func)
{
	for (unsigned i = 0; i < len; i++) {
		func(head[i]);
	}
	return;
}

template<typename T>
inline void vector<T>::push_back(T inserted)
{
	list_insert(inserted, len);
}

template<typename T>
inline void vector<T>::insert_front(T inserted)
{
	list_insert(inserted, 0);
}

template<typename T>
inline void vector<T>::print_linear_list(std::ostream& out) const noexcept
{
	for (unsigned i = 0; i < len; i++) {
		out << head[i] << " ";
	}
	out << std::endl;
}

template<typename T>
inline vector_iterator<T> vector<T>::begin() const noexcept {
	vector_iterator<T> ite(0u, this);
	return ite;
}

template<typename T>
inline vector_iterator<T> vector<T>::end() const noexcept {
	vector_iterator<T> ite(len, this);
	return ite;
}

template<typename T>
inline vector<T>::~vector()
{
	this->cap = 0;
	this->len = 0;
	delete[](this->head);
	return;
}


}