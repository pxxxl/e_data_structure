#pragma once

#include"settings.hpp"

namespace eds {
template<typename T>
class matrix;

template<typename T>
class matrix {
public:
	friend matrix<T> operator+(matrix<T> a, matrix<T> b);
	friend matrix<T> operator-(matrix<T> a, matrix<T> b);
	friend matrix<T> operator*(matrix<T> a, matrix<T> b);
	matrix(unsigned line, unsigned column, std::initializer_list<T> data);
	matrix(const matrix&);
	void set(unsigned line, unsigned column, T value);
	T get(unsigned line, unsigned column) const;
	unsigned get_line()const noexcept;
	unsigned get_column()const noexcept;
	void transposition();
	T det() const;
	void inversition();
	void adjoint();
private:
	unsigned line;
	unsigned column;
	T** mat;
};

template<typename T>
inline matrix<T>::matrix(unsigned line, unsigned column, std::initializer_list<T> data)
{
	if (line == 0 || column == 0) {
		throw std::runtime_error("From eds::matrix : line or column == 0");
	}
	this->line = line;
	this->column = column;
	unsigned total_num = line * column;
	if (total_num < line || total_num < column) {
		throw std::runtime_error("From eds::matrix : multiply overflow");
	}
	mat = new T * [line];
	if (mat == nullptr) {
		throw std::runtime_error("From eds::matrix : cannot alloc memory");
	}
	for (unsigned i = 0; i < column; i++) {
		mat[i] = new T * [column];
		if (mat[i] == nullptr) {
			throw std::runtime_error("From eds::matrix : cannot alloc memory");
		}
	}
	auto data_num = data.size();
	if (data_num > total_num) {
		throw std::runtime_error("From eds::matrix : matrix overflow, too much data");
	}
	auto be = data.begin();
	auto en = data.end();
	unsigned anchor = 0;
	for (unsigned i = 0; i < line; i++) {
		for (unsigned j = 0; j < column; j++) {
			if (be == en) {
				anchor = i * line + j;
				goto loop_break;
			}
			mat[i][j] = *be;
			be++;
		}
	}
loop_break:
	if (anchor != 0) {
		for (unsigned i = 0; i < total; i++) {
			mat[i / column][i % column] = 0;
		}
	}
	return;
}

template<typename T>
inline matrix<T>::matrix(const matrix& raw)
{

}

template<typename T>
inline void matrix<T>::set(unsigned line, unsigned column, T value)
{
	if (line >= this->line || column >= this->column) {
		throw std::out_of_range("From eds::matrix::set : index out of range");
	}
	mat[line][column] = T;
	return;
}

template<typename T>
inline T matrix<T>::get(unsigned line, unsigned column) const 
{
	if (line >= this->line || column >= this->column) {
		throw std::out_of_range("From eds::matrix::set : index out of range");
	}
	return mat[line][column];
}

template<typename T>
inline unsigned matrix<T>::get_line() const noexcept
{
	return line;
}

template<typename T>
inline unsigned matrix<T>::get_column() const noexcept
{
	return column;
}
