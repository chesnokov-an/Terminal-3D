#ifndef POLYLINE_H
#define POLYLINE_H

#include <concepts>
#include <array>
#include <cstddef>
#include <cmath>
#include <numbers>
#include <cstring>
#include <Matrix/Matrix.h>

template <Numeric T>
class Point{
public:
    T x = 0;
    T y = 0;
    T z = 0;
    char name_ = 'A';

    double distance(const Point& other);
};

template <Numeric T>
Matrix<T, 1, 3> get_matrix_from_point(const Point<T>& point){
    return Matrix<T, 1, 3>{point.x, point.y, point.z};
}

template <Numeric T>
Point<T> get_point_from_matrix(const Matrix<T, 1, 3>& matrix, char name){
    return Point<T>{matrix[0, 0], matrix[0, 1], matrix[0, 2], name};
}

template <Numeric T>
class Polyline{
private:
    Point<T>* dots_ = nullptr;
    size_t capacity_ = 0;
    size_t size_ = 0;

public:
    using iterator = Point<T>*;
    using const_iterator = const Point<T>*;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    
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

public:
    Polyline() = default;
    Polyline(const Polyline& other) : dots_(new Point<T>[other.capacity_]), capacity_(other.capacity_), size_(other.size_){
        std::copy(other.dots_, other.dots_ + size_, dots_);
    }
    Polyline(Polyline&& other){
        swap(other);
    }
    Polyline& operator=(Polyline other);
    Point<T>& operator[](size_t i);
    const Point<T>& operator[](size_t i) const;
    void swap(Polyline& other);
    ~Polyline();
    void resize(size_t new_capacity);
    void add_point(const Point<T>& point);
    void add_point(T x, T y, T z, char name);
    void add_polyline(const Polyline& polyline);
    void add_polyline(Polyline&& polyline);
    void rotate_from_origin(double x_degree, double y_degree, double z_degree);
    void rotate_by_vector(const Point<T>& start, const Point<T>& finish, double degree);
    void shift(double x, double y, double z);
    double length() const;
    size_t points_count() const;
    size_t find_distant() const;
    void remove_distant();
};

/****************Realization****************/
/*----------------POINT----------------*/
template <Numeric T>
double Point<T>::distance(const Point<T> &other){
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
    double result = 0;
    for(size_t i = 1; i < size_; i++){
        result += dots_[i].distance(dots_[i-1]);
    }
    return result;
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

#endif