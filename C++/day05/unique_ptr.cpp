#include <iostream>
#include <memory>

int main() {
    auto p1 = std::make_unique<int>(42);

    // 所有权转移
    auto p2 = std::move(p1);

    std::cout<<(p1 == nullptr)<<"\n";
    std::cout<<*p2<<"\n";

    return 0;
}