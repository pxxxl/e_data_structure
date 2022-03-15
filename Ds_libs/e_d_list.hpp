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
	//初始化d_list
	d_list() {}
	//清除线性表的数据
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

}