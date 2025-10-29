/**
 * @file Matrix.h
 * @brief Template class for mathematical matrix operations with comprehensive iterator support
 * @author Chesnokov Alexandr
 * @date 2025
 * @version 1.0
 * 
 * This header defines a template Matrix class with fixed dimensions, supporting various
 * mathematical operations, iterators for row-major and column-major traversal, and
 * common matrix operations like transposition and multiplication.
 */

#ifndef MATRIX_H
#define MATRIX_H

#include <cstddef>
#include <utility>
#include <iterator>
#include <algorithm>
#include <numeric>


namespace MatrixNameSpace {
    
    /**
     * @concept Numeric
     * @brief Concept that checks if a type is arithmetic (numeric)
     * @tparam T Type to check for arithmetic properties
     * 
     * This concept is used to constrain template parameters to numeric types only,
     * ensuring type safety in mathematical operations.
     */
    template <typename T>
    concept Numeric = std::is_arithmetic_v<T>;

    /**
     * @class Matrix
     * @brief Template class representing a mathematical matrix with fixed dimensions
     * @tparam T Numeric type of matrix elements (must satisfy Numeric concept)
     * @tparam col_size_ Number of columns in the matrix (compile-time constant)
     * @tparam row_size_ Number of rows in the matrix (compile-time constant)
     * 
     * The Matrix class provides a comprehensive implementation of matrix operations
     * including addition, subtraction, multiplication, transposition, and various
     * iterator types for different traversal patterns.
     */
    template <Numeric T, size_t col_size_, size_t row_size_>
    class Matrix{
    private:
        T matrix_[col_size_][row_size_]{}; ///< Internal 2D array storage for matrix elements

    public:
        // Iterator type definitions
        using iterator = T*; ///< Random access iterator type for row-major traversal
        using const_iterator = const T*; ///< Constant random access iterator type
        using reverse_iterator = std::reverse_iterator<iterator>; ///< Reverse iterator type
        using const_reverse_iterator = std::reverse_iterator<const_iterator>; ///< Constant reverse iterator type

        /**
         * @class ColumnIterator
         * @brief Iterator for column-major traversal of the matrix
         * @tparam Const Boolean flag indicating whether iterator is constant (true) or mutable (false)
         */
        template <bool Const>
        class ColumnIterator;

        using column_iterator = ColumnIterator<false>; ///< Non-const column-major iterator
        using const_column_iterator = ColumnIterator<true>; ///< Const column-major iterator
        
        // Row-major iterator methods
        iterator begin(); ///< Returns iterator to the first element (row-major order)
        iterator end(); ///< Returns iterator to the element after the last (row-major order)
        const_iterator begin() const; ///< Returns const iterator to the first element
        const_iterator end() const; ///< Returns const iterator to the element after the last
        const_iterator cbegin() const; ///< Returns const iterator to the first element
        const_iterator cend() const; ///< Returns const iterator to the element after the last

        // Reverse iterator methods
        reverse_iterator rbegin(); ///< Returns reverse iterator to the last element
        reverse_iterator rend(); ///< Returns reverse iterator to the element before the first
        const_reverse_iterator rbegin() const; ///< Returns const reverse iterator to the last element
        const_reverse_iterator rend() const; ///< Returns const reverse iterator to the element before the first
        const_reverse_iterator crbegin() const; ///< Returns const reverse iterator to the last element
        const_reverse_iterator crend() const; ///< Returns const reverse iterator to the element before the first

        // Column iterator methods
        column_iterator col_begin(); ///< Returns column iterator to the first element (column-major)
        const_column_iterator col_begin() const; ///< Returns const column iterator to the first element
        const_column_iterator col_cbegin() const; ///< Returns const column iterator to the first element
        column_iterator col_end(); ///< Returns column iterator to the element after the last
        const_column_iterator col_end() const; ///< Returns const column iterator to the element after the last
        const_column_iterator col_cend() const; ///< Returns const column iterator to the element after the last

        // Row and column range methods
        std::pair<iterator, iterator> row_iters(size_t i); ///< Returns iterators for specified row
        std::pair<const_iterator, const_iterator> row_iters(size_t i) const; ///< Returns const iterators for specified row
        std::pair<ColumnIterator<false>, ColumnIterator<false>> col_iters(size_t j); ///< Returns column iterators for specified column
        std::pair<ColumnIterator<true>, ColumnIterator<true>> col_iters(size_t j) const; ///< Returns const column iterators for specified column

    public:
        // Constructors and assignment operators
        Matrix() = default; ///< Default constructor (initializes all elements to default value of T)
        Matrix(const Matrix& other) = default; ///< Copy constructor

        /**
         * @brief Constructor that fills matrix with a specified value
         * @param value The value to fill all matrix elements with
         */
        Matrix(T value);

        /**
         * @brief Constructor from iterator range
         * @tparam InputIter Input iterator type
         * @param begin Iterator to the first element to copy
         * @param end Iterator to the element after the last to copy
         * @throws std::out_of_range if the range size doesn't match matrix size
         */
        template <typename InputIter>
        Matrix(InputIter begin, InputIter end);

        /**
         * @brief Constructor from initializer list
         * @param init Initializer list containing matrix elements in row-major order
         * @throws std::out_of_range if initializer list size doesn't match matrix size
         */
        Matrix(std::initializer_list<T> init);
        
        // Assignment operators
        Matrix& operator=(const Matrix& other) = default; ///< Copy assignment operator

        /**
         * @brief Assignment from initializer list
         * @param init Initializer list containing new matrix values
         * @return Reference to this matrix
         * @throws std::out_of_range if initializer list size doesn't match matrix size
         */
        Matrix& operator=(std::initializer_list<T> init);

        /**
         * @brief Matrix addition assignment operator
         * @param other Matrix to add to this matrix
         * @return Reference to this matrix after addition
         * @throws std::invalid_argument if matrix dimensions don't match
         */
        Matrix& operator+=(const Matrix& other);

        /**
         * @brief Matrix subtraction assignment operator
         * @param other Matrix to subtract from this matrix
         * @return Reference to this matrix after subtraction
         * @throws std::invalid_argument if matrix dimensions don't match
         */
        Matrix& operator-=(const Matrix& other);

        // Element access operators
        /**
         * @brief 2D element access operator (mutable)
         * @param i Row index (0-based)
         * @param j Column index (0-based)
         * @return Reference to the element at position (i, j)
         * @throws std::out_of_range if indices are out of bounds
         */
        T& operator[](size_t i, size_t j);

        /**
         * @brief 2D element access operator (const)
         * @param i Row index (0-based)
         * @param j Column index (0-based)
         * @return Const reference to the element at position (i, j)
         * @throws std::out_of_range if indices are out of bounds
         */
        T operator[](size_t i, size_t j) const;
        
        // Size information methods (compile-time)
        /**
         * @brief Get number of columns in the matrix
         * @return Number of columns (compile-time constant)
         */
        consteval size_t get_col_size() const;

        /**
         * @brief Get number of rows in the matrix
         * @return Number of rows (compile-time constant)
         */
        consteval size_t get_row_size() const;

        /**
         * @brief Get total number of elements in the matrix
         * @return Total element count (compile-time constant)
         */
        consteval size_t get_size() const;

        // Matrix operations
        /**
         * @brief Fill all matrix elements with a specified value
         * @param value The value to fill the matrix with
         */
        void fill(T value);
        
        /**
         * @brief Create and return the transposed matrix
         * @return New matrix that is the transpose of this matrix
         */
        Matrix<T, row_size_, col_size_> transposed() const;

        /**
         * @class ColumnIterator
         * @brief Random access iterator for column-major traversal of the matrix
         * @tparam Const Boolean flag indicating constness of the iterator
         */
        template <bool Const>
        class ColumnIterator{
        public:
            using difference_type = std::ptrdiff_t; ///< Type for iterator differences
            using value_type = T; ///< Type of values pointed to
            using pointer = std::conditional_t<Const, const value_type*, value_type*>; ///< Pointer type (const or mutable)
            using reference = std::conditional_t<Const, const value_type&, value_type&>; ///< Reference type (const or mutable)
            using iterator_category = std::random_access_iterator_tag; ///< Iterator category
        private:
            pointer start_ = nullptr; ///< Pointer to the start of the matrix data
            size_t pos_ = 0; ///< Current position in column-major order

        public:
            ColumnIterator() = default; ///< Default constructor
            ColumnIterator(const ColumnIterator& other) = default; ///< Copy constructor
            /**
             * @brief Parameterized constructor
             * @param start Pointer to matrix data start
             * @param pos Initial position in column-major order
             */
            ColumnIterator(pointer start, size_t pos) : start_(start), pos_(pos){}

            friend class ColumnIterator<true>;
            friend class ColumnIterator<false>;

            /**
             * @brief Conversion operator from mutable to const iterator
             * @tparam C Constness template parameter (automatically deduced)
             * @return Const column iterator equivalent to this iterator
             */
            template <bool C = Const, typename = std::enable_if_t<!C>>
            operator ColumnIterator<true>() const{
                return ColumnIterator<true>(start_, pos_);
            }

            // Iterator operations
            reference operator*() const; ///< Dereference operator
            pointer operator->() const; ///< Member access operator
            ColumnIterator& operator++(); ///< Prefix increment operator
            ColumnIterator operator++(int); ///< Postfix increment operator
            ColumnIterator& operator--(); ///< Prefix decrement operator
            ColumnIterator operator--(int); ///< Postfix decrement operator
            ColumnIterator& operator=(const ColumnIterator& other) & = default; ///< Assignment operator
            ColumnIterator& operator+=(size_t n); ///< Addition assignment operator
            ColumnIterator& operator-=(size_t n); ///< Subtraction assignment operator
            difference_type operator-(const ColumnIterator& other) const; ///< Difference operator
            bool operator==(const ColumnIterator& other) const = default; ///< Equality operator
            std::strong_ordering operator<=>(const ColumnIterator& other) const = default; ///< Comparison operator
            reference operator[](size_t index) const; ///< Subscript operator

            ColumnIterator operator+(size_t n) const; ///< Addition operator
            /// Friend addition operator for column iterator
            friend ColumnIterator operator+(size_t n, const ColumnIterator& it){
                return it + n;
            }
            ColumnIterator operator-(size_t n) const; ///< Subtraction operator
            /// Friend subtraction operator for column iterator
            friend ColumnIterator operator-(size_t n, ColumnIterator& it){
                return it - n;
            }
        };
    };

    // Free function operators
    /**
     * @brief Matrix addition operator
     * @tparam T Numeric type of matrix elements
     * @tparam col_size_ Number of columns
     * @tparam row_size_ Number of rows
     * @param a First matrix operand
     * @param b Second matrix operand
     * @return New matrix containing element-wise sum
     * @throws std::invalid_argument if matrix dimensions don't match
     */
    template <Numeric T, size_t col_size_, size_t row_size_>
    Matrix<T, col_size_, row_size_> operator+(const Matrix<T, col_size_, row_size_>& a, const Matrix<T, col_size_, row_size_>& b);

    /**
     * @brief Matrix subtraction operator
     * @tparam T Numeric type of matrix elements
     * @tparam col_size_ Number of columns
     * @tparam row_size_ Number of rows
     * @param a First matrix operand
     * @param b Second matrix operand
     * @return New matrix containing element-wise difference
     * @throws std::invalid_argument if matrix dimensions don't match
     */
    template <Numeric T, size_t col_size_, size_t row_size_>
    Matrix<T, col_size_, row_size_> operator-(const Matrix<T, col_size_, row_size_>& a, const Matrix<T, col_size_, row_size_>& b);

    /**
     * @brief Matrix multiplication operator
     * @tparam T Numeric type of first matrix elements
     * @tparam U Numeric type of second matrix elements
     * @tparam M Rows in first matrix
     * @tparam N Columns in first matrix (must equal rows in second matrix)
     * @tparam P Columns in second matrix
     * @param a First matrix operand (M x N)
     * @param b Second matrix operand (N x P)
     * @return New matrix containing matrix product (M x P)
     * @throws std::invalid_argument if inner dimensions don't match (N must be equal)
     */
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
    Matrix<T, col_size_, row_size_>::column_iterator Matrix<T, col_size_, row_size_>::col_begin(){
        ColumnIterator<false> col_it{begin(), 0};
        return col_it;
    }

    template <Numeric T, size_t col_size_, size_t row_size_>
    Matrix<T, col_size_, row_size_>::const_column_iterator Matrix<T, col_size_, row_size_>::col_begin() const{
        const ColumnIterator<true> col_it{begin(), 0};
        return col_it;
    }

    template <Numeric T, size_t col_size_, size_t row_size_>
    Matrix<T, col_size_, row_size_>::const_column_iterator Matrix<T, col_size_, row_size_>::col_cbegin() const{
        ColumnIterator<true> col_it{begin(), 0};
        return col_it;
    }

    template <Numeric T, size_t col_size_, size_t row_size_>
    Matrix<T, col_size_, row_size_>::column_iterator Matrix<T, col_size_, row_size_>::col_end(){
        ColumnIterator<false> col_it{begin(), col_size_ * row_size_};
        return col_it;
    }

    template <Numeric T, size_t col_size_, size_t row_size_>
    Matrix<T, col_size_, row_size_>::const_column_iterator Matrix<T, col_size_, row_size_>::col_end() const{
        const ColumnIterator<true> col_it{begin(), col_size_ * row_size_};
        return col_it;
    }

    template <Numeric T, size_t col_size_, size_t row_size_>
    Matrix<T, col_size_, row_size_>::const_column_iterator Matrix<T, col_size_, row_size_>::col_cend() const{
        ColumnIterator<true> col_it{begin(), col_size_ * row_size_};
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
        return *this;
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
    consteval size_t Matrix<T, col_size_, row_size_>::get_col_size() const{
        return col_size_;
    }

    template <Numeric T, size_t col_size_, size_t row_size_>
    consteval size_t Matrix<T, col_size_, row_size_>::get_row_size() const{
        return row_size_;
    }

    template <Numeric T, size_t col_size_, size_t row_size_>
    consteval size_t Matrix<T, col_size_, row_size_>::get_size() const{
        return col_size_ * row_size_;
    }

    /*----------------MAIN FUNCTIONS----------------*/
    template <Numeric T, size_t col_size_, size_t row_size_>
    void Matrix<T, col_size_, row_size_>::fill(T value){
        std::fill(begin(), end(), value);
    }

    template <Numeric T, size_t col_size_, size_t row_size_>
    Matrix<T, row_size_, col_size_> Matrix<T, col_size_, row_size_>::transposed() const{
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
        ColumnIterator tmp = *this;
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
}

#endif