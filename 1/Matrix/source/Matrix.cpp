#include <iostream>
#include <Matrix/Matrix.h>

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::Matrix(T value){
    std::fill(&matrix_[0][0], &matrix_[0][0] + col_size_ * row_size_, value);
}

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::Matrix(T* begin, T* end){
    if((end - begin) != (col_size_ * row_size_)){
        throw std::runtime_error("Размер диапазона должен быть равен \
            числу элементов матрицы");
    }
    std::copy(begin, end, matrix_); 
}

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::Matrix(std::initializer_list<std::initializer_list<T>> list){
    if(list.size() != (col_size_ * row_size_)){
        throw std::runtime_error("Размер массива должен быть равен \
            числу элементов матрицы");
    }
    std::copy(list.begin(), list.end(), matrix_);
}

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>& Matrix<T, col_size_, row_size_>::operator=(Matrix other){
    std::swap(matrix_, other.matrix_);
    return *this;
}

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
