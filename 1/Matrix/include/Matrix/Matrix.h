#ifndef MATRIX_H
#define MATRIX_H


#include <type_traits>
#include <cstddef>
#include <utility>
#include <iterator>

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

    // const перегрузки
    ColumnIterator col_begin();
    ColumnIterator col_end();

    std::pair<iterator, iterator> row_iters(size_t i);
    std::pair<ColumnIterator, ColumnIterator> col_iters(size_t j);

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

    Matrix transposed() const;

    class ColumnIterator{
    private:
        T* ptr_ = nullptr;

    public:
        using difference_type = std::ptrdiff_t;
        ColumnIterator() = delete;
        ColumnIterator(const ColumnIterator& other) = default;
        ColumnIterator(T* ptr) : ptr_(ptr){}

        ColumnIterator& operator++();
        ColumnIterator operator++(int);
        ColumnIterator& operator--();
        ColumnIterator operator--(int);
        ColumnIterator& operator=(const ColumnIterator& other) & = default;
        ColumnIterator& operator+=(const size_t n);
        ColumnIterator& operator-=(const size_t n);
        bool operator==(const ColumnIterator& other) const = default;
        std::strong_ordering operator<=>(const ColumnIterator& other) const  { return ptr_ <=> other.ptr_; };

        T& operator[](size_t index) const;
    };

    static_assert(std::random_access_iterator<iterator>);
    static_assert(std::random_access_iterator<const_iterator>);
};

// std::iterator_traits<int*>::
template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_> operator+(const Matrix<T, col_size_, row_size_>& a, const Matrix<T, col_size_, row_size_>& b);

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_> operator-(const Matrix<T, col_size_, row_size_>& a, const Matrix<T, col_size_, row_size_>& b);

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_> operator*(const Matrix<T, col_size_, row_size_>& a, const Matrix<T, col_size_, row_size_>& b);

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::ColumnIterator operator+(typename Matrix<T, col_size_, row_size_>::ColumnIterator it, const T n);

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::ColumnIterator operator+(const T n, typename Matrix<T, col_size_, row_size_>::ColumnIterator it);

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::ColumnIterator operator-(typename Matrix<T, col_size_, row_size_>::ColumnIterator it, const T n);

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_>::ColumnIterator operator-(const T n, typename Matrix<T, col_size_, row_size_>::ColumnIterator it);

// static_assert(std::random_access_iterator<int>);


#endif