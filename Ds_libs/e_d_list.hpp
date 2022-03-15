#pragma once

#include"settings.hpp"

namespace eds {
template<typename T>
struct d_node {
	T data;
	d_node<T>* next;
	d_node<T>* prior;
};

template<typename T>
class d_list {
public:
	//��ʼ��d_list
	d_list() {}
	//������Ա������
	void clear_list() noexcept;
	//����list�Ƿ�Ϊ��
	bool list_empty() const noexcept;
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

}