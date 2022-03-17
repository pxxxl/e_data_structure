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
	//��ʼ��list
	list(){}
	//������Ա������
	void clear_list() noexcept;
	//����list�Ƿ�Ϊ��
	bool is_not_empty() const noexcept;
	//����list����
	unsigned list_length() const noexcept;
	//��ȡ���Ա��Ԫ��
	T& get_item(unsigned) const;
	//����list�е�һ���봫��Ԫ������compare��ϵ��Ԫ�ص���ţ�����ʱ��func(�����Ԫ��, list��Ԫ��)
	std::tuple<unsigned, status> locate_item(const T&, std::function<bool(const T&, const T&)>) const noexcept;
	//���ظ���Ԫ�ص�ǰ��
	std::tuple<T&, status> prior_item(const T&) const noexcept;
	//���ظ���Ԫ�صĺ��
	std::tuple<T&, status> next_item(const T&) const noexcept;
	//��Ԫ�ز��뵽ĳλ����
	void list_insert(T, unsigned);
	//ɾ������λ���ϵ�Ԫ��(��ȥѧѧ��ֵ���ã�
	std::tuple<T, status> list_delete(unsigned);
	//��list��ÿ��������ô���ĺ���
	void list_traverse(std::function<void(T&)>) noexcept;
	//β����
	void push_back(T);
	//����
	~vector();
private:
	unsigned len = 0;
	l_node<T>* head = nullptr;
	T n_obj;

	list(const list&);
};
//��������





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