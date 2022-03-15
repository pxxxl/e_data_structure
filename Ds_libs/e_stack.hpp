#pragma once

#include"settings.hpp"

namespace eds {

template<typename T>
class stack {
public:
	//构造stack
	stack(unsigned size = 100);
	//清除栈
	void clear_stack() noexcept;
	//判断是否空栈
	bool stack_empty() const noexcept;
	//返回栈的长度
	unsigned stack_length() const noexcept;
	//查看栈顶元素
	const T& get_top() const;
	//压栈
	void push(T);
	//弹出栈顶
	T pop();
	//所有元素进行操作
	
private:
	stack();
	T* base
	unsigned size;
	unsigned top;//栈顶元素的上一个位置
	unsigned base = 0;
};


template<typename T>
inline stack<T>::stack(unsigned size)
{

}

}