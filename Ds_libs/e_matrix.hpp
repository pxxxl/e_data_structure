#pragma once

#include"settings.hpp"

namespace eds {

template<typename T>
class matrix;

template<typename T>
void print_mat_decimal(matrix<T> mat);

template<typename T>
matrix<T> unit_matrix(unsigned line);

template<typename T>
class matrix{
public:
	matrix() :line(0), column(0) {};
	matrix(unsigned line, unsigned column) : line(line), column(column){
		data = new T * [line];
		handle_memory_alloc(data);
		for (unsigned i = 0; i < column; i++) {
			data[i] = new T[column];
			handle_memory_alloc(data);
		}
		for (unsigned i = 0; i < line; i++) {
			for (unsigned j = 0; j < column; j++) {
					data[i][j] = 0;
			}
		}
	};
	matrix(unsigned line, unsigned column, std::initializer_list<T> list) : line(line), column(column) {
		assert(line * column >= list.size());
		data = new T * [line];
		handle_memory_alloc(data);
		for (unsigned i = 0; i < column; i++) {
			data[i] = new T[column];
			handle_memory_alloc(data);
		}
		auto be = list.begin();
		auto en = list.end();
		for (unsigned i = 0; i < line; i++) {
			for (unsigned j = 0; j < column; j++) {
				if (be == en) {
					data[i][j] = 0;
				}
				else {
					data[i][j] = *be;
					be++;
				}
			}
		}
	};
	matrix(matrix<T>&& sample) noexcept : line(sample.line), column(sample.column) {
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

	//get the standard form of the matrix
	matrix std_line_form() const;

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

	//print the matrix itself(only int)
	void print_mat() const noexcept;

	friend std::ostream& operator<<(std::ostream& out, const matrix<T>& sample) { sample.print_mat(); return out; }

private:
	const unsigned line;
	const unsigned column;
	T** data = nullptr;
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
	data[line][column] = value;
}

template<typename T>
inline T matrix<T>::get(unsigned line, unsigned column) const
{
	if (line >= this->line || column >= this->column) {
		throw std::runtime_error("From eds::matrix::get : line or column out of bound");
	}
	return data[line][column];
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
			new_mat.reset(i, j, this->data[j][i]);
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
	T multiple_sum = 1;
	for (unsigned i = 0; i < line; i++) {
		for (unsigned j = i + 1; j < line; j++) {
			holder = tool.data[j][i];
			for (unsigned m = 0; m < line; m++) {
				tool.data[j][m] *= tool.data[i][i];
				tool.data[j][m] -= tool.data[i][m] * holder;
			}
			multiple_sum *= tool.data[i][i];
		}
	}
	holder = 1;
	for (unsigned i = 0; i < line; i++) {
		holder *= tool.data[i][i];
	}
	return holder/multiple_sum;
}

template<typename T>
inline matrix<T> matrix<T>::inversition() const
{
	static_assert(is_decimal<T>(T()), "make sure T is demical before calling inversition");
	if (line != column) {
		throw std::runtime_error("From eds::matrix::inversition : line != column");
	}
	T det_sum = det();
	if (det_sum < 10E-6 && det_sum > -(10E-6)) {
		return matrix();
	}
	matrix<T> tool = copy();
	matrix<T> target = unit_matrix<T>(line);
	T holder;

	for (unsigned i = 0; i < line - 1; i++) {
		for (unsigned j = i + 1; j < line; j++) {
			holder = tool.data[j][i];
			for (unsigned m = 0; m < line; m++) {
				tool.data[j][m] -= tool.data[i][m] * holder / tool.data[i][i];
				target.data[j][m] -= target.data[i][m] * holder / tool.data[i][i];
			}
		}
	}
	for (unsigned i = line - 1; i > 0; i--) {
		for (unsigned j = i - 1; j > 0; j--) {
			holder = tool.data[j][i];
			for (unsigned m = 0; m < line; m++) {
				tool.data[j][m] -= tool.data[i][m] * holder / tool.data[i][i];
				target.data[j][m] -= target.data[i][m] * holder / tool.data[i][i];
			}
		}
		holder = tool.data[0][i];
		for (unsigned m = 0; m < line; m++) {
			tool.data[0][m] -= tool.data[i][m] * holder / tool.data[i][i];
			target.data[0][m] -= target.data[i][m] * holder / tool.data[i][i];
		}
	}
	for (unsigned i = 0; i < line; i++) {
		for (unsigned j = 0; j < line; j++) {
			target.data[i][j] /= tool.data[i][i];
		}
	}
	return target;
}

template<typename T>
inline matrix<T> eds::matrix<T>::adjoint() const
{
	return inversition()*det();
}

//the method below is complex enough to force me adding some notes 
template<typename T>
inline matrix<T> matrix<T>::std_line_form() const
{
	static_assert(is_decimal<T>(T()), "make sure T is demical before calling std_line_form");
	matrix<T> sample = copy();
	unsigned cur_line = 0;
	unsigned cur_column = 0;
	T holder = 0;
	unsigned hold_line = 0;
	bool flag = true;

	//if cursor is out of bound, transfer has completed, then return the result
	while(cur_line < line && cur_column < column) {
		//if cursor element is zero, check its elements below
		if (is_zero(sample.data[cur_line][cur_column])) {
			flag = false;
			for (unsigned below = cur_line; below < line; below++) {
				if (!is_zero(sample.data[below][cur_column])) {
					flag = true;
					hold_line = below;
					break;
				}
			}
			//one of the elements below is not zero, then swap the two line
			if (flag == true) {
				for (unsigned i = 0; i < column; i++) {
					swap<T>(sample.data[hold_line][i], sample.data[cur_line][i]);
				}
			}
			//cursor's below elements are all zero, then cursor move to its right
			else {
				cur_column++;
			}
		}
		//if cursor element is not zero, turn it to 1, and clear all the element in this column
		else {
			for (unsigned i = cur_column; i < column; i++) {
				sample.data[cur_line][i] /= sample.data[cur_line][cur_column];
			}
			for (unsigned i = 0; i < cur_line; i++) {
				holder = sample.data[i][cur_column];
				for (unsigned j = cur_column; j < column; j++) {
					sample.data[i][j] -= sample.data[cur_column][j] * holder;
				}
			}
			for (unsigned i = cur_line + 1; i < line; i++) {
				holder = sample.data[i][cur_column];
				for (unsigned j = cur_column; j < column; j++) {
					sample.data[i][j] -= sample.data[cur_column][j] * holder;
				}
			}
			cur_column++;
			cur_line++;
		}
	}
	return sample;
}

template<typename T>
inline matrix<T> matrix<T>::copy() const
{
	matrix<T> tool(line, column);
	for (unsigned i = 0; i < line; i++) {
		for (unsigned j = 0; j < column; j++) {
			tool.data[i][j] = this->data[i][j];
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
	matrix<T> tool(line, column);
	for (unsigned i = 0; i < line; i++) {
		for (unsigned j = 0; j < column; j++) {
			tool.data[i][j] = data[i][j] + mat.data[i][j];
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
			data[i][j] += mat.data[i][j];
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
	matrix<T> tool(line, column);
	for (unsigned i = 0; i < line; i++) {
		for (unsigned j = 0; j < column; j++) {
			tool.data[i][j] = data[i][j] - mat.data[i][j];
		}
	}
	return tool;
}

template<typename T>
inline void matrix<T>::subtract_self(const matrix<T>& mat) const
{
	if (mat.line != this->line || mat.column != this->column) {
		throw std::runtime_error("From eds::matrix::subtract_self : matrixes do not have the same format");
	}
	for (unsigned i = 0; i < line; i++) {
		for (unsigned j = 0; j < column; j++) {
			this->data[i][j] -= mat.data[i][j];
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
				tool.data[i][j] += this->data[i][m] * mat.data[m][j];
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
			tool.data[i][j] = data[i][j] * num;
		}
	}
	return tool;
}

template<typename T>
inline void matrix<T>::num_multiply_self(const T& num) const
{
	for (unsigned i = 0; i < line; i++) {
		for (unsigned j = 0; j < column; j++) {
			data[i][j] *= num;
		}
	}
	return;
}

template<typename T>
inline void matrix<T>::print_mat() const noexcept
{
	std::cout << "Error : type is neither integer nor demical";
}

inline void matrix<long>::print_mat() const noexcept
{
	std::cout << "Matrix : " << std::endl;
	for (unsigned i = 0; i < get_line(); i++) {
		for (unsigned j = 0; j < get_column(); j++) {
			printf("%-5ld  ", get(i, j));
		}
		std::cout << std::endl;
		std::cout << std::endl;
	}
}

inline void matrix<int>::print_mat() const noexcept
{
	std::cout << "Matrix : " << std::endl;
	for (unsigned i = 0; i < get_line(); i++) {
		for (unsigned j = 0; j < get_column(); j++) {
			printf("%-5d  ", get(i, j));
		}
		std::cout << std::endl;
		std::cout << std::endl;
	}
}

inline void matrix<long long>::print_mat() const noexcept
{
	std::cout << "Matrix : " << std::endl;
	for (unsigned i = 0; i < get_line(); i++) {
		for (unsigned j = 0; j < get_column(); j++) {
			printf("%-5lld  ", get(i, j));
		}
		std::cout << std::endl;
		std::cout << std::endl;
	}
}

inline void matrix<short>::print_mat() const noexcept
{
	std::cout << "Matrix : " << std::endl;
	for (unsigned i = 0; i < get_line(); i++) {
		for (unsigned j = 0; j < get_column(); j++) {
			printf("%-5d  ", get(i, j));
		}
		std::cout << std::endl;
		std::cout << std::endl;
	}
}

inline void matrix<double>::print_mat() const noexcept
{
	std::cout << "Matrix : " << std::endl;
	for (unsigned i = 0; i < get_line(); i++) {
		for (unsigned j = 0; j < get_column(); j++) {
			printf("%6.2f  ", get(i, j));
		}
		std::cout << std::endl;
		std::cout << std::endl;
	}
}

inline void matrix<float>::print_mat() const noexcept
{
	std::cout << "Matrix : " << std::endl;
	for (unsigned i = 0; i < get_line(); i++) {
		for (unsigned j = 0; j < get_column(); j++) {
			printf("%6.2f  ", get(i, j));
		}
		std::cout << std::endl;
		std::cout << std::endl;
	}
}

inline void matrix<long double>::print_mat() const noexcept
{
	std::cout << "Matrix : " << std::endl;
	for (unsigned i = 0; i < get_line(); i++) {
		for (unsigned j = 0; j < get_column(); j++) {
			printf("%6.2f  ", get(i, j));
		}
		std::cout << std::endl;
		std::cout << std::endl;
	}
}

template<typename T>
void print_mat_decimal(const matrix<T>* mat)
{
	std::cout << "Matrix : " << std::endl;
	for (unsigned i = 0; i < (*mat).get_line(); i++) {
		for (unsigned j = 0; j < (*mat).get_column(); j++) {
			printf("%6.2f  ", (*mat).get(i, j));
		}
		std::cout << std::endl;
		std::cout << std::endl;
	}
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