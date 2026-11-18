#include "dot_product.h"
#include <iostream>
// #include <vector>

int main() {
    std::vector<float> a{1.0, 2.0, 3.0};
    std::vector<float> b{4.0, 5.0, 6.0};
    std::cout << "dot = " << dot_product(a, b) << std::endl;
    return 0;
}

