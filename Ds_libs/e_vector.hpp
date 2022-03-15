#pragma once

#include"settings.hpp"

namespace eds {

constexpr unsigned INIT_SIZE = 4;
constexpr double EXPAND_RATE = 1.5;

template<typename T>
class vector {
public:
	 //初始化vector
	 vector();
	 //清除线性表的数据（不清除分配的空间）
	 void clear_list() noexcept;
	 //返回list是否为空
	 bool list_empty() const noexcept;
	 //返回list长度
	 unsigned list_length() const noexcept;
	 //获取线性表的元素
	 T& get_item(unsigned) const;
	 //返回list中第一个与传入元素满足compare关系的元素的序号，调用时，func(传入的元素, list中元素)
	 std::tuple<unsigned, status> locate_item(const T&, std::function<bool(const T&, const T&)>) const noexcept;
	 //返回给定元素的前驱
	 std::tuple<T&, status> prior_item(const T&) const noexcept;
	 //返回给定元素的后继
	 std::tuple<T&, status> next_item(const T&) const noexcept;
	 //将元素插入到某位置上
	 void list_insert(T, unsigned);
	 //删除给定位置上的元素（错了，不该传引用）
	 std::tuple<T&, status> list_delete(unsigned);
	 //对list的每个对象调用传入的函数
	 void list_traverse(std::function<void(T&)>) noexcept;
	 //尾插入
	 void push_back(T);
	 //析构
	 ~vector();
private:
	unsigned cap = 0;
	unsigned len = 0;
	T* head = nullptr;
	T n_obj;

	vector(const vector&);

};
//声明结束

template<typename T>
vector<T>::vector() {
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
inline void vector<T>::clear_list() noexcept {
	this->len = 0;
	return;
}

template<typename T>
inline bool vector<T>::list_empty() const noexcept{
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
	if (n >= len) {
		throw std::out_of_range("from vector::get_item : n >= len");
	}
	if (n < len && n >= 0) {
		return head[n];
	}
}

template<typename T>
inline std::tuple<unsigned, status> vector<T>::locate_item(const T& sample, std::function<bool(const T&, const T&)> func) const noexcept{
	for (unsigned i = 0, bool ok = false; i < len; i++) {
		if (func(sample, head[i])) {
			return std::tuple(i, OK);
		}
	}
	return std::tuple(0u, INFEASIBLE);
}

template<typename T>
inline std::tuple<T&, status> vector<T>::prior_item(const T& sample) const noexcept {
	if (len == 0) {
		return std::tie(n_obj, INFEASIBLE);
	}
	if (sample == (head)[0]) {
		return std::tuple(n_obj, INFEASIBLE);
	}
	for (unsigned i = 1; i < len; i++) {
		if (sample == (head)[i]) {
			return std::tuple(head[i - 1], INFEASIBLE);
		}
	}
	return std::tuple(n_obj, INFEASIBLE);
}

template<typename T>
inline std::tuple<T&, status> vector<T>::next_item(const T& sample) const{
	if (len == 0) {
		return std::nullopt;
	}
	if (sample == head[this->len - 1]) {
		return std::nullopt;
	}
	for (unsigned i = 0; i < len - 1; i++) {
		if (sample == head[i]) {
			return head[i + 1];
		}
	}
	return std::nullopt;
}

template<typename T>
inline void vector<T>::list_insert(T sample, unsigned n){
	if (n >= len) {
		throw std::out_of_range("from vector::list_insert : n >= len");
	}
	if (len == cap) {
		unsigned new_cap = cap * EXPAND_RATE;
		if (new_cap < cap) {
			throw std::overflow_error("from vector::list_insert : multiple overflow");
		}
		T* new_head = new T[new_cap];
		for (unsigned i = len - 1; i >= n; i--) {
			new_head[i + 1] = head[i];
		}
		new_head[n] = sample;
		for (unsigned i = n - 1; i < 0; i--) {
			new_head[i] = head[i];
		}
	}
	else {
		for (unsigned i = len; i <= n; i--) {
			head[i] = head[i - 1];
		}
		head[n] = sample;
	}
	return;
}

template<typename T>
inline std::tuple<T&, status> vector<T>::list_delete(unsigned n)
{
	if (n >= len || n < 0) {
		return std::nullopt;
	}
	T& copy = head[n];
	for (unsigned i = n; i < len - 1; i++) {
		head[i] = head[i + 1];
	}
	len--;
	return copy;
}

template<typename T>
inline void vector<T>::list_traverse(std::function<bool(T&)> func)
{
	bool state;
	for (unsigned i = 0; i < len; i++) {
		state = func(head[i]);
		if (state != true) {
			throw std::runtime_error("from vector::list_traverse : function returns false");
		}
	}
	return;
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