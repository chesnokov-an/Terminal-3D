#include <iostream>
#include <Matrix/Matrix.h>
#include <Polyline/Polyline.h>
#include <Buffer/Buffer.h>
#include <algorithm>

int main(){
    Buffer<74, 313> buffer;
    Matrix<int, 5, 5> mat{5};
    auto col_iter = mat.col_begin();
    auto col_c_iter = mat.col_cbegin();
    col_c_iter = col_iter;
    Polyline<int> pol1;
    pol1.add_point(20 + 0, 0, 0 + 25);
    pol1.add_point(20 + 25, 0, 0 + 25);
    pol1.add_point(20 + 25, 0, 15 + 25);
    pol1.add_point(20 + 0, 0, 15 + 25);
    pol1.add_point(20 + 0, 0, 0 + 25);
    pol1.add_point(20 + 0, 15, 0 + 25);
    pol1.add_point(20 + 0, 15, 15 + 25);
    pol1.add_point(20 + 0, 0, 15 + 25);
    pol1.add_point(20 + 0, 0, 0 + 25);
    pol1.add_point(20 + 0, 0, 15 + 25);
    pol1.add_point(20 + 0, 15, 15 + 25);
    pol1.add_point(20 + 0, 15, 0 + 25);
    pol1.add_point(20 + 25, 15, 0 + 25);
    pol1.add_point(20 + 25, 15, 15 + 25);
    pol1.add_point(20 + 0, 15, 15 + 25);
    pol1.add_point(20 + 0, 15, 0 + 25);
    pol1.add_point(20 + 25, 15, 0 + 25);
    pol1.add_point(20 + 25, 0, 0 + 25);
    pol1.add_point(20 + 25, 0, 15 + 25);
    pol1.add_point(20 + 25, 15, 15 + 25);
    
    buffer << pol1;
    std::cout << buffer;


    return 0;
}