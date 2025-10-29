#ifndef DIALOGUE_H
#define DIALOGUE_H

#include <cstddef>
#include <vector>
#include <Matrix/Matrix.h>
#include <Polyline/Polyline.h>
#include <Buffer/Buffer.h>
#include <Utils/GetNumber.h>

#define GREEN "\033[38;2;0;255;0m"
#define RED "\033[38;2;255;0;0m"
#define BLUE "\033[38;2;0;191;255m"
#define ORANGE "\033[38;2;255;165;0m"
#define MAGENTA "\033[38;2;255;20;147m"
#define YELLOW "\033[38;2;255;255;0m"
#define RESET "\033[0;0m"

namespace DialogueNameSpace {
    using namespace PolylineNameSpace;
    using namespace BufferNameSpace;
    using namespace UtilsNameSpace;

    template<Numeric T, size_t height, size_t width>
    void D_create_popyline(std::vector<Polyline<T>>& lines, __attribute__((unused)) Buffer<height, width>& buffer){
        std::cout << "Введите количество точек ломаной: ";
        size_t dots_count = get_num(1);
        Polyline<T> polyline;
        for(size_t i = 0; i < dots_count; i++){
            std::cout << "Введите координату x точки " << i+1 << ": ";
            T x = get_num<T>();
            std::cout << "Введите координату y точки " << i+1 << ": ";
            T y = get_num<T>();
            std::cout << "Введите координату z точки " << i+1 << ": ";
            T z = get_num<T>();
            std::cout << "Введите название точки " << i+1 << ": ";
            char name = get_num<char>('A', 'z');
            polyline.add_point(x, y, z, name);
        }
        lines.push_back(polyline);
    }

    template<Numeric T, size_t height, size_t width>
    void D_shift_polyline(std::vector<Polyline<T>>& lines, __attribute__((unused)) Buffer<height, width>& buffer){
        if(lines.size() == 0){ std::cout << "Буфер пуст :(" << std::endl; return; }
        std::cout << "Введите номер линии для сдвига (от 1 до " << lines.size() << "): ";
        size_t polyline_num = get_num<size_t>(1, lines.size());
        std::cout << "Введите сдвиг по x: ";
        double x = get_num<double>();
        std::cout << "Введите сдвиг по y: ";
        double y = get_num<double>();
        std::cout << "Введите сдвиг по z: ";
        double z = get_num<double>();
        lines[polyline_num - 1].shift(x, y, z);
    }

    template<Numeric T, size_t height, size_t width>
    void D_rotate_polyline_from_origin(std::vector<Polyline<T>>& lines, __attribute__((unused)) Buffer<height, width>& buffer){
        if(lines.size() == 0){ std::cout << "Буфер пуст :(" << std::endl; return; }
        std::cout << "Введите номер линии для поворота (от 1 до " << lines.size() << "): ";
        size_t polyline_num = get_num<size_t>(1, lines.size());
        std::cout << "Введите поворот по x: ";
        double x = get_num<double>();
        std::cout << "Введите поворот по y: ";
        double y = get_num<double>();
        std::cout << "Введите поворот по z: ";
        double z = get_num<double>();
        lines[polyline_num - 1].rotate_from_origin(x, y, z);
    }

    template<Numeric T, size_t height, size_t width>
    void D_rotate_polyline_by_vector(std::vector<Polyline<T>>& lines, __attribute__((unused)) Buffer<height, width>& buffer){
        if(lines.size() == 0){ std::cout << "Буфер пуст :(" << std::endl; return; }
        std::cout << "Введите номер линии для поворота (от 1 до " << lines.size() << "): ";
        size_t polyline_num = get_num<size_t>(1, lines.size());
        std::cout << "Введите координату x начала вектора: ";
        T x1 = get_num<T>();
        std::cout << "Введите координату y начала вектора: ";
        T y1 = get_num<T>();
        std::cout << "Введите координату z начала вектора: ";
        T z1 = get_num<T>();
        std::cout << "Введите координату x конца вектора: ";
        T x2 = get_num<T>();
        std::cout << "Введите координату y конца вектора: ";
        T y2 = get_num<T>();
        std::cout << "Введите координату z конца вектора: ";
        T z2 = get_num<T>();
        std::cout << "Введите угол поворота: ";
        double degree = get_num<double>();
        lines[polyline_num - 1].rotate_by_vector(Point<T>{x1, y1, z1}, Point<T>{x2, y2, z2}, degree);
    }

    template<Numeric T, size_t height, size_t width>
    void D_join_polyline(std::vector<Polyline<T>>& lines, __attribute__((unused)) Buffer<height, width>& buffer){
        if(lines.size() == 0){ std::cout << "Буфер пуст :(" << std::endl; return; }
        std::cout << "Введите номер линии к которой присоединить (от 1 до " << lines.size() << "): ";
        size_t polyline1_num = get_num<size_t>(1, lines.size());
        std::cout << "Введите номер линии которую присоединить (от 1 до " << lines.size() << "): ";
        size_t polyline2_num = get_num<size_t>(1, lines.size());
        lines[polyline1_num - 1].add_polyline(lines[polyline2_num - 1]);
        if(polyline1_num != polyline2_num){ lines.erase(lines.begin() + polyline2_num - 1); }
        std::cout << RED << lines[polyline1_num - 1].points_count() << RESET << std::endl;
    }

    template<Numeric T, size_t height, size_t width>
    void D_remove_distant(std::vector<Polyline<T>>& lines, __attribute__((unused)) Buffer<height, width>& buffer){
        if(lines.size() == 0){ std::cout << "Буфер пуст :(" << std::endl; return; }
        std::cout << "Введите номер линии к которой присоединить (от 1 до " << lines.size() << "): ";
        size_t polyline_num = get_num<size_t>(1, lines.size());
        lines[polyline_num - 1].remove_distant();
    }

    template<Numeric T, size_t height, size_t width>
    void D_print(std::vector<Polyline<T>>& lines, Buffer<height, width>& buffer){
        std::for_each(lines.begin(), lines.end(), [&buffer](const Polyline<T>& polyline){
            buffer << polyline;
        });
        std::cout << buffer;
        buffer.clean_buffer();
    }

    template<Numeric T, size_t height, size_t width>
    void D_clean(__attribute__((unused)) std::vector<Polyline<T>>& lines, Buffer<height, width>& buffer){
        buffer.clean_buffer();
        lines.clear();
    }

    void Dialogue(){
        void (*func_array[])(std::vector<Polyline<double>>&, Buffer<74, 313>&) = {D_create_popyline, D_shift_polyline, D_rotate_polyline_from_origin, D_rotate_polyline_by_vector, D_join_polyline, D_remove_distant, D_print, D_clean};
        Buffer<74, 313> buffer;
        std::vector<Polyline<double>> lines{};
        int option = -1;
	    do{
            std::cout << MAGENTA << "\n\n---------- МЕНЮ ----------\nВозможные команды:\n\n" << RESET;
            std::cout << GREEN << "1: Создание ломанной линии проходящей через заданный набор точек, заданных координатами и именем (одной буквой)\n" << RESET;
            std::cout << YELLOW << "2: Перемещение линии в заданном направлении на заданное расстояние\n" << RESET;
            std::cout << YELLOW << "3: Поворот линии вокруг начала координат\n" << RESET;
            std::cout << YELLOW << "4: Поворот линии вокруг точки на заданной произвольным вектором оси на заданное число градусов\n" << RESET;
            std::cout << ORANGE << "5: Объединение двух линий в одну отрезком, соединяющим последнюю точку первой линии с первой точкой второй линии\n" << RESET;
            std::cout << RED << "6: Удаление из линии точки, которая находится от своих соседей дальше всего\n" << RESET;
            std::cout << BLUE << "7: Вывод всех линий в трёхмерном виде в консоль\n" << RESET;
            std::cout << ORANGE << "8: Очистить буфер\n" << RESET;
            std::cout << RED << "\n0: завершение программы\n\n" << RESET;
            std::cout << MAGENTA << "Выберите опцию: " << RESET;
            try {
                option = get_num(0, 8);
            }
            catch(const std::runtime_error& e){
                std::cerr << "Input failed: " << RED << e.what() << RESET << std::endl;
                return;
            }

            std::cout << std::endl;
            if(option == 0){ return; }

            try {
                func_array[option-1](lines, buffer);
            }
            catch(const std::exception& e){
                std::cerr << "Something went wrong: " << RED << e.what() << RESET << std::endl;
                return;
            }
        }while(option != 0);
    }
}

#endif