#include <iostream>
#include <Matrix/Matrix.h>
#include <Polyline/Polyline.h>
#include <Buffer/Buffer.h>
#include <algorithm>

int main(){
    Buffer<74, 313> buffer;
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