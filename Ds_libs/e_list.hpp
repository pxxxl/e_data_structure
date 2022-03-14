#pragma once

#include"settings.hpp"

namespace eds {

template<typename T>
class list {
public:
	//初始化vector
	list();
	//销毁线性表，调用后不应再对vector的值做任何假设，请将其丢掉
	void destroy_list() noexcept;
	//清除线性表的数据（不清除分配的空间）
	void clear_list() noexcept;
	//返回list是否为空
	bool list_empty() const noexcept;
	//返回list长度
	unsigned list_length() const noexcept;
	//获取线性表的元素
	T& get_item(unsigned) const;
	//返回list中第一个与传入元素满足compare关系的元素的序号，调用时，func(传入的元素, list中元素)
	std::optional<unsigned> locate_item(const T&, std::function<bool(const T&, const T&)>) const noexcept;
	//返回给定元素的前驱
	std::optional<T&> prior_item(const T&) const noexcept;
	//返回给定元素的后继
	std::optional<T&> next_item(const T&) const noexcept;
	//将元素插入到某位置上
	void list_insert(T, unsigned);
	//删除给定位置上的元素
	std::optional<T&> list_delete(unsigned);
	//对list的每个对象调用传入的函数
	void list_traverse(std::function<bool(T&)>) noexcept;
private:
	unsigned len = 0;
	T* head = nullptr;

	list(const vector&);
};
//声明结束

}