#include <iostream>
#include <Matrix/Matrix.h>
#include <algorithm>

int main(){
    Matrix<int, 4, 3> m1 = {
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12
    };
    Matrix<int, 3, 7> m2(5);
    Matrix<int, 4, 7> res = m1 * m2;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 7; j++){
            std::cout << res(i, j) << " ";
        }
        std::cout << std::endl;
    }
    // std::for_each(m2.begin(), res.end(), [](int value){ std::cout << value << " "; });

    return 0;
}