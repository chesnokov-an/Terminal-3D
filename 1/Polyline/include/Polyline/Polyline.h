#ifndef POLYLINE_H
#define POLYLINE_H

#include <concepts>
#include <array>
#include <cstddef>
#include <cmath>
#include <cstring>
#include <Matrix/Matrix.h>

template <Numeric T>
class Point{
public:
    T x = 0;
    T y = 0;
    T z = 0;

    double distance(const Point& other);

};

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
    Polyline(Polyline&& other) : dots_(new Point<T>[other.capacity_]), capacity_(other.capacity_), size_(other.size_){
        std::copy(other.dots_, other.dots_ + size_, dots_);
        other.dots_ = nullptr;
    }
    Polyline& operator=(Polyline other);
    Polyline& operator=(Polyline&& other);
    Point<T>& operator[](size_t i);
    const Point<T>& operator[](size_t i) const;
    void swap(Polyline& other);
    ~Polyline();
    void resize(size_t new_capacity);
    void add_point(const Point<T>& point);
    void add_point(T x, T y, T z);
    void add_polyline(Polyline& polyline);
    void add_polyline(Polyline&& polyline);
    template <size_t col_size_>
    Matrix<T, col_size_, 3> get_matrix();
    template <size_t col_size_>
    void set_from_matrix(const Matrix<T, col_size_, 3>& polyline_matrix);
    void rotate(double x_degree, double y_degree, double z_degree);
    void move(double x, double y, double z);
    double distance() const;
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
Polyline<T> &Polyline<T>::operator=(Polyline<T> &&other){
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
    std::copy(dots_, dots_ + size_, new_dots);
    delete [] dots_;
    dots_ = new_dots;
    capacity_ = new_capacity;
}

template <Numeric T>
void Polyline<T>::add_point(const Point<T>& point){
    if(size_ == capacity_){
        if(capacity_ == 0){ capacity_ = 1; }
        resize(capacity_ * 2);
    }
    dots_[size_] = point;
    size_++;
}

template <Numeric T>
void Polyline<T>::add_point(T x, T y, T z){
    Point<T> new_point = {x, y, z};
    add_point(new_point);
}

template <Numeric T>
void Polyline<T>::add_polyline(Polyline<T>& other){
    if(size_ + other.size_ > capacity_){
        resize(std::max(capacity_ * 2, other.capacity * 2));
    }
    std::copy(other.begin(), other.end(), begin() + size_);
    size_ += other.size_;
}

template <Numeric T>
void Polyline<T>::add_polyline(Polyline<T>&& other){
    if(size_ + other.size_ > capacity_){
        resize(std::max(capacity_ * 2, other.capacity * 2));
    }
    std::move(other.begin(), other.end(), begin() + size_);
    size_ += other.size_;
}

template<Numeric T>
template<size_t col_size_>
Matrix<T, col_size_, 3> Polyline<T>::get_matrix(){
    Matrix<T, this->size_, 3> polyline_matrix{};
    for(size_t i = 0; i < size_; i++){
        polyline_matrix(i, 0) = dots_[i].x;
        polyline_matrix(i, 1) = dots_[i].y;
        polyline_matrix(i, 2) = dots_[i].z;
    }
    return polyline_matrix;
}

template<Numeric T>
template<size_t col_size_>
void Polyline<T>::set_from_matrix(const Matrix<T, col_size_, 3>& polyline_matrix){
    for(size_t i = 0; i < size_; i++){
        dots_[i].x = polyline_matrix(i, 0);
        dots_[i].y = polyline_matrix(i, 1);
        dots_[i].z = polyline_matrix(i, 2);
    }
}

template <Numeric T>
void Polyline<T>::rotate(double x_degree, double y_degree, double z_degree){
    double x_radians = x_degree * M_PI / 180.0;
    double y_radians = y_degree * M_PI / 180.0;
    double z_radians = z_degree * M_PI / 180.0;
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
    Matrix<T, this->size_, 3> polyline_matrix = get_matrix();
    polyline_matrix = polyline_matrix * x_matrix;
    polyline_matrix = polyline_matrix * y_matrix;
    polyline_matrix = polyline_matrix * z_matrix;
    set_from_matrix(polyline_matrix);
}

template <Numeric T>
void Polyline<T>::move(double x, double y, double z){
    std::transform(begin(), end(), begin(), [x, y, z](Point<T>& point){
        point.x += x;
        point.y += y;
        point.z += z;
    });
}

template <Numeric T>
double Polyline<T>::distance() const{
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
    std::memmove(begin() + distant_index, begin() + distant_index + 1, sizeof(Point<T>) * (size_ - distant_index - 1));
}

#endif