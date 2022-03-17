#pragma once

#include"settings.hpp"

namespace eds {
template<typename T>
class matrix{
public:
	matrix() : line(line), column(column) {};
	matrix(unsigned line, unsigned column) : line(line), column(column){};
	matrix(unsigned line, unsigned column, std::initializer_list<T> list) : line(line), column(column) {
		assert(line * column < list.size());
		data = new T[line * column];
		handle_memory_alloc(data);
		auto be = list.begin();
		auto en = list.end();
		for (unsigned i = 0; i < line; i++) {
			for (unsigned j = 0; j < column; j++) {
				if (be == en) {
					data[i * line + j] = 0;
				}
				else {
					data[i * line + j] = *be;
					be++;
				}
			}
		}
	};
	matrix(matrix<T>&& sample) : line(sample.line), column(sample.column) {
		this->data = sample.data;
		sample.data = nullptr;
	}

	//if the matrix is null, return true
	bool is_null();

	//reset the value of one element
	void reset(unsigned line, unsigned column, T value);

	//get the value of one element
	T get(unsigned line, unsigned column) const;

	//get the number of lines of the matrix
	unsigned get_line() const noexcept;

	//get the number of columns of the matrix
	unsigned get_column() const noexcept;

	//get the transposition of the matrix
	matrix transposition() const;

	//get the det of the matrix
	T det() const;

	//get the inversition of the matrix
	matrix inversition() const;

	//get the adjoint of the matrix
	matrix adjoint() const;

	//get the copy of the matrix
	matrix copy() const;

	//get the result of matrix plus
	matrix add(const matrix& mat) const;

	//add the matrix to the main matrix
	void add_self(const matrix& mat) const;

	//get the result of matrix subtract
	matrix subtract(const matrix& mat) const;

	//the main matrix will be subtracted 
	void subtract_self(const matrix& mat) const;

	//get the result of matrix multiply
	matrix multiply(const matrix& mat) const;

	//number multiply
	matrix num_multiply(const T& num) const;

	//number multiply self
	void num_multiply_self(const T& num) const;

private:
	const unsigned line;
	const unsigned column;
	T* data;
};

template<typename T>
inline bool matrix<T>::is_null()
{
	if (line == 0 || column == 0) {
		return true;
	}
	return false;
}

template<typename T>
inline void matrix<T>::reset(unsigned line, unsigned column, T value) {
	if (line >= this->line || column >= this->column) {
		throw std::runtime_error("From eds::matrix::reset : line or column out of bound");
	}
	data[line * this->column + column] = value;
}

template<typename T>
inline T matrix<T>::get(unsigned line, unsigned column) const
{
	if (line >= this->line || column >= this->column) {
		throw std::runtime_error("From eds::matrix::get : line or column out of bound");
	}
	return data[line * this->column + column];
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

template<typename T>
inline matrix<T> matrix<T>::transposition() const
{	
	auto new_mat = matrix(this->column, this->line);
	for (unsigned i = 0; i < new_mat.line; i++) {
		for (unsigned j = 0; j < new_mat.column; j++) {
			new_mat.reset(i, j, this->data[j * column + i]);
		}
	}
	return new_mat;
}

template<typename T>
inline T matrix<T>::det() const
{
	if (line != column) {
		throw std::runtime_error("From eds::matrix::det : line != column");
	}
	matrix<T> tool = copy();
	T holder;
	for (unsigned i = 0; i < line; i++) {
		for (unsigned j = i + 1; j < line; j++) {
			holder = tool.data[j * column + i];
			for (unsigned m = 0; m < line; m++) {
				tool.data[j * column + m] *= tool.data[i * column + i];
				tool.data[j * column + m] -= tool.data[i * column + m] * holder;
			}
		}
	}
	holder = 1;
	for (unsigned i = 0; i < line; i++) {
		holder *= tool.data[i * column + i];
	}
	return T();
}

template<typename T>
inline matrix<T> matrix<T>::inversition() const
{
	if (line != column) {
		throw std::runtime_error("From eds::matrix::inversition : line != column");
	}
	T det_sum = det();
	if (det_sum < 10E-6) {
		return matrix();
	}
	matrix<T> tool = copy();
	matrix<T> target = unit_matrix<T>(line);
	T holder;
	for (unsigned i = 0; i < line; i++) {
		for (unsigned j = i + 1; j < line; j++) {
			holder = tool.data[j * column + i];
			for (unsigned m = 0; m < line; m++) {
				tool.data[j * column + m] *= tool.data[i * column + i];
				target.data[j * column + m] *= tool.data[i * column + i];
				tool.data[j * column + m] -= tool.data[i * column + m] * holder;
				target.data[j * column + m] -= tool.data[i * column + m] * holder;
			}
		}
	}
	for (unsigned i = line; i >= 0; i--) {
		for (unsigned j = i - 1; j >= 0; j--) {
			holder = tool.data[j * column + i];
			for (unsigned m = 0; m < line; m++) {
				tool.data[j * column + m] *= tool.data[i * column + i];
				target.data[j * column + m] *= tool.data[i * column + i];
				tool.data[j * column + m] -= tool.data[i * column + m] * holder;
				target.data[j * column + m] -= tool.data[i * column + m] * holder;
			}
		}
	}
	return target;
}

template<typename T>
inline matrix<T> eds::matrix<T>::adjoint() const
{
	return inversition()*det();
}

template<typename T>
inline matrix<T> matrix<T>::copy() const
{
	matrix<T> tool(line, column);
	for (unsigned i = 0; i < line; i++) {
		for (unsigned j = 0; j < column; j++) {
			tool.data[i * column + j] = this->data[i * column + j];
		}
	}
	return tool;
}

template<typename T>
inline matrix<T> matrix<T>::add(const matrix<T>& mat) const
{
	if (mat.line != this->line || mat.column != this->column) {
		throw std::runtime_error("From eds::matrix::add : matrixes do not have the same format");
	}
	matrix<T> tool(this->line, mat.column);
	auto l = this->line;
	auto c = mat.column;
	for (unsigned i = 0; i < l; i++) {
		for (unsigned j = 0; j < c; j++) {
			tool.data[i * column + j] += mat.data[i * column + j];
		}
	}
	return tool;
}

template<typename T>
inline void matrix<T>::add_self(const matrix<T>& mat) const
{
	if (mat.line != this->line || mat.column != this->column) {
		throw std::runtime_error("From eds::matrix::add_self : matrixes do not have the same format");
	}
	for (unsigned i = 0; i < line; i++) {
		for (unsigned j = 0; j < column; j++) {
			this->data[i * column + j] += mat.data[i * column + j];
		}
	}
	return;
}

template<typename T>
inline matrix<T> matrix<T>::subtract(const matrix<T>& mat) const
{
	if (mat.line != this->line || mat.column != this->column) {
		throw std::runtime_error("From eds::matrix::add : matrixes do not have the same format");
	}
	matrix<T> tool(this->line, mat.column);
	auto l = this->line;
	auto c = mat.column;
	for (unsigned i = 0; i < l; i++) {
		for (unsigned j = 0; j < c; j++) {
			tool.data[i * column + j] -= mat.data[i * column + j];
		}
	}
	return tool;
	return matrix();
}

template<typename T>
inline void matrix<T>::subtract_self(const matrix<T>& mat) const
{
	if (mat.line != this->line || mat.column != this->column) {
		throw std::runtime_error("From eds::matrix::subtract_self : matrixes do not have the same format");
	}
	for (unsigned i = 0; i < line; i++) {
		for (unsigned j = 0; j < column; j++) {
			this->data[i * column + j] -= mat.data[i * column + j];
		}
	}
	return;
}

template<typename T>
inline matrix<T> matrix<T>::multiply(const matrix<T>& mat) const
{
	if (this->column != mat.line) {
		throw std::runtime_error("From eds::matrix::multiply : this->column != mat.line");
	}
	matrix<T> tool(this->line, mat.column);
	auto l = this->line;
	auto c = mat.column;
	auto bound = this->column;
	for (unsigned i = 0; i < l; i++) {
		for (unsigned j = 0; j < c; j++) {
			for (unsigned m = 0; m < bound; m++) {
				tool.data[i * column + j] += this->data[i * column + m] * mat.data[m * column + j];
			}
		}
	}
	return tool;
}

template<typename T>
inline matrix<T> matrix<T>::num_multiply(const T& num) const
{
	matrix<T> tool(line, column);
	for (unsigned i = 0; i < line; i++) {
		for (unsigned j = 0; j < column; j++) {
			tool.data[i * column + j] *= num;
		}
	}
	return tool;
}

template<typename T>
inline void matrix<T>::num_multiply_self(const T& num) const
{
	for (unsigned i = 0; i < line; i++) {
		for (unsigned j = 0; j < column; j++) {
			data[i * column + j] *= num;
		}
	}
	return;
}


template<typename T>
inline matrix<T> unit_matrix(unsigned line) {
	matrix<T> tool = matrix<T>(line, line);
	for (unsigned i = 0; i < line; i++) {
		tool.reset(i, i, 1);
	}
	return tool;
}
template<typename T>
inline matrix<T> operator+(const matrix<T>& a, const matrix<T>& b) {
	return a.add(b);
}

template<typename T>
inline matrix<T> operator-(const matrix<T>& a, const matrix<T>& b) {
	return a.subtract(b);
}

template<typename T>
inline matrix<T> operator*(const matrix<T>& a, const matrix<T>& b) {
	return a.multiply(b);
}

template<typename T>
inline matrix<T> operator*(const matrix<T>& a, const T& b) {
	return a.num_multiply(b);
}
};