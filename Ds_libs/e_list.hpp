#pragma once

#include"settings.hpp"

namespace eds {

template<typename T>
class list {
public:
	//��ʼ��vector
	list();
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
private:
	unsigned len = 0;
	T* head = nullptr;

	list(const vector&);
};
//��������

}