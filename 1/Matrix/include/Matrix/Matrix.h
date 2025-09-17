#ifndef MATRIX_H
#define MATRIX_H

#include <cstddef>
#include <utility>
#include <iterator>
#include <algorithm>
#include <numeric>

template <typename T>
concept Numeric = std::is_arithmetic_v<T>;

template <Numeric T, size_t col_size_, size_t row_size_>
class Matrix{
private:
    T matrix_[col_size_][row_size_]{};

public:
    using iterator = T*;
    using const_iterator = const T*;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    template <bool Const>
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

    ColumnIterator<false> col_begin();
    ColumnIterator<true> col_cbegin() const;
    ColumnIterator<true> col_begin() const;
    ColumnIterator<false> col_end();
    ColumnIterator<true> col_end() const;

    std::pair<iterator, iterator> row_iters(size_t i);
    std::pair<const_iterator, const_iterator> row_iters(size_t i) const;
    std::pair<ColumnIterator<false>, ColumnIterator<false>> col_iters(size_t j);
    std::pair<ColumnIterator<true>, ColumnIterator<true>> col_iters(size_t j) const;

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

    T& operator[](size_t i, size_t j);
    T operator[](size_t i, size_t j) const;
    
    size_t get_col_size() const;
    size_t get_row_size() const;
    size_t get_size() const;

    void fill(T value);
    Matrix transposed() const;

    template <bool Const>
    class ColumnIterator{
    public:
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = std::conditional_t<Const, const value_type*, value_type*>;
        using reference = std::conditional_t<Const, const value_type&, value_type&>;
        using iterator_category = std::random_access_iterator_tag;
    private:
        pointer start_ = nullptr;
        size_t pos_ = 0;

    public:
        ColumnIterator() = default;
        ColumnIterator(const ColumnIterator& other) = default;
        ColumnIterator(pointer start, size_t pos) : start_(start), pos_(pos){}

        friend class ColumnIterator<true>;
        friend class ColumnIterator<false>;

        template <bool C = Const, typename = std::enable_if_t<!C>>
        operator ColumnIterator<true>() const{
            return ColumnIterator<true>(start_, pos_);
        }

        reference operator*() const;
        pointer operator->() const;
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
        reference operator[](size_t index) const;

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

template <Numeric T, Numeric U, size_t M, size_t N, size_t P>
Matrix<T, M, P> operator*(const Matrix<T, M, N>& a, const Matrix<U, N, P>& b);

/****************Realization****************/
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
Matrix<T, col_size_, row_size_>::ColumnIterator<false> Matrix<T, col_size_, row_size_>::col_begin(){
    ColumnIterator<false> col_it{begin(), 0};
    return col_it;
}

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::ColumnIterator<true> Matrix<T, col_size_, row_size_>::col_cbegin() const{
    ColumnIterator<true> col_it{begin(), 0};
    return col_it;
}

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::ColumnIterator<true> Matrix<T, col_size_, row_size_>::col_begin() const{
    const ColumnIterator<true> col_it{begin(), 0};
    return col_it;
}

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::ColumnIterator<false> Matrix<T, col_size_, row_size_>::col_end(){
    ColumnIterator<false> col_it{begin(), col_size_ * row_size_};
    return col_it;
}

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::ColumnIterator<true> Matrix<T, col_size_, row_size_>::col_end() const{
    const ColumnIterator<true> col_it{begin(), col_size_ * row_size_};
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
std::pair<typename Matrix<T, col_size_, row_size_>::ColumnIterator<false>, typename Matrix<T, col_size_, row_size_>::ColumnIterator<false>> Matrix<T, col_size_, row_size_>::col_iters(size_t j){
    return std::pair<ColumnIterator<false>, ColumnIterator<false>>(col_begin() + j * col_size_, col_begin() + (j + 1) * col_size_);
}

template <Numeric T, size_t col_size_, size_t row_size_>
std::pair<typename Matrix<T, col_size_, row_size_>::ColumnIterator<true>, typename Matrix<T, col_size_, row_size_>::ColumnIterator<true>> Matrix<T, col_size_, row_size_>::col_iters(size_t j) const{
    return std::pair<ColumnIterator<true>, ColumnIterator<true>>(col_begin() + j * col_size_, col_begin() + (j + 1) * col_size_);
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
Matrix<T, col_size_, row_size_>::Matrix(std::initializer_list<T> init) : Matrix(init.begin(), init.end()){}

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
T& Matrix<T, col_size_, row_size_>::operator[](size_t i, size_t j){
    return matrix_[i][j];
}

template <Numeric T, size_t col_size_, size_t row_size_>
T Matrix<T, col_size_, row_size_>::operator[](size_t i, size_t j) const{
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

template <Numeric T, Numeric U, size_t M, size_t N, size_t P>
Matrix<T, M, P> operator*(const Matrix<T, M, N>& a, const Matrix<U, N, P>& b){
    Matrix<U, P, N> b_transposed = b.transposed();
    Matrix<T, M, P> result;
    
    for(size_t i = 0; i < M * P; i++){
        size_t res_row = i / P;
        size_t res_col = i % P;

        auto a_row = a.row_iters(res_row).first;
        auto b_row = b_transposed.row_iters(res_col).first;

        result[res_row, res_col] = std::inner_product(a_row, a_row + N, b_row, 0);
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
void Matrix<T, col_size_, row_size_>::fill(T value){
    std::fill(begin(), end(), value);
}

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_> Matrix<T, col_size_, row_size_>::transposed() const{
    Matrix<T, row_size_, col_size_> result;
    std::copy(begin(), end(), result.col_begin());
    return result;
}

/*----------------COL ITERATOR----------------*/
/*----------------OPERATORS----------------*/
template<Numeric T, size_t col_size_, size_t row_size_>
template<bool Const>
Matrix<T, col_size_, row_size_>::ColumnIterator<Const>::reference Matrix<T, col_size_, row_size_>::ColumnIterator<Const>::operator*() const{
    return *(start_ + row_size_ * (pos_ % col_size_) + (pos_ / col_size_));
}

template <Numeric T, size_t col_size_, size_t row_size_>
template <bool Const>
Matrix<T, col_size_, row_size_>::ColumnIterator<Const>::pointer Matrix<T, col_size_, row_size_>::ColumnIterator<Const>::operator->() const{
    return start_ + row_size_ * (pos_ % col_size_) + (pos_ / col_size_);
}

template <Numeric T, size_t col_size_, size_t row_size_>
template <bool Const>
Matrix<T, col_size_, row_size_>::ColumnIterator<Const>& Matrix<T, col_size_, row_size_>::ColumnIterator<Const>::operator++(){
    ++pos_;
    return *this;
}

template <Numeric T, size_t col_size_, size_t row_size_>
template <bool Const>
Matrix<T, col_size_, row_size_>::ColumnIterator<Const> Matrix<T, col_size_, row_size_>::ColumnIterator<Const>::operator++(int){
    ColumnIterator tmp = *this;
    pos_++;
    return tmp;
}

template <Numeric T, size_t col_size_, size_t row_size_>
template <bool Const>
Matrix<T, col_size_, row_size_>::ColumnIterator<Const>& Matrix<T, col_size_, row_size_>::ColumnIterator<Const>::operator--(){
    --pos_;
    return *this;
}

template <Numeric T, size_t col_size_, size_t row_size_>
template <bool Const>
Matrix<T, col_size_, row_size_>::ColumnIterator<Const> Matrix<T, col_size_, row_size_>::ColumnIterator<Const>::operator--(int){
    ColumnIterator tmp{start_, pos_};
    pos_--;
    return tmp;
}

template <Numeric T, size_t col_size_, size_t row_size_>
template <bool Const>
Matrix<T, col_size_, row_size_>::ColumnIterator<Const>& Matrix<T, col_size_, row_size_>::ColumnIterator<Const>::operator+=(size_t n){
    pos_ += n;
    return *this;
}

template <Numeric T, size_t col_size_, size_t row_size_>
template <bool Const>
Matrix<T, col_size_, row_size_>::ColumnIterator<Const>& Matrix<T, col_size_, row_size_>::ColumnIterator<Const>::operator-=(size_t n){
    pos_ -= n;
    return *this;
}

template <Numeric T, size_t col_size_, size_t row_size_>
template <bool Const>
Matrix<T, col_size_, row_size_>::ColumnIterator<Const>::difference_type Matrix<T, col_size_, row_size_>::ColumnIterator<Const>::operator-(const typename Matrix<T, col_size_, row_size_>::ColumnIterator<Const>& other) const{
    return pos_ - other.pos_;
}

template <Numeric T, size_t col_size_, size_t row_size_>
template <bool Const>
Matrix<T, col_size_, row_size_>::ColumnIterator<Const> Matrix<T, col_size_, row_size_>::ColumnIterator<Const>::operator+(size_t n) const{
    typename Matrix<T, col_size_, row_size_>::ColumnIterator<Const> result = *this;
    result += n;
    return result;
}

template <Numeric T, size_t col_size_, size_t row_size_>
template <bool Const>
Matrix<T, col_size_, row_size_>::ColumnIterator<Const> Matrix<T, col_size_, row_size_>::ColumnIterator<Const>::operator-(size_t n) const{
    typename Matrix<T, col_size_, row_size_>::ColumnIterator<Const> result = *this;
    result -= n;
    return result;
}

template <Numeric T, size_t col_size_, size_t row_size_>
template <bool Const>
Matrix<T, col_size_, row_size_>::ColumnIterator<Const>::reference Matrix<T, col_size_, row_size_>::ColumnIterator<Const>::operator[](size_t index) const{
    return *(start_ + row_size_ * ((pos_ + index) % col_size_) + ((pos_ + index) / col_size_));
}


// static_assert(std::random_access_iterator<Matrix<int, 4, 5>::ColumnIterator<true>>);


#endif