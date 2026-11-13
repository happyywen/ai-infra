#include <iostream>

template <typename T>
T max_val(T a, T b) {
    return a > b ? a : b;
}

int main() {
    std::cout<<max_val(2,5)<<"\n";
    std::cout<<max_val("c","a")<<"\n";
    std::cout<<max_val(5.1,5.0)<<"\n";

    return 0;
}