/**
 * @file Buffer.h
 * @brief 2D character buffer for 3D polyline rendering with isometric projection
 * @author Chesnokov Alexandr
 * @date 2025
 * @version 1.0
 * 
 * This header defines a Buffer class for rendering 3D polylines onto a 2D character
 * display using isometric projection. Includes support for colored terminal output.
 */

#ifndef BUFFER_H
#define BUFFER_H

#include <Matrix/Matrix.h>
#include <Polyline/Polyline.h>
#include <concepts>
#include <cstddef>
#include <utility>
#include <cmath>
#include <stdexcept>

// ANSI color codes for terminal output
#define GREEN "\033[38;2;0;255;0m"    ///< Green color code (RGB: 0,255,0)
#define BLUE "\033[38;2;0;191;255m"   ///< Blue color code (RGB: 0,191,255)
#define RESET "\033[0;0m"             ///< Reset color and style codes

namespace BufferNameSpace {
    using namespace MatrixNameSpace;
    using namespace PolylineNameSpace;

    /**
     * @struct BufferPoint
     * @brief 2D point representation for buffer coordinates
     * 
     * Used internally for converting 3D points to 2D screen coordinates
     * during the rendering process.
     */
    struct BufferPoint{
        double x = 0; ///< X coordinate in buffer (vertical position)
        double y = 0; ///< Y coordinate in buffer (horizontal position)
    };

    /**
     * @class Buffer
     * @brief 2D character buffer for rendering 3D polylines with isometric projection
     * @tparam height_ Height of the buffer in characters (compile-time constant)
     * @tparam width_ Width of the buffer in characters (compile-time constant)
     * 
     * The Buffer class provides a character-based display for 3D graphics using
     * isometric projection. It supports rendering polylines with automatic line
     * drawing and colored terminal output.
     */
    template <size_t height_, size_t width_>
    class Buffer{
    private:
        Matrix<char, height_, width_> buffer_{}; ///< Character matrix representing the display buffer

        /**
         * @brief Converts 3D point to 2D buffer coordinates using isometric projection
         * @tparam T Numeric type of point coordinates (must satisfy Numeric concept)
         * @param point 3D point to convert (Point<T> with x, y, z coordinates)
         * @return BufferPoint containing 2D screen coordinates after projection
         * 
         * The projection formula used:
         * x_2d = round((point.x + point.y) / sqrt(15) - point.z * 0.6) + height_ * 2 / 3
         * y_2d = point.y - point.x + width_ / 2
         */
        template <Numeric T>
        BufferPoint get_point_2d(const Point<T>& point);

        /**
         * @brief Calculates perpendicular distance from a point to a line segment
         * @param point Point to calculate distance from (BufferPoint with x, y coordinates)
         * @param start_line Start point of the line segment (BufferPoint)
         * @param end_line End point of the line segment (BufferPoint)
         * @return double Distance from the point to the line segment
         * 
         * Uses the formula for distance from point to line:
         * distance = |(end.x - start.x)*(start.y - point.y) - (start.x - point.x)*(end.y - start.y)| 
         *            / sqrt((end.x - start.x)² + (end.y - start.y)²)
         */
        double distance_to_the_line(const BufferPoint& point, const BufferPoint& start_line, const BufferPoint& end_line);

        /**
         * @brief Draws a line between two 3D points in the buffer using Bresenham-like algorithm
         * @tparam T Numeric type of point coordinates (must satisfy Numeric concept)
         * @param point1 First 3D point (Point<T> with x, y, z coordinates and name)
         * @param point2 Second 3D point (Point<T> with x, y, z coordinates and name)
         * 
         * Draws the points themselves as their character labels and connects them
         * with '-' characters. Uses distance-based line drawing for smooth lines.
         */
        template <Numeric T>
        void draw_line(const Point<T>& point1, const Point<T>& point2);

        /**
         * @brief Draws coordinate axes (X, Y, Z) in the buffer
         * 
         * Creates axes labeled 'X', 'Y', 'Z' originating from point 'O' (origin).
         * The axes are drawn using the draw_line method.
         */
        void draw_axes();

    public:
        /**
         * @brief Default constructor
         * 
         * Initializes the buffer with spaces and draws coordinate axes.
         */
        Buffer();

        /**
         * @brief Clears the buffer and redraws axes
         * 
         * Fills the buffer with space characters and redraws the coordinate axes.
         * Useful for resetting the display between frames.
         */
        void clean_buffer();

        /**
         * @brief Stream insertion operator for Polyline objects
         * @tparam T Numeric type of polyline coordinates (must satisfy Numeric concept)
         * @param buffer Reference to the target buffer
         * @param polyline Polyline object to render into the buffer
         * @return Reference to the buffer after rendering
         * 
         * Renders each segment of the polyline using draw_line method.
         * Friend function for direct access to buffer internals.
         */
        template <Numeric T>
        friend Buffer& operator<<(Buffer& buffer, const Polyline<T>& polyline){
            size_t size = polyline.points_count();
            if(size == 1){ buffer.draw_line(polyline[0], polyline[0]); }
            for(size_t i = 1; i < size; i++){
                buffer.draw_line(polyline[i - 1], polyline[i]);
            }
            return buffer;
        }

        /**
         * @brief Output stream operator for buffer display
         * @param out Output stream to write to (e.g., std::cout)
         * @param buffer Buffer object to display
         * @return Reference to the output stream
         * 
         * Outputs the buffer contents with colored formatting:
         * - Lines are displayed in GREEN
         * - Points are displayed in BLUE
         * Friend function for direct access to buffer internals.
         */
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
}

#endif