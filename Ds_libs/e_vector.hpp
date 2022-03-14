#pragma once

#include"settings.hpp"

namespace eds {

constexpr unsigned INIT_SIZE = 4;
constexpr double EXPAND_RATE = 1.5;

template<typename T>
class vector {
public:
	 //��ʼ��vector
	 vector();
	 //�������Ա����ú�Ӧ�ٶ�vector��ֵ���κμ��裬�뽫�䶪��
	 void destroy_list() noexcept;
	 //������Ա�����ݣ����������Ŀռ䣩
	 void clear_list() noexcept;
	 //����list�Ƿ�Ϊ��
	 bool list_empty() const noexcept;
	 //����list����
	 unsigned list_length() const noexcept;
	 //��ȡ���Ա��Ԫ��
	 T& get_item(unsigned) const;
	 //����list�е�һ���봫��Ԫ������compare��ϵ��Ԫ�ص���ţ�����ʱ��func(�����Ԫ��, list��Ԫ��)
	 std::optional<unsigned> locate_item(const T&, std::function<bool(const T&, const T&)>) const noexcept;
	 //���ظ���Ԫ�ص�ǰ��
	 std::optional<T&> prior_item(const T&) const noexcept;
	 //���ظ���Ԫ�صĺ��
	 std::optional<T&> next_item(const T&) const noexcept;
	 //��Ԫ�ز��뵽ĳλ����
	 void list_insert(T, unsigned);
	 //ɾ������λ���ϵ�Ԫ��
	 std::optional<T&> list_delete(unsigned);
	 //��list��ÿ��������ô���ĺ���
	 void list_traverse(std::function<bool(T&)>) noexcept;
	 //����[]
	 T& operator[]() const;
private:
	unsigned cap = 0;
	unsigned len = 0;
	T* head = nullptr;

	vector(const vector&);

};
//��������

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
inline void vector<T>::destroy_list() noexcept {
	this->cap = 0;
	this->len = 0;
	delete[](this->head);
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
inline std::optional<unsigned> vector<T>::locate_item(const T& sample, std::function<bool(const T&, const T&)> func) const noexcept{
	for (unsigned i = 0, bool ok = false; i < len; i++) {
		if (func(sample, head[i])) {
			return i;
		}
	}
	return std::nullopt;
}

template<typename T>
inline std::optional<T&> vector<T>::prior_item(const T& sample) const noexcept {
	if (len == 0) {
		return std::nullopt;
	}
	if (sample == (head)[0]) {
		return std::nullopt;
	}
	for (unsigned i = 1; i < len; i++) {
		if (sample == (head)[i]) {
			return head[i-1];
		}
	}
	return std::nullopt;
}

template<typename T>
inline std::optional<T&> vector<T>::next_item(const T& sample) const{
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
inline std::optional<T&> vector<T>::list_delete(unsigned n)
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
inline T& vector<T>::operator[]() const{
	
}

}