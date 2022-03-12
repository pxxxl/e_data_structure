#pragma once

#include"settings.hpp"

/*
 * 声明：
 * 忽略status值可能引起空引用等错误。
 * 本库所有函数及方法均会返回status值。多返回值使用tuple，其中status值是最后一个返回值。
 * 本库不处理错误，只设置status值，请调用者使用本库自带的 handle(status) 检验返回的status。
*/

namespace eds {

constexpr unsigned INIT_SIZE = 4;
constexpr double EXPAND_RATE = 1.5;

template<typename T>
class vector;

//返回一个已经初始化的线性表
template<typename T>
std::tuple<vector<T>, status> init_vector();

template<typename T>
class vector {
	friend std::tuple<vector<T>, status> init_list();
public:
	 //销毁线性表，调用后不应再对vector的值做任何假设，请将其丢掉
	 status destroy_list();
	 //清除线性表的数据（不清除分配的空间）
	 status clear_list();
	 //返回list是否为空
	 std::tuple<bool, status> list_empty();
	 //返回list长度
	 std::tuple<unsigned, status> list_length();
	 //获取线性表的元素
	 std::tuple<T&, status> get_item(unsigned);
	 //返回list中第一个与传入元素满足compare关系的元素的序号，调用时，func(传入的元素, list中元素)
	 std::tuple<unsigned, status> locate_item(const T&, std::function<bool(const T&, const T&)>);
	 //返回给定元素的前驱
	 std::tuple<T&, status> prior_item(const T&);
	 //返回给定元素的后继
	 std::tuple<T&, status> next_item(const T&);
	 //将元素插入到某位置上（拷贝构造）
	 status list_insert(T, unsigned);
	 //删除给定位置上的元素
	 std::tuple<T&, status> list_delete(unsigned);
	 //对list的每个对象调用传入的函数
	 status list_traverse(std::function<bool(T&)>);
private:
	unsigned cap;
	unsigned len;
	T* head;

	vector();
	vector(const vector&);
	~vector();
};
//声明结束

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