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
    Matrix res = m1.transposed();
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 3; j++){
            std::cout << res(i, j) << " ";
        }
        std::cout << std::endl;
    }
    // std::for_each(res.begin(), res.end(), [](int value){ std::cout << value << " "; });

    return 0;
}