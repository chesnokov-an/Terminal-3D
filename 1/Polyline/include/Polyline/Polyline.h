/**
 * @file Polyline.h
 * @brief 3D polyline class with geometric transformations and operations
 * @author Chesnokov Alexandr
 * @date 2025
 * @version 1.0
 * 
 * This header defines a Polyline class for representing 3D polylines with support
 * for various geometric transformations, point management, and mathematical operations.
 */

#ifndef POLYLINE_H
#define POLYLINE_H

#include <concepts>
#include <array>
#include <cstddef>
#include <cmath>
#include <numbers>
#include <cstring>
#include <Matrix/Matrix.h>

namespace PolylineNameSpace {
    using namespace MatrixNameSpace;
    /**
     * @class Point
     * @brief 3D point with coordinates and character label
     * @tparam T Numeric type of coordinates (must satisfy Numeric concept)
     * 
     * Represents a point in 3D space with x, y, z coordinates and a character
     * label for identification in rendering.
     */
    template <Numeric T>
    class Point{
    public:
        T x = 0; ///< X coordinate (horizontal position)
        T y = 0; ///< Y coordinate (depth position)  
        T z = 0; ///< Z coordinate (vertical position)
        char name_ = '*'; ///< Character label for point identification

        /**
         * @brief Calculates Euclidean distance to another point
         * @param other Other point to calculate distance to (Point<T>)
         * @return double Euclidean distance between the two points
         * 
         * Formula: sqrt((other.x - x)² + (other.y - y)² + (other.z - z)²)
         */
        double distance(const Point& other) const;
    };

    /**
     * @brief Converts Point to 1x3 Matrix for mathematical operations
     * @tparam T Numeric type of coordinates (must satisfy Numeric concept)
     * @param point Point object to convert
     * @return Matrix<T, 1, 3> containing [x, y, z] coordinates
     */
    template <Numeric T>
    Matrix<T, 1, 3> get_matrix_from_point(const Point<T>& point){
        return Matrix<T, 1, 3>{point.x, point.y, point.z};
    }

    /**
     * @brief Converts 1x3 Matrix back to Point object
     * @tparam T Numeric type of coordinates (must satisfy Numeric concept)
     * @param matrix 1x3 Matrix containing [x, y, z] coordinates
     * @param name Character label for the new point
     * @return Point<T> with coordinates from matrix and specified name
     */
    template <Numeric T>
    Point<T> get_point_from_matrix(const Matrix<T, 1, 3>& matrix, char name){
        return Point<T>{matrix[0, 0], matrix[0, 1], matrix[0, 2], name};
    }

    /**
     * @class Polyline
     * @brief 3D polyline composed of connected points with geometric operations
     * @tparam T Numeric type of point coordinates (must satisfy Numeric concept)
     * 
     * The Polyline class represents a sequence of connected 3D points with support
     * for various geometric transformations, point management, and mathematical operations.
     * Uses dynamic memory allocation for point storage.
     */
    template <Numeric T>
    class Polyline{
    private:
        Point<T>* dots_ = nullptr; ///< Dynamic array storing the polyline points
        size_t capacity_ = 0; ///< Current capacity of the dynamic array
        size_t size_ = 0; ///< Current number of points in the polyline

    public:
        // Iterator type definitions
        using iterator = Point<T>*; ///< Random access iterator type for point access
        using const_iterator = const Point<T>*; ///< Constant random access iterator type
        using reverse_iterator = std::reverse_iterator<iterator>; ///< Reverse iterator type
        using const_reverse_iterator = std::reverse_iterator<const_iterator>; ///< Constant reverse iterator type
        
        // Iterator methods
        iterator begin(); ///< Returns iterator to the first point
        iterator end(); ///< Returns iterator to the element after the last point
        const_iterator begin() const; ///< Returns const iterator to the first point
        const_iterator end() const; ///< Returns const iterator to the element after the last point
        const_iterator cbegin() const; ///< Returns const iterator to the first point
        const_iterator cend() const; ///< Returns const iterator to the element after the last point
        reverse_iterator rbegin(); ///< Returns reverse iterator to the last point
        reverse_iterator rend(); ///< Returns reverse iterator to the element before the first point
        const_reverse_iterator rbegin() const; ///< Returns const reverse iterator to the last point
        const_reverse_iterator rend() const; ///< Returns const reverse iterator to the element before the first point
        const_reverse_iterator crbegin() const; ///< Returns const reverse iterator to the last point
        const_reverse_iterator crend() const; ///< Returns const reverse iterator to the element before the first point

    public:
    // Constructors and destructor
        Polyline() = default; ///< Default constructor (empty polyline)

        /**
         * @brief Copy constructor
         * @param other Polyline to copy from
         * 
         * Creates a deep copy of the other polyline with separate memory allocation.
         */
        Polyline(const Polyline& other) : dots_(new Point<T>[other.capacity_]), capacity_(other.capacity_), size_(other.size_){
            std::copy(other.dots_, other.dots_ + size_, dots_);
        }

        /**
         * @brief Move constructor
         * @param other Polyline to move from
         * 
         * Transfers ownership of the other polyline's resources efficiently.
         */
        Polyline(Polyline&& other){
            swap(other);
        }

        /**
         * @brief Copy/move assignment operator
         * @param other Polyline to assign from (copied or moved)
         * @return Reference to this polyline after assignment
         * 
         * Uses copy-and-swap idiom for exception safety.
         */
        Polyline& operator=(Polyline other);

        /**
         * @brief Point access operator (mutable)
         * @param i Index of the point to access (0-based)
         * @return Reference to the point at index i
         * @throws std::out_of_range if index is out of bounds
         */
        Point<T>& operator[](size_t i);

        /**
         * @brief Point access operator (const)
         * @param i Index of the point to access (0-based)
         * @return Const reference to the point at index i
         * @throws std::out_of_range if index is out of bounds
         */
        const Point<T>& operator[](size_t i) const;

        /**
         * @brief Swap contents with another polyline
         * @param other Polyline to swap with
         * 
         * Efficiently exchanges the contents of two polylines.
         */
        void swap(Polyline& other);

        /**
         * @brief Destructor
         * 
         * Releases all allocated memory for points.
         */
        ~Polyline();

        // Capacity management
        /**
         * @brief Resize the internal storage capacity
         * @param new_capacity New capacity for the point array
         * 
         * Reallocates memory to the new capacity, preserving existing points.
         * If new capacity is smaller than current size, excess points are lost.
         */
        void resize(size_t new_capacity);

        // Point operations
        /**
         * @brief Add a point to the end of the polyline
         * @param point Point object to add
         * 
         * Automatically resizes storage if necessary.
         */
        void add_point(const Point<T>& point);

        /**
         * @brief Add a point with specified coordinates and label
         * @param x X coordinate of the new point
         * @param y Y coordinate of the new point
         * @param z Z coordinate of the new point
         * @param name Character label for the new point
         * 
         * Creates a Point object and adds it to the end of the polyline.
         */
        void add_point(T x, T y, T z, char name);

        /**
         * @brief Append another polyline to this one (copy version)
         * @param polyline Polyline to append (copied)
         * 
         * Adds all points from the other polyline to the end of this one.
         */
        void add_polyline(const Polyline& polyline);

        /**
         * @brief Append another polyline to this one (move version)
         * @param polyline Polyline to append (moved)
         * 
         * Efficiently transfers points from the other polyline to this one.
         */
        void add_polyline(Polyline&& polyline);

        // Geometric transformations
        /**
         * @brief Rotate the polyline around the origin
         * @param x_degree Rotation angle around X-axis in degrees
         * @param y_degree Rotation angle around Y-axis in degrees
         * @param z_degree Rotation angle around Z-axis in degrees
         * 
         * Applies rotation matrices in Z-Y-X order (intrinsic rotations).
         * Converts degrees to radians for trigonometric calculations.
         */
        void rotate_from_origin(double x_degree, double y_degree, double z_degree);

        /**
         * @brief Rotate the polyline around an arbitrary vector
         * @param start Starting point of the rotation vector
         * @param finish Ending point of the rotation vector
         * @param degree Rotation angle in degrees
         * 
         * Uses Rodrigues' rotation formula to create rotation matrix.
         * The rotation axis is the vector from start to finish.
         */
        void rotate_by_vector(const Point<T>& start, const Point<T>& finish, double degree);

        /**
         * @brief Translate (shift) the polyline by specified amounts
         * @param x Translation amount along X-axis
         * @param y Translation amount along Y-axis
         * @param z Translation amount along Z-axis
         * 
         * Adds the translation values to all point coordinates.
         */
        void shift(double x, double y, double z);

        // Geometric properties and operations
        /**
         * @brief Calculate the total length of the polyline
         * @return double Total length (sum of distances between consecutive points)
         */
        double length() const;

        /**
         * @brief Get the number of points in the polyline
         * @return size_t Number of points
         */
        size_t points_count() const;

        /**
         * @brief Find the index of the most "distant" point
         * @return size_t Index of the point with maximum sum of distances to neighbors
         * 
         * For each interior point (not first or last), calculates:
         * distance(point[i-1], point[i]) + distance(point[i], point[i+1])
         * Returns the index with the maximum such sum.
         * Returns 0 if polyline has less than 3 points.
         */
        size_t find_distant() const;

        /**
         * @brief Remove the most "distant" point from the polyline
         * 
         * Finds the point with maximum sum of distances to neighbors and removes it.
         * Does nothing if polyline has 2 or fewer points.
         */
        void remove_distant();
    };

    /****************Realization****************/
    /*----------------POINT----------------*/
    template <Numeric T>
    double Point<T>::distance(const Point<T> &other) const{
        return std::sqrt((other.x - x)*(other.x - x) + (other.y - y)*(other.y - y) + (other.z - z)*(other.z - z));
    }

    /*----------------POLYLINE----------------*/
    /*----------------ITERATORS----------------*/
    template <Numeric T>
    Polyline<T>::iterator Polyline<T>::begin(){
        return (&dots_[0]);
    }

    template <Numeric T>
    Polyline<T>::iterator Polyline<T>::end(){
        return (&dots_[0] + size_);
    }

    template <Numeric T>
    Polyline<T>::const_iterator Polyline<T>::begin() const{
        return (&dots_[0]);
    }

    template <Numeric T>
    Polyline<T>::const_iterator Polyline<T>::end() const{
        return (&dots_[0] + size_);
    }

    template <Numeric T>
    Polyline<T>::const_iterator Polyline<T>::cbegin() const{
        return (&dots_[0]);
    }

    template <Numeric T>
    Polyline<T>::const_iterator Polyline<T>::cend() const{
        return (&dots_[0] + size_);
    }

    template <Numeric T>
    Polyline<T>::reverse_iterator Polyline<T>::rbegin(){
        return std::reverse_iterator<iterator>(end());
    }

    template <Numeric T>
    Polyline<T>::reverse_iterator Polyline<T>::rend(){
        return std::reverse_iterator<iterator>(begin());
    }

    template <Numeric T>
    Polyline<T>::const_reverse_iterator Polyline<T>::rbegin() const{
        return std::reverse_iterator<const_iterator>(end());
    }

    template <Numeric T>
    Polyline<T>::const_reverse_iterator Polyline<T>::rend() const{
        return std::reverse_iterator<const_iterator>(begin());
    }

    template <Numeric T>
    Polyline<T>::const_reverse_iterator Polyline<T>::crbegin() const{
        return std::reverse_iterator<const_iterator>(end());
    }

    template <Numeric T>
    Polyline<T>::const_reverse_iterator Polyline<T>::crend() const{
        return std::reverse_iterator<const_iterator>(begin());
    }

    /*----------------OPERATORS----------------*/
    template <Numeric T>
    Polyline<T>& Polyline<T>::operator=(Polyline<T> other){
        swap(other);
        return *this;
    }

    template <Numeric T>
    Point<T> &Polyline<T>::operator[](size_t i){
        return dots_[i];
    }

    template <Numeric T>
    const Point<T> &Polyline<T>::operator[](size_t i) const{
        return dots_[i];
    }

    template <Numeric T>
    void Polyline<T>::swap(Polyline<T> &other){
        std::swap(dots_, other.dots_);
        std::swap(capacity_, other.capacity_);
        std::swap(size_, other.size_);
    }

    /*----------------DISTRUCTOR----------------*/
    template <Numeric T>
    Polyline<T>::~Polyline(){
        delete [] dots_;
    }

    /*----------------MAIN FUNCTIONS----------------*/

    template <Numeric T>
    void Polyline<T>::resize(size_t new_capacity){
        Point<T>* new_dots = new Point<T>[new_capacity];
        std::copy(dots_, dots_ + std::min(size_, new_capacity), new_dots);
        delete [] dots_;
        dots_ = new_dots;
        capacity_ = new_capacity;
    }

    template <Numeric T>
    void Polyline<T>::add_point(const Point<T>& point){
        if(size_ == capacity_){
            resize(capacity_ * 2 + 1);
        }
        dots_[size_] = point;
        size_++;
    }

    template <Numeric T>
    void Polyline<T>::add_point(T x, T y, T z, char name){
        add_point(Point<T>{x, y, z, name});
    }

    template <Numeric T>
    void Polyline<T>::add_polyline(const Polyline<T>& other){
        if(size_ + other.size_ > capacity_){
            resize(std::max(capacity_ * 2, other.capacity_ * 2));
        }
        std::copy(other.begin(), other.end(), begin() + size_);
        size_ += other.size_;
    }

    template <Numeric T>
    void Polyline<T>::add_polyline(Polyline<T>&& other){
        if((size_ + other.size_ > capacity_) && (size_ + other.size_ <= other.capacity_)){
            std::move(other.begin(), other.end(), other.begin() + size_);
            std::move(begin(), end(), other.begin());
            swap(other);
            size_ += other.size_;
            other.size_ = 0;
            return;
        }
        if(size_ + other.size_ > capacity_){
            resize(std::max(capacity_ * 2, other.capacity_ * 2));
        }
        std::move(other.begin(), other.end(), begin() + size_);
        size_ += other.size_;
        other.size_ = 0;
    }

    template <Numeric T>
    void Polyline<T>::rotate_from_origin(double x_degree, double y_degree, double z_degree){
        double x_radians = x_degree * std::numbers::pi_v<double> / 180.0;
        double y_radians = y_degree * std::numbers::pi_v<double> / 180.0;
        double z_radians = z_degree * std::numbers::pi_v<double> / 180.0;
        Matrix<double, 3, 3> x_matrix = {
            1, 0, 0,
            0, std::cos(x_radians), std::sin(x_radians),
            0, (-1) * std::sin(x_radians), std::cos(x_radians)
        };
        Matrix<double, 3, 3> y_matrix = {
            std::cos(y_radians), 0, (-1) * std::sin(y_radians),
            0, 1, 0,
            std::sin(y_radians), 0, std::cos(y_radians)
        };
        Matrix<double, 3, 3> z_matrix = {
            std::cos(z_radians), std::sin(z_radians), 0,
            (-1) * std::sin(z_radians), std::cos(z_radians), 0,
            0, 0, 1
        };
        std::transform(begin(), end(), begin(), [x_matrix, y_matrix, z_matrix](const Point<T>& point){
                Matrix<T, 1, 3> matrix_point = get_matrix_from_point(point);
                matrix_point = matrix_point * x_matrix;
                matrix_point = matrix_point * y_matrix;
                matrix_point = matrix_point * z_matrix;
                return get_point_from_matrix(matrix_point, point.name_);
        });
    }

    template <Numeric T>
    void Polyline<T>::rotate_by_vector(const Point<T>& start, const Point<T>& finish, double degree){
        double radians = (-1) * degree * std::numbers::pi_v<double> / 180.0;
        double u = finish.x, v = finish.y, w = finish.z;
        double len = std::sqrt(u*u + v*v + w*w);
        if(len == 0) { return; }
        u /= len; v /= len; w /= len;
        double c = std::cos(radians);
        double s = std::sin(radians);
        double t = 1 - c;
        Matrix<double, 3, 3> rotation_matrix = {
            t*u*u + c,      t*u*v - s*w,   t*u*w + s*v,
            t*u*v + s*w,    t*v*v + c,     t*v*w - s*u,
            t*u*w - s*v,    t*v*w + s*u,   t*w*w + c
        };
        std::transform(begin(), end(), begin(), [rotation_matrix, start](const Point<T>& point){
                Matrix<T, 1, 3> matrix_point = get_matrix_from_point(point) - get_matrix_from_point(start);
                matrix_point = matrix_point * rotation_matrix;
                matrix_point = matrix_point + get_matrix_from_point(start);
                return get_point_from_matrix(matrix_point, point.name_);
        });
    }

    template <Numeric T>
    void Polyline<T>::shift(double x, double y, double z){
        std::transform(begin(), end(), begin(), [x, y, z](Point<T> point){
            point.x += x;
            point.y += y;
            point.z += z;
            return point;
        });
    }

    template <Numeric T>
    double Polyline<T>::length() const{
        if(size_ < 2){ return 0.0; }    
        return std::transform_reduce(begin() + 1, end(), begin(), 0, std::plus<>(), [](const auto& current, const auto& previous){
            return current.distance(previous);
        });
    }

    template <Numeric T>
    size_t Polyline<T>::points_count() const{
        return size_;
    }

    template <Numeric T>
    size_t Polyline<T>::find_distant() const{
        size_t res = 0;
        double max_distance = 0;
        double current_distance = 0;
        for(size_t i = 1; i < (size_ - 1); i++){
            current_distance = dots_[i].distance(dots_[i-1]) + dots_[i].distance(dots_[i+1]);
            if(max_distance < current_distance){
                max_distance = current_distance;
                res = i;
            }
        }
        return res;
    }

    template <Numeric T>
    void Polyline<T>::remove_distant(){
        if(size_ <= 2){ return; }
        size_t distant_index = find_distant();
        std::move(begin() + distant_index + 1, end(), begin() + distant_index);
        size_--;
    }
}

#endif