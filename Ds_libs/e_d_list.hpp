#pragma once

#include"settings.hpp"
#include"interface_linear_list.hpp"


namespace eds {
template<typename T>
struct d_node {
	T data;
	d_node<T>* next;
	d_node<T>* prior;
};

template<typename T>
class d_list : public intf_linear_list<T> {
public:
	d_list();
	d_list(std::initializer_list<T> list);

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
	std::tuple<unsigned, status> locate_item(const T& sample, std::function<bool(const T& sample, const T& tested)> func) const;

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
	//d_list_iterator<T> begin() const noexcept;

	//get the iterator at the ending of the linear list
	//d_list_iterator<T> end() const noexcept;

	friend std::ostream& operator<<(std::ostream& out, const d_list<T>& sample) { sample.print_linear_list(out); return out; }

	T& operator[](unsigned n) { return get_item(n); }

	//friend class d_list_iterator<T>;

	//destructor
	~d_list();
private:
	unsigned len = 0;
	d_node<T>* head = nullptr;
	d_node<T>* tail = nullptr;
	T& n_obj;

	d_list(const d_list&);
};

template<typename T>
inline d_list<T>::d_list() :n_obj((T&)eds_m::null_class()) {}

template<typename T>
inline d_list<T>::d_list(std::initializer_list<T> list) : n_obj((T&)eds_m::null_class())
{
	len = list.size();
	if (len == 0) {
		d_list();
		return;
	}
	head = new d_node<T>;
	handle_memory_alloc(head);
	auto be = list.begin();
	auto en = list.end();
	auto cur = head;
	cur->data = *be;
	be++;
	for (; be != en;) {
		cur->next = new d_node<T>;
		handle_memory_alloc(cur->next);
		cur->next->data = *be;
		cur->next->prior = cur;
		cur = cur->next;
		be++;
	}
	tail = cur;
	head->prior = nullptr;
	tail->next = nullptr;
	return;
}

template<typename T>
inline void d_list<T>::clear_list() noexcept
{
	while (head != tail) {
		tail = tail->prior;
		free(tail->next);
	}
	if (head != nullptr) {
		free(head);
	}
	head = nullptr;
	tail = nullptr;
	len = 0;
}

template<typename T>
inline bool d_list<T>::is_not_empty() const noexcept
{
	if (len == 0) {
		return false;
	}
	return true;
}

template<typename T>
inline unsigned d_list<T>::list_length() const noexcept
{
	return len;
}

template<typename T>
inline T& d_list<T>::get_item(unsigned n) const
{
	if (n >= len) {
		throw std::runtime_error("From eds::d_list::get_item : n >= len");
	}

	if (n < len / 2) {
		//从头遍历
		auto cur = head;
		for (unsigned i = 0; i < n; i++) {
			cur = cur->next;
		}
		return cur->data;
	}
	else {
		//从尾遍历
		auto cur = tail;
		for (unsigned i = len - 1; i < n; i++) {
			cur = cur->prior;
		}
		return cur->data;
	}
}

template<typename T>
inline std::tuple<unsigned, status> d_list<T>::locate_item(const T& sample, std::function<bool(const T& sample, const T& tested)> func) const
{
	auto cur = head;
	for (unsigned i = 0; cur != nullptr; i++) {
		if (func(sample, cur->data)) {
			return std::tuple<unsigned, status>(i, INFEASIBLE);
		}
	}
	return std::tuple<unsigned, status>(0, INFEASIBLE);
}

template<typename T>
inline std::tuple<T&, status> d_list<T>::prior_item(const T& sample) const noexcept
{
	if (len == 0) {
		return std::tuple<T&, status>(n_obj, INFEASIBLE);
	}
	auto cur = head ->next;
	for (; cur != nullptr;cur = cur->next) {
		if (sample == cur->data) {
			return std::tuple<T&, status>(cur->prior->data, OK);
		}
	}
	return std::tuple<T&, status>(n_obj, INFEASIBLE);
}

template<typename T>
inline std::tuple<T&, status> d_list<T>::next_item(const T& sample) const noexcept
{
	if (len == 0) {
		return std::tuple<T&, status>(n_obj, INFEASIBLE);
	}
	auto cur = tail->prior;
	for (; cur != nullptr; cur = cur->prior) {
		if (sample == cur->data) {
			return std::tuple<T&, status>(cur->next->data, OK);
		}
	}
	return std::tuple<T&, status>(n_obj, INFEASIBLE);
}

template<typename T>
inline void d_list<T>::list_insert(T inserted, unsigned n)
{
	if (n > len) {
		throw std::runtime_error("From eds::d_list::list_insert : n > len");
	}
	if (n <= len / 2) {
		//从头开始
		if (n == 0) {
			if (len == 0) {
				head = new d_node<T>;
				handle_memory_alloc(head);
				head->next = nullptr;
				head->prior = nullptr;
				tail = head;
				head->data = inserted;
			}
			head->prior = new d_node<T>;
			handle_memory_alloc(head->prior);
			head->prior->next = head;
			head->prior->data = inserted;
			head->prior->prior = nullptr;
			head = head->prior;
		}
		else {
			auto cur = head;
			for (unsigned i = 1; i < n; i++) {
				cur = cur->next;
			}
			auto cur_next = cur->next;
			cur->next = new d_node<T>;
			handle_memory_alloc(cur->next);
			cur->next->next = cur_next;
			cur->next->data = inserted;
			cur->next->prior = cur;
			cur_next->prior = cur->next;
		}
	}
	else {
		//从尾开始
		if (n == len) {
			tail->next = new d_node<T>;
			handle_memory_alloc(tail->next);
			tail->next->next = nullptr;
			tail->next->data = inserted;
			tail->next->prior = tail;
			tail = tail->next;
		}
		else {
			auto cur = tail;
			for (unsigned i = len - 1; i > n; i--) {
				cur = cur->prior;
			}
			auto cur_prior = cur->prior;
			cur->prior = new d_node<T>;
			handle_memory_alloc(cur->prior);
			cur->prior->prior = cur_prior;
			cur->prior->data = inserted;
			cur->prior->next = cur;
			cur_prior->next = cur->prior;
		}
	}
	len++;
	return;
}

template<typename T>
inline void d_list<T>::list_delete(unsigned n)
{
	if (n >= len) {
		throw std::runtime_error("From eds::d_list::list_delete : n >= len");
	}
	if (n == 0) {
		if (len == 1) {
			free(head);
			head = nullptr;
			tail = nullptr;
			return;
		}
		head = head->next;
		free(head->prior);
		len--;
		head->prior = nullptr;
	}
	else if (n == len - 1) {
		tail = tail->prior;
		free(tail->next);
		len--;
		tail->next = nullptr;
	}
	else if (n < len / 2) {
		auto cur = head;
		for (unsigned i = 1; i < n; i++) {
			cur = cur->next;
		}
		cur->next = cur->next->next;
		free(cur->next->prior);
		cur->next->prior = cur;
	}
	else {
		auto cur = tail;
		for (unsigned i = len - 1; i > n; i--) {
			cur = cur->prior;
		}
		cur->prior = cur->prior->prior;
		free(cur->prior->next);
		cur->prior->next = cur;
	}
	len--;
}

template<typename T>
inline void d_list<T>::list_traverse(std::function<void(T& element)> func)
{
	auto cur = head;
	for (; cur != nullptr;cur = cur->next) {
		func(cur->data);
	}
	return;
}

template<typename T>
inline void d_list<T>::push_back(T inserted)
{
	list_insert(inserted, len);
}

template<typename T>
inline void d_list<T>::insert_front(T inserted)
{
	list_insert(inserted, 0);
}

template<typename T>
inline void d_list<T>::print_linear_list(std::ostream& out) const noexcept
{
	d_node<T>* cur = head;
	for (; cur != nullptr; cur = cur->next) {
		std::cout << cur->data << " ";
	}
	return;
}

//template<typename T>
//inline d_list_iterator<T> d_list<T>::begin() const noexcept
//{
//	d_list_iterator<T> ite(0u, this);
//	return ite;
//}

//template<typename T>
//inline d_list_iterator<T> d_list<T>::end() const noexcept
//{
//	d_list_iterator<T> ite(len, this);
//	return ite;
//}

template<typename T>
inline d_list<T>::~d_list()
{
	clear_list();
}

}