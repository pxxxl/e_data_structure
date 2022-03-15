#pragma once

#include"settings.hpp"

namespace eds {

template<typename T>
class linear_list {
public:
	//清除线性表的数据（不清除分配的空间）
	virtual void clear_list() noexcept = 0;
	//返回list是否为空
	virtual bool list_empty() const noexcept = 0;
	//返回list长度
	virtual unsigned list_length() const noexcept = 0;
	//获取线性表的元素引用
	virtual T& get_item(unsigned) const = 0;
	//返回list中第一个与传入元素满足compare关系的元素的序号，调用时，func(传入的元素, list中元素)
	virtual std::tuple<unsigned, status> locate_item(const T&, std::function<bool(const T&, const T&)>) const noexcept = 0;
	//返回给定元素的前驱
	virtual std::tuple<T&, status> prior_item(const T&) const noexcept = 0;
	//返回给定元素的后继
	virtual std::tuple<T&, status> next_item(const T&) const noexcept = 0;
	//将元素插入到某位置上
	virtual void list_insert(T, unsigned) = 0;
	//删除给定位置上的元素（错了，不该传引用）
	virtual void list_delete(unsigned) = 0;
	//对list的每个对象调用传入的函数
	virtual void list_traverse(std::function<void(T&)>) noexcept = 0;
	//尾插入
	virtual void push_back(T) = 0;
	//头插入
	virtual void insert_front(T) = 0;
	//析构
	virtual ~linear_list() = 0;
};

}