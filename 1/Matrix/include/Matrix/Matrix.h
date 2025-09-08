#ifndef MATRIX_H
#define MATRIX_H

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
    class column_iterator;
    
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
    column_iterator col_begin();
    column_iterator col_end();

    std::pair<iterator, iterator> row_iters(size_t i);
    std::pair<column_iterator, column_iterator> col_iters(size_t j);

public:
    Matrix() = default;
    Matrix(const Matrix& other) = default;
    Matrix(T value);

    Matrix(iterator begin, iterator end);
    Matrix(std::initializer_list<T> init);
    
    Matrix& operator=(const Matrix& other) & = default;
    Matrix& operator=(std::initializer_list<std::initializer_list<T>> init);
    Matrix& operator+=(const Matrix& other);
    Matrix& operator-=(const Matrix& other);
    
    size_t get_col() const;
    size_t get_row() const;
    size_t get_size() const;

    T& at(size_t i, size_t j);
    T at(size_t i, size_t j) const;

    void set(size_t i, size_t j, T value);

    Matrix& transposed() const;

    class column_iterator{
    private:
        T* ptr_ = nullptr;

    public:
        using difference_type = std::ptrdiff_t;
        column_iterator() = delete;
        column_iterator(const column_iterator& other) = default;
        column_iterator(T* ptr) : matrix_(matrix), ptr_(ptr){}

        column_iterator& operator++();
        column_iterator operator++(int);
        column_iterator& operator--();
        column_iterator operator--(int);
        column_iterator& operator=(const column_iterator& other) & = default;
        column_iterator& operator+=(const size_t n);
        column_iterator& operator-=(const size_t n);
        bool operator==(const column_iterator& other) const = default;
        std::strong_ordering operator<=>(const column_iterator& other) const  { return ptr_ <=> other.ptr_};

        T& operator[](size_t index) const;
    };

    static_assert(std::random_access_iterator<iterator>);
    static_assert(std::random_access_iterator<const_iterator>);


    column_iterator operator+(column_iterator it, const T n);
    column_iterator operator+(const T n, column_iterator it);
    column_iterator operator-(column_iterator it, const T n);
    column_iterator operator-(const T n, column_iterator it);
};

// std::iterator_traits<int*>::
template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_> operator+(const Matrix<T, col_size_, row_size_>& a, const Matrix<T, col_size_, row_size_>& b);

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_> operator-(const Matrix<T, col_size_, row_size_>& a, const Matrix<T, col_size_, row_size_>& b);

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_> operator*(const Matrix<T, col_size_, row_size_>& a, const Matrix<T, col_size_, row_size_>& b);


// static_assert(std::random_access_iterator<int>);



#endif