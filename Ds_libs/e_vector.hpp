#pragma once

#include"settings.hpp"

/*
 * ������
 * ����statusֵ������������õȴ���
 * �������к������������᷵��statusֵ���෵��ֵʹ��tuple������statusֵ�����һ������ֵ��
 * ���ⲻ��������ֻ����statusֵ���������ʹ�ñ����Դ��� handle(status) ���鷵�ص�status��
*/

namespace eds {

constexpr unsigned INIT_SIZE = 4;
constexpr double EXPAND_RATE = 1.5;

template<typename T>
class vector;

//����һ���Ѿ���ʼ�������Ա�
template<typename T>
std::tuple<vector<T>, status> init_vector();

template<typename T>
class vector {
	friend std::tuple<vector<T>, status> init_list();
public:
	 //�������Ա������ú�Ӧ�ٶ�vector��ֵ���κμ��裬�뽫�䶪��
	 status destroy_list();
	 //������Ա������ݣ����������Ŀռ䣩
	 status clear_list();
	 //����list�Ƿ�Ϊ��
	 std::tuple<bool, status> list_empty();
	 //����list����
	 std::tuple<unsigned, status> list_length();
	 //��ȡ���Ա���Ԫ��
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
	unsigned cap;
	unsigned len;
	T* head;

	vector();
	vector(const vector&);
	~vector();
};
//��������

template<typename T>
inline std::tuple<vector<T>&, status> init_vector() {
	vector<T> vec();
	vec.head = new T[INIT_SIZE];
	if (vec.head == nullptr) {
		return std::make_tuple < vector<T>&, status>(vec, OVERFLOWED);
	}
	vec.cap = INIT_SIZE;
	vec.len = 0;
	return std::make_tuple < vector<T>&, status>(vec, OK);
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
		return make_tuple<bool, status>(false, OK);
	}
	else {
		return make_tuple<bool, status>(true, OK);
	}
}

template<typename T>
inline std::tuple<unsigned, status> vector<T>::list_length()
{
	return std::make_tuple<unsigned, status>(len, OK);
}

template<typename T>
inline std::tuple<T&, status> vector<T>::get_item(unsigned n){
	if (n < len && n >= 0) {
		return std::make_tuple<T&, status>(head[n], OK);
	}
	else {
		return std::make_tuple<T&, status>(static_cast<T&>(nullptr), INFEASIBLE);
	}
}

template<typename T>
inline std::tuple<unsigned, status> vector<T>::locate_item(const T& sample, std::function<bool(const T&, const T&)> func){
	for (unsigned i = 0, bool ok = false; i < len; i++) {
		if (func(sample, head[i])) {
			return std::make_tuple<unsigned, status>(i, OK);
		}
	}
	return std::make_tuple<unsigned, status>(0, INFEASIBLE);
}

template<typename T>
inline std::tuple<T&, status> vector<T>::prior_item(const T& sample){
	if (len == 0) {
		return std::make_tuple<T&, status>(static_cast<T&>(nullptr), INFEASIBLE);
	}
	if (sample == (head)[0]) {
		return std::make_tuple<T&, status>(static_cast<T&>(nullptr), INFEASIBLE);
	}
	for (unsigned i = 1; i < len; i++) {
		if (sample == (head)[i]) {
			return std::make_tuple<T&, status>((head)[i-1], OK);
		}
	}
	return std::make_tuple<T&, status>(static_cast<T&>(nullptr), INFEASIBLE);
}

template<typename T>
inline std::tuple<T&, status> vector<T>::next_item(const T& sample)
{
	if (len == 0) {
		return std::make_tuple<T&, status>(static_cast<T&>(nullptr), INFEASIBLE);
	}
	if (sample == head[this->len - 1]) {
		return std::make_tuple<T&, status>(static_cast<T&>(nullptr), INFEASIBLE);
	}
	for (unsigned i = 0; i < len - 1; i++) {
		if (sample == head[i]) {
			return std::make_tuple<T&, status>((this->head)[i + 1], OK);
		}
	}
	return std::make_tuple<T&, status>(static_cast<T&>(nullptr), INFEASIBLE);
}

template<typename T>
inline status vector<T>::list_insert(T sample, unsigned n){
	if (n >= len || n < 0) {
		return INFEASIBLE;
	}
	if (len == cap) {
		new_cap = cap * EXPAND_RATE;
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
		return std::make_tuple<T&, status>(static_cast<T&>(nullptr), INFEASIBLE);
	}
	T& copy = head[n];
	for (unsigned i = n; i < len - 1; i++) {
		head[i] = head[i + 1];
	}
	len--;
	return std::make_tuple<T&, status>(copy, OK);
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