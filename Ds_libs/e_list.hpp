#pragma once

#include"settings.hpp"

namespace eds {

template<typename T>
struct l_node {
	T data;
	l_node<T>* next;
};

template<typename T>
class list {
public:
	//初始化list
	list(){}
	//清除线性表的数据
	void clear_list() noexcept;
	//返回list是否为空
	bool is_not_empty() const noexcept;
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
	//删除给定位置上的元素(回去学学右值引用）
	std::tuple<T, status> list_delete(unsigned);
	//对list的每个对象调用传入的函数
	void list_traverse(std::function<void(T&)>) noexcept;
	//尾插入
	void push_back(T);
	//析构
	~vector();
private:
	unsigned len = 0;
	l_node<T>* head = nullptr;
	T n_obj;

	list(const list&);
};
//声明结束





template<typename T>
inline bool list<T>::is_not_empty() const noexcept
{
	if (head == nullptr) {
		return false;
	}
	return true;
}

template<typename T>
inline unsigned list<T>::list_length() const noexcept
{
	return len;
}

template<typename T>
inline T& list<T>::get_item(unsigned n) const
{
	if (n >= len) {
		throw std::out_of_range("a");
	}
	l_node<T>* cur = head;
	for (unsigned i = 0; i < n; i++) {
		cur = cur->next;
	}
	return cur->data;
}

template<typename T>
inline std::tuple<unsigned, status> list<T>::locate_item(const T& sample, std::function<bool(const T&, const T&)> func) const noexcept
{
	bool flag = false;
	unsigned index = 0;
	for (l_node<T>* cur = head; cur != nullptr; cur = cur->next, index++) {
		flag = func(sample, cur->data);
		if (flag == true) {
			return std::tuple<unsigned, status>(index, OK);
		}
	}
	return std::tuple<unsigned, status>(0, INFEASIBLE);
}

template<typename T>
inline std::tuple<T&, status> list<T>::prior_item(const T&) const noexcept
{

	return std::tuple<T&, status>();
}

template<typename T>
inline void list<T>::list_insert(T sample, unsigned n)
{
	if (n > len) {
		throw std::out_of_range("a");
	}
	l_node<T>* cur = head;
	l_node<T>* new_node = new l_node<T>;
	if (new_node == nullptr) {
		throw std::runtime_error("a");
	}
	new_node->next = nullptr;
	new_node->data = sample;
	if (n == 0) {
		new_node->next = head;
		head = new_node;
	}
	else {
		for (unsigned i = 0; i < n - 1; i++) {
			cur = cur->next;
		}
		l_node<T>* holder = cur->next;
		new_node->next = holder;
		cur->next = new_node;
	}
	return;
}

template<typename T>
inline std::tuple<T, status> list<T>::list_delete(unsigned n)
{
	if (n >= len) {
		throw std::out_of_range("a");
	}
	l_node<T>* cur = head;
	l_node<T>* holder = nullptr;
	if (n == 0) {
		holder = head;
		head = head->next;
		T data = holder->data;
		free(holder);
		return std::tuple<T, status>(data, OK);
	}
	else {
		for (unsigned i = 0; i < n - 1; i++) {
			cur = cur->next;
		}
		holder = cur->next;
		cur->next = holder->next;
		T data = holder->data;
		free(holder);
		return std::tuple<T, status>(data, OK);
	}
	//uncomplete
}

template<typename T>
inline void list<T>::list_traverse(std::function<void(T&)> func) noexcept
{
	for (l_node<T>* cur = head; cur != nullptr; cur = cur->next) {
		func(cur->data);
	}
	return;
}

template<typename T>
inline void list<T>::push_back(T sample)
{
	list_insert(sample, len);
	return;
}

}