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
    iterator begin();
    iterator end();
    const iterator cbegin() const;
    const iterator cend() const;
    std::reverse_iterator<T*> rbegin();
    std::reverse_iterator<T*> rend();
    const std::reverse_iterator<T*> crbegin() const;
    const std::reverse_iterator<T*> crend() const;

public:
    Matrix() = default;
    Matrix(T value);
    Matrix(iterator begin, iterator end);
    Matrix(std::initializer_list<std::initializer_list<T>> list);
    
    Matrix& operator=(const Matrix& other) = default;
    Matrix& operator=(std::initializer_list<std::initializer_list<T>> list);
    Matrix& operator+=(const Matrix& other);
    Matrix& operator-=(const Matrix& other);
    
    size_t get_col() const;
    size_t get_row() const;
    size_t get_size() const;
    T& at(size_t i, size_t j);
    const T& at(size_t i, size_t j) const;

    void set(size_t i, size_t j, T value);

    Matrix& get_transposed() const;

};

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_> operator+(const Matrix<T, col_size_, row_size_>& a, const Matrix<T, col_size_, row_size_>& b);

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_> operator-(const Matrix<T, col_size_, row_size_>& a, const Matrix<T, col_size_, row_size_>& b);

#endif