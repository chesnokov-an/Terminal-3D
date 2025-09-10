#ifndef POLYLINE_H
#define POLYLINE_H

#include <concepts>
#include <array>

template <typename T>
concept Numeric = std::is_arithmetic_v<T>;

template <Numeric T>
struct Point{
    T x = 0;
    T y = 0;
    T z = 0;
};

template <Numeric T>
class Polyline{
private:
    Point<T>* dots_ = nullptr;
    size_t capacity_ = 0;
    size_t size_ = 0;

public:
    Polyline(const Polyline& other) : dots_(new Point<T>[other.capacity_]), capacity_(other.capacity_), size_(other.size_){
        std::copy(other.dots_, other.dots_ + size_, dots_);
    }
    Polyline(Polyline&& other) : dots_(new Point<T>[other.capacity_]), capacity_(other.capacity_), size_(other.size_){
        std::copy(other.dots_, other.dots_ + size_, dots_);
        other.dots_ = nullptr;
    }
    Polyline& operator=(Polyline other);
    Polyline& operator=(Polyline&& other);
    void swap(Polyline& other);
    ~Polyline();
    void add_point(const Point<T>& point);
    

};

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
void Polyline<T>::swap(Polyline<T> &other){
    std::swap(dots_, other.dots_);
    std::swap(capacity_, other.capacity_);
    std::swap(size_, other.size_);
}

template <Numeric T>
Polyline<T>::~Polyline(){
    delete [] dots_;
}

template <Numeric T>
void Polyline<T>::add_point(const Point<T>& point){
    if(size_ == capacity_){
        if(capacity_ == 0){ capacity_ = 1; }
        Point<T>* new_dots = new Point<T>[capacity_ * 2];
        capacity_ *= 2;
        std::copy(dots_, dots_ + size_, new_dots);
        delete [] dots_;
        dots_ = new_dots;
    }
    dots_[size_] = point;
    size_++;
}

#endif