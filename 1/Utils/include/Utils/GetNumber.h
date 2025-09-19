#ifndef GETNUMBER_H
#define GETNUMBER_H

#include <iostream>
#include <limits>

namespace UtilsNameSpace {
    template<typename T>
    T get_num(T min = std::numeric_limits<T>::lowest(), T max = std::numeric_limits<T>::max()){
        T num = 0;
        while(!std::cin.eof()){
            std::cin >> num;
            if(std::cin.eof()){
                throw std::runtime_error("End Of File\n");
            }
            else if(std::cin.bad()){
                throw std::runtime_error("BAD! Something is wrong!\n");
            }
            else if(std::cin.fail() || num < min || num > max){
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Please repeat it correctly!" << std::endl;
            }
            else{
                return num;
            }
        }
        return num;
    }
}

#endif