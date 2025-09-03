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
    Matrix& operator=(Matrix other);
    size_t get_col() const;
    size_t get_row() const;
    size_t get_size() const;
};

#endif