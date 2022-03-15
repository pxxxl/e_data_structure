#pragma once

#include"settings.hpp"

namespace eds {

template<typename T>
class stack {
public:
	//����stack
	stack(unsigned size = 100);
	//���ջ
	void clear_stack() noexcept;
	//�ж��Ƿ��ջ
	bool stack_empty() const noexcept;
	//����ջ�ĳ���
	unsigned stack_length() const noexcept;
	//�鿴ջ��Ԫ��
	const T& get_top() const;
	//ѹջ
	void push(T);
	//����ջ��
	T pop();
	//����Ԫ�ؽ��в���
	
private:
	stack();
	T* base
	unsigned size;
	unsigned top;//ջ��Ԫ�ص���һ��λ��
	unsigned base = 0;
};


template<typename T>
inline stack<T>::stack(unsigned size)
{

}

}