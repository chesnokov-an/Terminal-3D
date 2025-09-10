#ifndef MATRIX_H
#define MATRIX_H

#include <type_traits>
#include <cstddef>
#include <utility>
#include <iterator>
#include <algorithm>
#include <numeric>

template <typename T>
concept Numeric = std::is_integral_v<T> || std::is_floating_point_v<T>;

template <Numeric T, size_t col_size_, size_t row_size_>
class Matrix{
private:
    T matrix_[col_size_][row_size_]{};

public:
    using iterator = T*;
    using const_iterator = const T*;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    class ColumnIterator;
    
    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

    const_iterator cbegin() const;
    const_iterator cend() const;

    reverse_iterator rbegin();
    reverse_iterator rend();
    const_reverse_iterator rbegin() const;
    const_reverse_iterator rend() const;

    const_reverse_iterator crbegin() const;
    const_reverse_iterator crend() const;

    ColumnIterator col_begin();
    const ColumnIterator col_begin() const;
    ColumnIterator col_end();
    const ColumnIterator col_end() const;

    std::pair<iterator, iterator> row_iters(size_t i);
    std::pair<const_iterator, const_iterator> row_iters(size_t i) const;
    std::pair<ColumnIterator, ColumnIterator> col_iters(size_t j);
    std::pair<const ColumnIterator, const ColumnIterator> col_iters(size_t j) const;

public:
    Matrix() = default;
    Matrix(const Matrix& other) = default;
    Matrix(T value);
    template <typename InputIter>
    Matrix(InputIter begin, InputIter end);
    Matrix(std::initializer_list<T> init);
    
    Matrix& operator=(const Matrix& other) = default;
    Matrix& operator=(std::initializer_list<T> init);
    Matrix& operator+=(const Matrix& other);
    Matrix& operator-=(const Matrix& other);

    T& operator()(size_t i, size_t j);
    T operator()(size_t i, size_t j) const;
    
    size_t get_col_size() const;
    size_t get_row_size() const;
    size_t get_size() const;

    Matrix<T, row_size_, col_size_> transposed() const;

    class ColumnIterator{
    private:
        T* start_ = nullptr;
        size_t pos_ = 0;

    public:
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::random_access_iterator_tag;

        ColumnIterator() = default;
        ColumnIterator(const ColumnIterator& other) = default;
        ColumnIterator(T* start, T pos) : start_(start), pos_(pos){}

        reference operator*();
        const reference operator*() const;
        pointer operator->();
        const pointer operator->() const;
        ColumnIterator& operator++();
        ColumnIterator operator++(int);
        ColumnIterator& operator--();
        ColumnIterator operator--(int);
        ColumnIterator& operator=(const ColumnIterator& other) & = default;
        ColumnIterator& operator+=(size_t n);
        ColumnIterator& operator-=(size_t n);
        difference_type operator-(const ColumnIterator& other) const;
        bool operator==(const ColumnIterator& other) const = default;
        std::strong_ordering operator<=>(const ColumnIterator& other) const = default;
        reference operator[](size_t index);
        const reference operator[](size_t index) const;

        ColumnIterator operator+(size_t n) const;
        friend ColumnIterator operator+(size_t n, const ColumnIterator& it){
            return it + n;
        }
        ColumnIterator operator-(size_t n) const;
        friend ColumnIterator operator-(size_t n, ColumnIterator& it){
            return it - n;
        }
    };
};

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_> operator+(const Matrix<T, col_size_, row_size_>& a, const Matrix<T, col_size_, row_size_>& b);

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_> operator-(const Matrix<T, col_size_, row_size_>& a, const Matrix<T, col_size_, row_size_>& b);

template <Numeric T, size_t M, size_t N, size_t P>
Matrix<T, M, P> operator*(const Matrix<T, M, N>& a, const Matrix<T, N, P>& b);

/*****************Realization*****************/
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
    ColumnIterator col_it{begin(), 0};
    return col_it;
}

template <Numeric T, size_t col_size_, size_t row_size_>
const Matrix<T, col_size_, row_size_>::ColumnIterator Matrix<T, col_size_, row_size_>::col_begin() const{
    const ColumnIterator col_it{begin(), 0};
    return col_it;
}

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::ColumnIterator Matrix<T, col_size_, row_size_>::col_end(){
    ColumnIterator col_it{begin(), col_size_ * row_size_};
    return col_it;
}

template <Numeric T, size_t col_size_, size_t row_size_>
const Matrix<T, col_size_, row_size_>::ColumnIterator Matrix<T, col_size_, row_size_>::col_end() const{
    const ColumnIterator col_it{begin(), col_size_ * row_size_};
    return col_it;
}

template <Numeric T, size_t col_size_, size_t row_size_>
std::pair<typename Matrix<T, col_size_, row_size_>::iterator, typename Matrix<T, col_size_, row_size_>::iterator> Matrix<T, col_size_, row_size_>::row_iters(size_t i){
    return std::pair<iterator, iterator>(begin() + i * row_size_, begin() + (i + 1) * row_size_);
}

template <Numeric T, size_t col_size_, size_t row_size_>
std::pair<typename Matrix<T, col_size_, row_size_>::const_iterator, typename Matrix<T, col_size_, row_size_>::const_iterator> Matrix<T, col_size_, row_size_>::row_iters(size_t i) const{
    return std::pair<const_iterator, const_iterator>(begin() + i * row_size_, begin() + (i + 1) * row_size_);
}

template <Numeric T, size_t col_size_, size_t row_size_>
std::pair<typename Matrix<T, col_size_, row_size_>::ColumnIterator, typename Matrix<T, col_size_, row_size_>::ColumnIterator> Matrix<T, col_size_, row_size_>::col_iters(size_t j){
    return std::pair<ColumnIterator, ColumnIterator>(col_begin() + j * col_size_, col_begin() + (j + 1) * col_size_);
}

template <Numeric T, size_t col_size_, size_t row_size_>
std::pair<const typename Matrix<T, col_size_, row_size_>::ColumnIterator, const typename Matrix<T, col_size_, row_size_>::ColumnIterator> Matrix<T, col_size_, row_size_>::col_iters(size_t j) const{
    return std::pair<const ColumnIterator, const ColumnIterator>(col_begin() + j * col_size_, col_begin() + (j + 1) * col_size_);
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

template <Numeric T, size_t M, size_t N, size_t P>
Matrix<T, M, P> operator*(const Matrix<T, M, N>& a, const Matrix<T, N, P>& b){
    Matrix<T, P, N> b_transposed = b.transposed();
    Matrix<T, M, P> result;
    
    for(size_t i = 0; i < M * P; i++){
        size_t res_row = i / P;
        size_t res_col = i % P;

        auto a_row = a.row_iters(res_row).first;
        auto b_row = b_transposed.row_iters(res_col).first;

        result(res_row, res_col) = std::inner_product(a_row, a_row + N, b_row, 0);
    }

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
Matrix<T, row_size_, col_size_> Matrix<T, col_size_, row_size_>::transposed() const{
    Matrix<T, row_size_, col_size_> result;
    std::copy(begin(), end(), result.col_begin());
    return result;
}

/*----------------COL ITERATOR----------------*/
/*----------------OPERATORS----------------*/

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::ColumnIterator::reference Matrix<T, col_size_, row_size_>::ColumnIterator::operator*(){
    return *(start_ + row_size_ * (pos_ % col_size_) + (pos_ / col_size_));
}

template <Numeric T, size_t col_size_, size_t row_size_>
const Matrix<T, col_size_, row_size_>::ColumnIterator::reference Matrix<T, col_size_, row_size_>::ColumnIterator::operator*() const{
    return *(start_ + row_size_ * (pos_ % col_size_) + (pos_ / col_size_));
}

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::ColumnIterator::pointer Matrix<T, col_size_, row_size_>::ColumnIterator::operator->(){
    return start_ + row_size_ * (pos_ % col_size_) + (pos_ / col_size_);
}

template <Numeric T, size_t col_size_, size_t row_size_>
const Matrix<T, col_size_, row_size_>::ColumnIterator::pointer Matrix<T, col_size_, row_size_>::ColumnIterator::operator->() const{
    return start_ + row_size_ * (pos_ % col_size_) + (pos_ / col_size_);
}

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::ColumnIterator& Matrix<T, col_size_, row_size_>::ColumnIterator::operator++(){
    ++pos_;
    return *this;
}

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::ColumnIterator Matrix<T, col_size_, row_size_>::ColumnIterator::operator++(int){
    ColumnIterator tmp{start_, pos_};
    pos_++;
    return tmp;
}

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::ColumnIterator& Matrix<T, col_size_, row_size_>::ColumnIterator::operator--(){
    --pos_;
    return *this;
}

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::ColumnIterator Matrix<T, col_size_, row_size_>::ColumnIterator::operator--(int){
    ColumnIterator tmp{start_, pos_};
    pos_--;
    return tmp;
}

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::ColumnIterator& Matrix<T, col_size_, row_size_>::ColumnIterator::operator+=(size_t n){
    pos_ += n;
    return *this;
}

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::ColumnIterator& Matrix<T, col_size_, row_size_>::ColumnIterator::operator-=(size_t n){
    pos_ -= n;
    return *this;
}

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::ColumnIterator::difference_type Matrix<T, col_size_, row_size_>::ColumnIterator::operator-(const typename Matrix<T, col_size_, row_size_>::ColumnIterator& other) const{
    return pos_ - other.pos_;
}

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::ColumnIterator Matrix<T, col_size_, row_size_>::ColumnIterator::operator+(size_t n) const{
    typename Matrix<T, col_size_, row_size_>::ColumnIterator result = *this;
    result += n;
    return result;
}

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::ColumnIterator Matrix<T, col_size_, row_size_>::ColumnIterator::operator-(size_t n) const{
    typename Matrix<T, col_size_, row_size_>::ColumnIterator result = *this;
    result -= n;
    return result;
}

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::ColumnIterator::reference Matrix<T, col_size_, row_size_>::ColumnIterator::operator[](size_t index){
    return *(start_ + row_size_ * ((pos_ + index) % col_size_) + ((pos_ + index) / col_size_));
}

template <Numeric T, size_t col_size_, size_t row_size_>
const Matrix<T, col_size_, row_size_>::ColumnIterator::reference Matrix<T, col_size_, row_size_>::ColumnIterator::operator[](size_t index) const{
    return *(start_ + row_size_ * ((pos_ + index) % col_size_) + ((pos_ + index) / col_size_));
}


static_assert(std::random_access_iterator<Matrix<int, 4, 5>::ColumnIterator>);


#endif