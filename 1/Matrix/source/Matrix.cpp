#include <iostream>
#include <Matrix/Matrix.h>

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::Matrix(T value){
    std::fill(&matrix_[0][0], &matrix_[0][0] + col_size_ * row_size_, value);
}
