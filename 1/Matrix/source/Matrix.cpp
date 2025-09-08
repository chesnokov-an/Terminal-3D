#include <iostream>
#include <Matrix/Matrix.h>
#include <algorithm>

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
Matrix<T, col_size_, row_size_>::const_iterator Matrix<T, col_size_, row_size_>::begin() const{
    return (&matrix_[0][0]);
}

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::const_iterator Matrix<T, col_size_, row_size_>::end() const{
    return (&matrix_[0][0] + col_size_ * row_size_);
}

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::const_iterator Matrix<T, col_size_, row_size_>::cbegin() const{
    return begin();
}

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::const_iterator Matrix<T, col_size_, row_size_>::cend() const{
    return end();
}

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::reverse_iterator Matrix<T, col_size_, row_size_>::rbegin(){
    return std::reverse_iterator<iterator>(end());
}

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::reverse_iterator Matrix<T, col_size_, row_size_>::rend(){
    return std::reverse_iterator<iterator>(begin());
}

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::const_reverse_iterator Matrix<T, col_size_, row_size_>::rbegin() const{
    return std::reverse_iterator<const_iterator>(end());
}

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::const_reverse_iterator Matrix<T, col_size_, row_size_>::rend() const{
    return std::reverse_iterator<const_iterator>(begin());
}

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::const_reverse_iterator Matrix<T, col_size_, row_size_>::crbegin() const{
    return std::reverse_iterator<const_iterator>(end());
}

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::const_reverse_iterator Matrix<T, col_size_, row_size_>::crend() const{
    return std::reverse_iterator<const_iterator>(begin());
}

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::ColumnIterator Matrix<T, col_size_, row_size_>::col_begin(){
    ColumnIterator col_it{this, begin()};
    return col_it;
}

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::ColumnIterator Matrix<T, col_size_, row_size_>::col_end(){
    ColumnIterator col_it{this, begin() + col_size_ * row_size_};
    return col_it;
}

template <Numeric T, size_t col_size_, size_t row_size_>
std::pair<typename Matrix<T, col_size_, row_size_>::iterator, typename Matrix<T, col_size_, row_size_>::iterator> Matrix<T, col_size_, row_size_>::row_iters(size_t i){
    return std::pair<iterator, iterator>(begin() + i * row_size_, begin() + (i + 1) * row_size_);
}

template <Numeric T, size_t col_size_, size_t row_size_>
std::pair<typename Matrix<T, col_size_, row_size_>::ColumnIterator, typename Matrix<T, col_size_, row_size_>::ColumnIterator> Matrix<T, col_size_, row_size_>::col_iters(size_t j){
    return std::pair<ColumnIterator, ColumnIterator>(col_begin() + j * col_size_, col_begin() + (j + 1) * col_size_);
}

/*----------------CONSTRUCTORS----------------*/
template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::Matrix(T value){
    std::fill(begin(), end(), value);
}

template <Numeric T, size_t col_size_, size_t row_size_>
template <typename InputIter>
Matrix<T, col_size_, row_size_>::Matrix(InputIter begin, InputIter end){
    std::copy(begin, end, this->begin());
}

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::Matrix(std::initializer_list<T> init){
    std::copy(init.begin(), init.end(), begin());
}

/*----------------OPERATORS----------------*/
template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>& Matrix<T, col_size_, row_size_>::operator=(std::initializer_list<T> init){
    std::copy(init.begin(), init.end(), begin());
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
T& Matrix<T, col_size_, row_size_>::operator()(size_t i, size_t j){
    return matrix_[i][j];
}

template <Numeric T, size_t col_size_, size_t row_size_>
T Matrix<T, col_size_, row_size_>::operator()(size_t i, size_t j) const{
    return matrix_[i][j];
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
size_t Matrix<T, col_size_, row_size_>::get_col_size() const{
    return col_size_;
}

template <Numeric T, size_t col_size_, size_t row_size_>
size_t Matrix<T, col_size_, row_size_>::get_row_size() const{
    return row_size_;
}

template <Numeric T, size_t col_size_, size_t row_size_>
size_t Matrix<T, col_size_, row_size_>::get_size() const{
    return col_size_ * row_size_;
}

/*----------------MAIN FUNCTIONS----------------*/
template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_> Matrix<T, col_size_, row_size_>::transposed() const{
    Matrix<T, col_size_, row_size_> result;
    std::copy(begin(), end(), result.col_begin());
    return result;
}

/*----------------COL ITERATOR----------------*/
/*----------------OPERATORS----------------*/

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::ColumnIterator& Matrix<T, col_size_, row_size_>::ColumnIterator::operator++(){
    ptr_ += matrix_->get_row();
    if(ptr_ >= matrix_->end() + matrix_->get_row() - 1){
        ptr_ = matrix_->end();
    }
    else if(ptr_ >= matrix_->end()){
        ptr_ -= matrix_->get_size() - 1;
    }
    return *this;
}

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::ColumnIterator Matrix<T, col_size_, row_size_>::ColumnIterator::operator++(int){
    ColumnIterator tmp{ptr_};
    ptr_ += matrix_->get_row();
    if(ptr_ >= matrix_->end() + matrix_->get_row() - 1){
        ptr_ = matrix_->end();
    }
    else if(ptr_ >= matrix_->end()){
        ptr_ -= matrix_->get_size();
    }
    return tmp;
}

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::ColumnIterator& Matrix<T, col_size_, row_size_>::ColumnIterator::operator--(){
    ptr_ -= matrix_->get_row();
    if(ptr_ < matrix_->begin()){
        ptr_ += matrix_->get_size();
    }
    return *this;
}

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::ColumnIterator Matrix<T, col_size_, row_size_>::ColumnIterator::operator--(int){
    ColumnIterator tmp{matrix_, ptr_};
    ptr_ -= matrix_->get_row();
    if(ptr_ < matrix_->begin()){
        ptr_ += matrix_->get_size();
    }
    return tmp;
}

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::ColumnIterator& Matrix<T, col_size_, row_size_>::ColumnIterator::operator+=(const size_t n){
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

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::ColumnIterator& Matrix<T, col_size_, row_size_>::ColumnIterator::operator-=(const size_t n){
    size_t count = n / matrix_->get_col();
    ptr_ -= count + (n % matrix_->get_col()) * matrix_->get_row();
    if((ptr_ == matrix_->begin()) || (ptr_ <= matrix_->begin() - matrix_->get_row())){
        ptr_ = matrix_->begin();
    }
    else if(ptr_ < matrix_->begin()){
        ptr_ += matrix_->get_size();
    }
    return *this;
}

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::ColumnIterator operator+(typename Matrix<T, col_size_, row_size_>::ColumnIterator it, const T n){
    typename Matrix<T, col_size_, row_size_>::ColumnIterator result = it;
    result += n;
    return result;
}

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::ColumnIterator operator+(const T n, typename Matrix<T, col_size_, row_size_>::ColumnIterator it){
    typename Matrix<T, col_size_, row_size_>::ColumnIterator result = it;
    result += n;
    return result;
}

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::ColumnIterator operator-(typename Matrix<T, col_size_, row_size_>::ColumnIterator it, const T n){
    typename Matrix<T, col_size_, row_size_>::ColumnIterator result = it;
    result -= n;
    return result;
}

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::ColumnIterator operator-(const T n, typename Matrix<T, col_size_, row_size_>::ColumnIterator it){
    typename Matrix<T, col_size_, row_size_>::ColumnIterator result = it;
    result -= n;
    return result;
}

template <Numeric T, size_t col_size_, size_t row_size_>
T& Matrix<T, col_size_, row_size_>::ColumnIterator::operator[](size_t index) const{
    return *(ptr_ + index * matrix_->get_row());
}