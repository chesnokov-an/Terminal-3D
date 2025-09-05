#ifndef MATRIX_H
#define MATRIX_H

template <typename T>
concept Numeric = std::is_integral_v<T> || std::is_floating_point_v<T>;

template <Numeric T, size_t col_size_, size_t row_size_>
class Matrix{
private:
    T matrix_[col_size_][row_size_]{};

public:
    Matrix() = default;
    Matrix(T value);
    Matrix(T* begin, T* end);
    Matrix(std::initializer_list<std::initializer_list<T>> list);
    size_t get_col() const;
    size_t get_row() const;
    size_t get_size() const;
    T& at(size_t i, size_t j);
    const T& at(size_t i, size_t j) const;
    Matrix& operator=(const Matrix& other) = default;
    //Matrix& operator=(std::initializer_list<std::initializer_list<T>> list);
    Matrix& operator+=(const Matrix& other);
    Matrix& operator-=(const Matrix& other);
    

public:
    using iterator = T**;
    iterator begin() const;
    iterator end() const;
    iterator cbegin() const;
    iterator cend() const;
    iterator rbegin() const;
    iterator rend() const;
    iterator crbegin() const;
    iterator crend() const;
};

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_> operator+(const Matrix<T, col_size_, row_size_>& a, const Matrix<T, col_size_, row_size_>& b);

template <Numeric T, size_t col_size_, size_t row_size_>
Matrix<T, col_size_, row_size_> operator-(const Matrix<T, col_size_, row_size_>& a, const Matrix<T, col_size_, row_size_>& b);

#endif