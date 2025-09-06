#include <iostream>
#include <Matrix/Matrix.h>
#include "Matrix.h"

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

template <Numeric T, size_t col_size_, size_t row_size_>
Col_iterator<T> Matrix<T, col_size_, row_size_>::col_begin(){
    Col_iterator<T> col_it{this, begin()};
    return col_it{this, begin()};
}

template <Numeric T, size_t col_size_, size_t row_size_>
Col_iterator<T> Matrix<T, col_size_, row_size_>::col_end(){
    Col_iterator<T> col_it{this, begin() + col_size_ * row_size_};
    return col_it;
}

template <Numeric T, size_t col_size_, size_t row_size_>
std::pair<T*, T*> Matrix<T, col_size_, row_size_>::row_iters(size_t i){
    return std::pair<T*, T*>(begin() + i * row_size_, begin() + (i + 1) * row_size_);
}

template <Numeric T, size_t col_size_, size_t row_size_>
std::pair<Col_iterator<T>, Col_iterator<T>> Matrix<T, col_size_, row_size_>::col_iters(size_t j){
    return std::pair<Col_iterator<T>, Col_iterator<T>>(col_begin() + j * col_size_, col_begin() + (j + 1) * col_size_);
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
template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>& Matrix<T, col_size_, row_size_>::transposed() const{
    Matrix<T, col_size_, row_size_> result;
    std::copy(begin(), end(), result.col_begin());
    return result;
}

/*----------------COL ITERATOR----------------*/
/*----------------OPERATORS----------------*/

template <Numeric T>
Сol_iterator<T>& Сol_iterator<T>::operator++(){
    ptr_ += matrix_->get_row();
    if(ptr_ >= matrix_->end() + matrix_->get_row() - 1){
        ptr_ = matrix_->end();
    }
    else if(ptr_ >= matrix_->end()){
        ptr_ -= matrix_->get_size() - 1;
    }
    return *this;
}

template <Numeric T>
Сol_iterator<T> Сol_iterator<T>::operator++(int){
    Сol_iterator<T> tmp{matrix_, ptr_}
    ptr_ += matrix_->get_row();
    if(ptr_ >= matrix_->end() + matrix_->get_row() - 1){
        ptr_ = matrix_->end();
    }
    else if(ptr_ >= matrix_->end()){
        ptr_ -= matrix_->get_size();
    }
    return tmp;
}

template <Numeric T>
Сol_iterator<T>& Сol_iterator<T>::operator--(){
    ptr_ -= matrix_->get_row();
    if(ptr_ < matrix_->begin()){
        ptr_ += matrix_->get_size();
    }
    return *this;
}

template <Numeric T>
Сol_iterator<T> Сol_iterator<T>::operator--(int){
    Сol_iterator<T> tmp{matrix_, ptr_}
    ptr_ -= matrix_->get_row();
    if(ptr_ < matrix_->begin()){
        ptr_ += matrix_->get_size();
    }
    return tmp;
}

template <Numeric T>
Сol_iterator<T>& Сol_iterator<T>::operator+=(const size_t n){
    size_t count = n / matrix_->get_col();
    ptr_ += count + (n % matrix_->get_col()) * matrix_->get_row();
    if(ptr_ >= matrix_->end() + matrix_->get_row() - 1){
        ptr_ = matrix_->end();
    }
    else if(ptr_ >= matrix_->end()){
        ptr_ -= matrix_->get_size();
    }
    return *this;
}

template <Numeric T>
Сol_iterator<T>& Сol_iterator<T>::operator-=(const size_t n){
    size_t count = n / matrix_->get_col();
    ptr_ -= count + (n % matrix_->get_col()) * matrix_->get_row();
    if((ptr_ == matrix_->begin()) || (ptr_ <= matrix_->begin() - matrix_->get_row())){
        ptr_ = matrix_->begin()
    }
    else if(ptr_ < matrix_->begin()){
        ptr_ += matrix_->get_size();
    }
    return *this;
}

template <Numeric T>
Сol_iterator<T> operator+(Сol_iterator<T> it, const T n){
    Сol_iterator<T> result = it;
    result += n;
    return result;
}

template <Numeric T>
Сol_iterator<T> operator+(const T n, Сol_iterator<T> it){
    Сol_iterator<T> result = it;
    result += n;
    return result;
}

template <Numeric T>
Сol_iterator<T> operator-(Сol_iterator<T> it, const T n){
    Сol_iterator<T> result = it;
    result -= n;
    return result;
}

template <Numeric T>
Сol_iterator<T> operator-(const T n, Сol_iterator<T> it){
    Сol_iterator<T> result = it;
    result -= n;
    return result;
}

template <Numeric T>
bool Сol_iterator<T>::operator==(const Сol_iterator<T>& other) const{
    return ptr_ == other.ptr_;
}

template <Numeric T>
bool Сol_iterator<T>::operator!=(const Сol_iterator<T>& other) const{
    return ptr_ != other.ptr_;
}

template <Numeric T>
bool Сol_iterator<T>::operator>(const Сol_iterator<T>& other) const{
    return ptr_ > other.ptr_;
}

template <Numeric T>
bool Сol_iterator<T>::operator<(const Сol_iterator<T>& other) const{
    return ptr_ < other.ptr_;
}

template <Numeric T>
bool Сol_iterator<T>::operator>=(const Сol_iterator<T>& other) const{
    return ptr_ >= other.ptr_;
}

template <Numeric T>
bool Сol_iterator<T>::operator<=(const Сol_iterator<T>& other) const{
    return ptr_ >= other.ptr_;
}

template <Numeric T>
T& Сol_iterator<T>::operator[](size_t index){
    return *(ptr_ + index * matrix_->get_row());
}