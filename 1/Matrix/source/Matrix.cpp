#include <iostream>
#include <Matrix/Matrix.h>

/*----------------ITERATORS----------------*/
template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::iterator Matrix<T, col_size_, row_size_>::begin(){
    return (&matrix_[0][0]);
}

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::iterator Matrix<T, col_size_, row_size_>::end(){
    return (&matrix_[0][0] + col_size_ * row_size_);
}

template <Numeric T, size_t col_size_, size_t row_size_>
const Matrix<T, col_size_, row_size_>::iterator Matrix<T, col_size_, row_size_>::cbegin() const{
    return begin();
}

template <Numeric T, size_t col_size_, size_t row_size_>
const Matrix<T, col_size_, row_size_>::iterator Matrix<T, col_size_, row_size_>::cend() const{
    return end();
}

template <Numeric T, size_t col_size_, size_t row_size_>
std::reverse_iterator<T*> Matrix<T, col_size_, row_size_>::rbegin(){
    return std::reverse_iterator<T*>(end());
}

template <Numeric T, size_t col_size_, size_t row_size_>
std::reverse_iterator<T*> Matrix<T, col_size_, row_size_>::rend(){
    return std::reverse_iterator<T*>(begin());
}

template <Numeric T, size_t col_size_, size_t row_size_>
const std::reverse_iterator<T*> Matrix<T, col_size_, row_size_>::crbegin() const{
    return std::reverse_iterator<T*>(end());
}

template <Numeric T, size_t col_size_, size_t row_size_>
const std::reverse_iterator<T*> Matrix<T, col_size_, row_size_>::crend() const{
    return std::reverse_iterator<T*>(begin());
}

/*----------------CONSTRUCTORS----------------*/
template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::Matrix(T value){
    std::fill(begin(), end(), value);
}

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::Matrix(iterator begin, iterator end){
    std::copy(begin, end, this->begin());
}

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::Matrix(std::initializer_list<std::initializer_list<T>> init){
    iterator it = begin();
    std::ranges::for_each(init, [](std::initializer_list<T> init_row){
        std::copy(init_row.begin(), init_row.end(), it); 
        it += row_size_;
    })
}

/*----------------OPERATORS----------------*/
template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>& Matrix<T, col_size_, row_size_>::operator=(std::initializer_list<std::initializer_list<T>> init){
    iterator it = begin();
    std::ranges::for_each(init, [](std::initializer_list<T> init_row){
        std::copy(init_row.begin(), init_row.end(), it); 
        it += row_size_;
    })
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

/*----------------SETTERS----------------*/
template <Numeric T, size_t col_size_, size_t row_size_>
void Matrix<T, col_size_, row_size_>::set(size_t i, size_t j, T value){
    matrix_[i][j] = value;
}

/*----------------MAIN FUNCTIONS----------------*/
/*template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>& Matrix<T, col_size_, row_size_>::get_transposed() const{
    Matrix result;
    for(int i = 0; i < col_size_; i++){
        for(int j = 0; j < row_size_; j++){
            result.set(j, i, matrix_[i][j]);
        }
    }
    std::transform(begin(), end(), result.begin(), [](){});
}*/