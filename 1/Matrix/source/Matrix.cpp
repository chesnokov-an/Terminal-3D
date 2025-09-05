#include <iostream>
#include <Matrix/Matrix.h>

/*----------------CONSTRUCTORS----------------*/
template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::Matrix(T value){
    std::fill(begin(), end(), value);
}

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::Matrix(iterator begin, iterator end){
    std::copy(begin, end, matrix_);
}

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::Matrix(std::initializer_list<std::initializer_list<T>> list){
    std::copy(list.begin(), list.end(), begin());
}

/*----------------OPERATORS----------------*/
template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>& Matrix<T, col_size_, row_size_>::operator=(std::initializer_list<std::initializer_list<T>> list){
    std::copy(list.begin(), list.end(), begin());
}

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>& Matrix<T, col_size_, row_size_>::operator+=(const Matrix& other){
    std::transform(begin(), end(), other.begin(), begin(), std::plus<T>());
    return *this;
}

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>& Matrix<T, col_size_, row_size_>::operator-=(const Matrix& other){
    std::transform(begin(), end(), other.begin(), begin(), std::minus<T>());
    return *this;
}

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_> operator+(const Matrix<T, col_size_, row_size_>& a, const Matrix<T, col_size_, row_size_>& b){
    Matrix result = a;
    result += b;
    return result;
}

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_> operator-(const Matrix<T, col_size_, row_size_>& a, const Matrix<T, col_size_, row_size_>& b){
    Matrix result = a;
    result -= b;
    return result;
}

/*----------------GETTERS----------------*/
template <Numeric T, size_t col_size_, size_t row_size_>
size_t Matrix<T, col_size_, row_size_>::get_col() const{
    return col_size_;
}

template <Numeric T, size_t col_size_, size_t row_size_>
size_t Matrix<T, col_size_, row_size_>::get_row() const{
    return row_size_;
}

template <Numeric T, size_t col_size_, size_t row_size_>
size_t Matrix<T, col_size_, row_size_>::get_size() const{
    return col_size_ * row_size_;
}

template <Numeric T, size_t col_size_, size_t row_size_>
T& Matrix<T, col_size_, row_size_>::at(size_t i, size_t j){
    return matrix_[i][j];
}

template <Numeric T, size_t col_size_, size_t row_size_>
const T& Matrix<T, col_size_, row_size_>::at(size_t i, size_t j) const{
    return matrix_[i][j];
}
