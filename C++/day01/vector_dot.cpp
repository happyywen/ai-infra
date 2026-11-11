#include <iostream>
#include <vector>

float dot_product (std::vector<float> a, std::vector<float> b) {
    float result = 0.0f;

    // TODO 1: 检查两个向量长度
    int lenA = a.size();
    int lenB = b.size();

    if (lenA != lenB) 
        throw std::invalid_argument("dot_product: size mismatch");

    // TODO 2: 使用 for 循环计算点积
    for(int i = 0; i < lenA; ++i) {
        result += a[i] * b[i];
    }
    // TODO 3: 返回结果
    return result;
}

int main() {
    std::vector<float> a = {1, 2, 3};
    std::vector<float> b = {1, 2, 3};

    std::cout<<dot_product(a, b);
    return 0;
}