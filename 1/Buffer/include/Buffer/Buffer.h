#ifndef BUFFER_H
#define BUFFER_H

#include <Matrix/Matrix.h>
#include <Polyline/Polyline.h>
#include <concepts>
#include <cstddef>
#include <utility>
#include <cmath>
#include <stdexcept>

struct BufferPoint{
    size_t x = 0;
    size_t y = 0;
};

template <size_t height_, size_t width_>
class Buffer{
private:
    Matrix<char, height_, width_> buffer_{};

    template <Numeric T>
    BufferPoint get_point_2d(const Point<T>& point);
    double distance_to_the_line(const BufferPoint& point, const BufferPoint& start_line, const BufferPoint& end_line);
    template <Numeric T>
    void draw_line(const Point<T>& point1, const Point<T>& point2);
    void draw_axes();

public:
    Buffer();
    void clean_buffer();
    template <Numeric T>
    friend Buffer& operator<<(Buffer& buffer, const Polyline<T>& polyline){
        size_t size = polyline.points_count();
        for(size_t i = 1; i < size; i++){
            buffer.draw_line(polyline[i - 1], polyline[i]);
        }
        return buffer;
    }
    friend std::ostream& operator<<(std::ostream& out, const Buffer& buffer){
        for(size_t x = 0; x < height_; x++){
            std::for_each(buffer.buffer_.begin() + x * width_, buffer.buffer_.begin() + (x + 1) * width_, [&out](char elem){ out << elem; });
            out << std::endl;
        }
        return out;
    }
};

template <size_t height_, size_t width_>
template <Numeric T>
BufferPoint Buffer<height_, width_>::get_point_2d(const Point<T>& point){
    long long new_y = std::round(static_cast<long long>(point.y) - static_cast<long long>(point.x)) + static_cast<long long>(width_) / 2;
    long long new_x = std::round((static_cast<long long>(point.x) + static_cast<long long>(point.y)) / std::sqrt(15) - static_cast<long long>(point.z) * 0.6) + static_cast<long long>(height_) * 2 / 3;
    if(new_x < 0 || new_y < 0){
        throw std::out_of_range("Point is not printable");
    }
    BufferPoint result = {static_cast<size_t>(new_x), static_cast<size_t>(new_y)};
    if(result.x >= height_ || result.y >= width_){
        throw std::out_of_range("Point is not printable");
    }
    return result;
}

template<size_t height_, size_t width_>
double Buffer<height_, width_>::distance_to_the_line(const BufferPoint& point, const BufferPoint& start_line, const BufferPoint& end_line){
    double x0 = static_cast<double>(point.x);
    double y0 = static_cast<double>(point.y);
    double x1 = static_cast<double>(start_line.x);
    double y1 = static_cast<double>(start_line.y);
    double x2 = static_cast<double>(end_line.x);
    double y2 = static_cast<double>(end_line.y);
    double numerator = std::abs((x2 - x1)*(y1 - y0) - (x1 - x0)*(y2 - y1));
    double denominator = std::sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
    return numerator / denominator;
}

template<size_t height_, size_t width_>
template <Numeric T>
void Buffer<height_, width_>::draw_line(const Point<T>& point1, const Point<T>& point2){
    BufferPoint point_2d_1 = get_point_2d(point1);
    BufferPoint point_2d_2 = get_point_2d(point2);
    buffer_[point_2d_1.x, point_2d_1.y] = '*';
    buffer_[point_2d_2.x, point_2d_2.y] = '*';
    size_t min_x = std::min(point_2d_1.x, point_2d_2.x);
    size_t min_y = std::min(point_2d_1.y, point_2d_2.y);
    size_t max_x = std::max(point_2d_1.x, point_2d_2.x);
    size_t max_y = std::max(point_2d_1.y, point_2d_2.y);
    if(min_x == max_x){
        for(size_t y = min_y+1; y < max_y; y++){
            buffer_[min_x, y] = '-';
        }
        return;
    }
    if(min_y == max_y){
        for(size_t x = min_x+1; x < max_x; x++){
            buffer_[x, min_y] = '-';
        }
        return;
    }
    for(size_t x = min_x+1; x < max_x; x++){
        for(size_t y = min_y+1; y < max_y; y++){
            BufferPoint current = {x, y};
            if(distance_to_the_line(current, point_2d_1, point_2d_2) < std::sqrt(3)/4){
                buffer_[x, y] = '-';
            }
        }
    }
}

template<size_t height_, size_t width_>
void Buffer<height_, width_>::draw_axes(){
    Point<int> O = {0, 0, 0};
    Point<int> X = {height_, 0, 0};
    Point<int> Y = {0, height_, 0};
    Point<int> Z = {0, 0, height_};
    draw_line(O, X);
    draw_line(O, Y);
    draw_line(O, Z);
}

template<size_t height_, size_t width_>
Buffer<height_, width_>::Buffer(){
    buffer_.fill(' ');
    draw_axes();
}

template<size_t height_, size_t width_>
void Buffer<height_, width_>::clean_buffer(){
    buffer_.fill(' ');
    draw_axes();
}

#endif