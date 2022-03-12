#pragma once

#include"settings.hpp"

/*
 * ������
 * ����statusֵ������������õȴ���
 * �������к������������᷵��statusֵ���෵��ֵʹ��tuple������statusֵ�����һ������ֵ��
 * ���ⲻ�������ֻ����statusֵ���������ʹ�ñ����Դ��� handle(status) ���鷵�ص�status��
*/

namespace eds {

constexpr unsigned INIT_SIZE = 4;
constexpr double EXPAND_RATE = 1.5;

template<typename T>
class vector;

template<typename T>
class vector {
public:
	 //��ʼ��vector
	 vector(status&);
	 //�������Ա����ú�Ӧ�ٶ�vector��ֵ���κμ��裬�뽫�䶪��
	 status destroy_list();
	 //������Ա�����ݣ����������Ŀռ䣩
	 status clear_list();
	 //����list�Ƿ�Ϊ��
	 std::tuple<bool, status> list_empty();
	 //����list����
	 std::tuple<unsigned, status> list_length();
	 //��ȡ���Ա��Ԫ��
	 std::tuple<T&, status> get_item(unsigned);
	 //����list�е�һ���봫��Ԫ������compare��ϵ��Ԫ�ص���ţ�����ʱ��func(�����Ԫ��, list��Ԫ��)
	 std::tuple<unsigned, status> locate_item(const T&, std::function<bool(const T&, const T&)>);
	 //���ظ���Ԫ�ص�ǰ��
	 std::tuple<T&, status> prior_item(const T&);
	 //���ظ���Ԫ�صĺ��
	 std::tuple<T&, status> next_item(const T&);
	 //��Ԫ�ز��뵽ĳλ���ϣ��������죩
	 status list_insert(T, unsigned);
	 //ɾ������λ���ϵ�Ԫ��
	 std::tuple<T&, status> list_delete(unsigned);
	 //��list��ÿ��������ô���ĺ���
	 status list_traverse(std::function<bool(T&)>);
private:
	unsigned cap = 0;
	unsigned len = 0;
	T* head = nullptr;

	vector();
	vector(const vector&);

	int nul_reference = 0;
};
//��������

template<typename T>
inline vector<T>::vector(status& sta) {
	head = new T[INIT_SIZE];
	if (head == nullptr) {
		sta = OVERFLOWED;
		return;
	}
	cap = INIT_SIZE;
	len = 0;
	sta = OK;
	return;
}

template<typename T>
inline status vector<T>::destroy_list() {
	this->cap = 0;
	this->len = 0;
	delete[](this->head);
	return OK;
}

template<typename T>
inline status vector<T>::clear_list() {
	this->len = 0;
	return OK;
}

template<typename T>
inline std::tuple<bool, status> vector<T>::list_empty()
{
	if (len == 0) {
		return std::tuple< bool, status>(false, OK);
	}
	else {
		return std::tuple< bool, status>(true, OK);
	}
}

template<typename T>
inline std::tuple<unsigned, status> vector<T>::list_length()
{
	return std::tuple<unsigned, status>(len, OK);
}

template<typename T>
inline std::tuple<T&, status> vector<T>::get_item(unsigned n){
	if (n < len && n >= 0) {
		return std::tuple<T&, status>(head[n], OK);
	}
	else {
		return std::tuple<T&, status>(static_cast<T&>(nul_reference), INFEASIBLE);
	}
}

template<typename T>
inline std::tuple<unsigned, status> vector<T>::locate_item(const T& sample, std::function<bool(const T&, const T&)> func){
	for (unsigned i = 0, bool ok = false; i < len; i++) {
		if (func(sample, head[i])) {
			return std::tuple<unsigned, status>(i, OK);
		}
	}
	return std::tuple<unsigned, status>(0, INFEASIBLE);
}

template<typename T>
inline std::tuple<T&, status> vector<T>::prior_item(const T& sample){
	if (len == 0) {
		return std::tuple<T&, status>(static_cast<T&>(nul_reference), INFEASIBLE);
	}
	if (sample == (head)[0]) {
		return std::tuple<T&, status>(static_cast<T&>(nul_reference), INFEASIBLE);
	}
	for (unsigned i = 1; i < len; i++) {
		if (sample == (head)[i]) {
			return std::tuple<T&, status>((head)[i-1], OK);
		}
	}
	return std::tuple<T&, status>(static_cast<T&>(nul_reference), INFEASIBLE);
}

template<typename T>
inline std::tuple<T&, status> vector<T>::next_item(const T& sample)
{
	if (len == 0) {
		return std::tuple<T&, status>(static_cast<T&>(nul_reference), INFEASIBLE);
	}
	if (sample == head[this->len - 1]) {
		return std::tuple<T&, status>(static_cast<T&>(nul_reference), INFEASIBLE);
	}
	for (unsigned i = 0; i < len - 1; i++) {
		if (sample == head[i]) {
			return std::tuple<T&, status>((this->head)[i + 1], OK);
		}
	}
	return std::tuple<T&, status>(static_cast<T&>(nul_reference), INFEASIBLE);
}

template<typename T>
inline status vector<T>::list_insert(T sample, unsigned n){
	if (n >= len || n < 0) {
		return INFEASIBLE;
	}
	if (len == cap) {
		unsigned new_cap = cap * EXPAND_RATE;
		if (new_cap < cap) {
			return OVERFLOWED;
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
	return OK;
}

template<typename T>
inline std::tuple<T&, status> vector<T>::list_delete(unsigned n)
{
	if (n >= len || n < 0) {
		return std::tuple<T&, status>(static_cast<T&>(nul_reference), INFEASIBLE);
	}
	T& copy = head[n];
	for (unsigned i = n; i < len - 1; i++) {
		head[i] = head[i + 1];
	}
	len--;
	return std::tuple<T&, status>(copy, OK);
}

template<typename T>
inline status vector<T>::list_traverse(std::function<bool(T&)> func)
{
	bool sta;
	for (unsigned i = 0; i < len; i++) {
		sta = func(head[i]);
		if (sta != true) {
			return INFEASIBLE;
		}
	}
	return OK;
}

}