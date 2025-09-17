#ifndef BUFFER_H
#define BUFFER_H

#include <Matrix/Matrix.h>
#include <Polyline/Polyline.h>
#include <concepts>
#include <cstddef>
#include <utility>
#include <cmath>
#include <stdexcept>

#define GREEN "\033[38;2;0;255;0m"
#define RED "\033[38;2;255;0;0m"
#define BLUE "\033[38;2;0;191;255m"
#define LIGHT "\033[38;2;180;180;180m"
#define ORANGE "\033[38;2;255;165;0m"
#define MAGENTA "\033[38;2;255;20;147m"
#define YELLOW "\033[38;2;255;255;0m"
#define RESET "\033[0;0m"


struct BufferPoint{
    double x = 0;
    double y = 0;
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
            std::for_each(buffer.buffer_.begin() + x * width_, buffer.buffer_.begin() + (x + 1) * width_, [&out](char elem){
                if(elem != '-'){ out << BLUE << elem << RESET; }
                else{ out << GREEN << elem << RESET; }
            });
            out << std::endl;
        }
        return out;
    }
};

template <size_t height_, size_t width_>
template <Numeric T>
BufferPoint Buffer<height_, width_>::get_point_2d(const Point<T>& point){
    BufferPoint result = {
        std::round((point.x + point.y) / std::sqrt(15) - point.z * 0.6) + height_ * 2 / 3,
        static_cast<double>(point.y) - static_cast<double>(point.x) + static_cast<double>(width_) / 2
    };
    return result;
}

template<size_t height_, size_t width_>
double Buffer<height_, width_>::distance_to_the_line(const BufferPoint& point, const BufferPoint& start_line, const BufferPoint& end_line){
    double numerator = std::abs((end_line.x - start_line.x)*(start_line.y - point.y) - (start_line.x - point.x)*(end_line.y - start_line.y));
    double denominator = std::sqrt((end_line.x - start_line.x)*(end_line.x - start_line.x) + (end_line.y - start_line.y)*(end_line.y - start_line.y));
    return numerator / denominator;
}

template<size_t height_, size_t width_>
template <Numeric T>
void Buffer<height_, width_>::draw_line(const Point<T>& point1, const Point<T>& point2){
    BufferPoint point_2d_1 = get_point_2d(point1);
    BufferPoint point_2d_2 = get_point_2d(point2);
    if(point_2d_1.x < height_ && point_2d_1.x >= 0 && point_2d_1.y < width_ && point_2d_1.y >= 0){
        buffer_[point_2d_1.x, point_2d_1.y] = point1.name_;
    }
    if(point_2d_2.x < height_ && point_2d_2.x >= 0 && point_2d_2.y < width_ && point_2d_2.y >= 0){
        buffer_[point_2d_2.x, point_2d_2.y] = point2.name_;
    }
    size_t min_x = std::min(std::max(std::min(point_2d_1.x, point_2d_2.x), static_cast<double>(0)), static_cast<double>(height_-1));
    size_t min_y = std::min(std::max(std::min(point_2d_1.y, point_2d_2.y), static_cast<double>(0)), static_cast<double>(width_-1));
    size_t max_x = std::max(std::min(std::max(point_2d_1.x, point_2d_2.x), static_cast<double>(height_-1)), static_cast<double>(0));
    size_t max_y = std::max(std::min(std::max(point_2d_1.y, point_2d_2.y), static_cast<double>(width_-1)), static_cast<double>(0));
    if(min_y == max_y){
        for(size_t x = min_x+1; x < max_x; x++){
            buffer_[x, min_y] = '-';
        }
        return;
    }
    for(size_t x = min_x; x <= max_x; x++){
        for(size_t y = min_y; y <= max_y; y++){
            if((x == min_x || x == max_x) && (y == min_y || y == max_y)){ continue; }
            BufferPoint current = {static_cast<double>(x), static_cast<double>(y)};
            if(distance_to_the_line(current, point_2d_1, point_2d_2) < 0.4){
                buffer_[x, y] = '-';
            }
        }
    }
}

template<size_t height_, size_t width_>
void Buffer<height_, width_>::draw_axes(){
    Point<int> O = {0, 0, 0, 'O'};
    Point<int> X = {height_-1, 0, 0, 'X'};
    Point<int> Y = {0, height_-1, 0, 'Y'};
    Point<int> Z = {0, 0, height_-1, 'Z'};
    draw_line(O, X);
    draw_line(O, Y);
    draw_line(O, Z);
}

template<size_t height_, size_t width_>
Buffer<height_, width_>::Buffer(){
    clean_buffer();
}

template<size_t height_, size_t width_>
void Buffer<height_, width_>::clean_buffer(){
    buffer_.fill(' ');
    draw_axes();
}

#endif