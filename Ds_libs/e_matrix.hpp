#pragma once

#include"settings.hpp"

namespace eds {

template<typename T>
class matrix;

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
			printf("%-5ld", get(i, j));
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
			printf("%-5d", get(i, j));
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
			printf("%-5lld", get(i, j));
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
			printf("%-5d", get(i, j));
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
			printf("%6.2f", get(i, j));
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
			printf("%-6.3f", get(i, j));
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
			printf("%-6.3f", get(i, j));
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