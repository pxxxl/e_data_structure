#pragma once

#include"settings.hpp"

namespace eds {

template<typename T>
class linear_list {
public:
	//������Ա�����ݣ����������Ŀռ䣩
	virtual void clear_list() noexcept = 0;
	//����list�Ƿ�Ϊ��
	virtual bool list_empty() const noexcept = 0;
	//����list����
	virtual unsigned list_length() const noexcept = 0;
	//��ȡ���Ա��Ԫ������
	virtual T& get_item(unsigned) const = 0;
	//����list�е�һ���봫��Ԫ������compare��ϵ��Ԫ�ص���ţ�����ʱ��func(�����Ԫ��, list��Ԫ��)
	virtual std::tuple<unsigned, status> locate_item(const T&, std::function<bool(const T&, const T&)>) const noexcept = 0;
	//���ظ���Ԫ�ص�ǰ��
	virtual std::tuple<T&, status> prior_item(const T&) const noexcept = 0;
	//���ظ���Ԫ�صĺ��
	virtual std::tuple<T&, status> next_item(const T&) const noexcept = 0;
	//��Ԫ�ز��뵽ĳλ����
	virtual void list_insert(T, unsigned) = 0;
	//ɾ������λ���ϵ�Ԫ�أ����ˣ����ô����ã�
	virtual void list_delete(unsigned) = 0;
	//��list��ÿ��������ô���ĺ���
	virtual void list_traverse(std::function<void(T&)>) noexcept = 0;
	//β����
	virtual void push_back(T) = 0;
	//ͷ����
	virtual void insert_front(T) = 0;
	//����
	virtual ~linear_list() = 0;
};

}