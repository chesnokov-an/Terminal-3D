#include <iostream>
#include <Matrix/Matrix.h>
#include <algorithm>

int main(){
    Matrix<int, 3, 4> m1 = {
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12
    };
    Matrix<int, 3, 4> m2(5);
    Matrix<int, 3, 4> res = m1 + m2;
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 4; j++){
            std::cout << m2(i, j) << " ";
        }
        std::cout << std::endl;
    }
    std::for_each(res.begin(), res.end(), [](int value){ std::cout << value << " "; });

    return 0;
}