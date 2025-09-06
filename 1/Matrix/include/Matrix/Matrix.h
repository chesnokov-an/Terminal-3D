#ifndef MATRIX_H
#define MATRIX_H

template <typename T>
concept Numeric = std::is_integral_v<T> || std::is_floating_point_v<T>;

template <Numeric T>
class Col_iterator;

template <Numeric T, size_t col_size_, size_t row_size_>
class Matrix{
private:
    T matrix_[col_size_][row_size_]{};

public:
    using iterator = T*;
    iterator begin();
    iterator end();
    const iterator cbegin() const;
    const iterator cend() const;
    std::reverse_iterator<T*> rbegin();
    std::reverse_iterator<T*> rend();
    const std::reverse_iterator<T*> crbegin() const;
    const std::reverse_iterator<T*> crend() const;

    Col_iterator<T> col_begin();
    Col_iterator<T> col_end();

    std::pair<T*, T*> row_iters(size_t i);
    std::pair<Col_iterator<T>, Col_iterator<T>> col_iters(size_t j);

public:
    Matrix() = default;
    Matrix(const Matrix& other) = default;
    Matrix(T value);
    Matrix(iterator begin, iterator end);
    Matrix(std::initializer_list<std::initializer_list<T>> init);
    
    Matrix& operator=(const Matrix& other) & = default;
    Matrix& operator=(std::initializer_list<std::initializer_list<T>> init);
    Matrix& operator+=(const Matrix& other);
    Matrix& operator-=(const Matrix& other);
    
    size_t get_col() const;
    size_t get_row() const;
    size_t get_size() const;
    T& at(size_t i, size_t j);
    const T& at(size_t i, size_t j) const;

    void set(size_t i, size_t j, T value);

    Matrix& transposed() const;

};

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_> operator+(const Matrix<T, col_size_, row_size_>& a, const Matrix<T, col_size_, row_size_>& b);

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_> operator-(const Matrix<T, col_size_, row_size_>& a, const Matrix<T, col_size_, row_size_>& b);

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_> operator*(const Matrix<T, col_size_, row_size_>& a, const Matrix<T, col_size_, row_size_>& b);

template <Numeric T>
class Сol_iterator{
private:
    Matrix* matrix_ = nullptr;
    T* ptr_ = nullptr;

public:
    Сol_iterator() = delete;
    Сol_iterator(const Col_iterator& other) : matrix_(other.matrix_), ptr_(other.ptr_){}
    Сol_iterator(Matrix* matrix, T* ptr) : matrix_(matrix), ptr_(ptr){}

    Сol_iterator& operator++();
    Сol_iterator operator++(int);
    Сol_iterator& operator--();
    Сol_iterator operator--(int);
    Сol_iterator& operator=(const Сol_iterator& other) & = default;
    Сol_iterator& operator+=(const size_t n);
    Сol_iterator& operator-=(const size_t n);
    bool operator==(const Сol_iterator& other) const;
    bool operator!=(const Сol_iterator& other) const;
    bool operator>(const Сol_iterator& other) const;
    bool operator<(const Сol_iterator& other) const;
    bool operator>=(const Сol_iterator& other) const;
    bool operator<=(const Сol_iterator& other) const;
    T& operator[](size_t index);
};

template <Numeric T>
Сol_iterator<T> operator+(Сol_iterator<T> it, const T n);

template <Numeric T>
Сol_iterator<T> operator+(const T n, Сol_iterator<T> it);

template <Numeric T>
Сol_iterator<T> operator-(Сol_iterator<T> it, const T n);

template <Numeric T>
Сol_iterator<T> operator-(const T n, Сol_iterator<T> it);


#endif