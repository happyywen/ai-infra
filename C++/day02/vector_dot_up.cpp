#include <iostream>
#include <vector>
#include <cstddef>

// 版本 1：值传递
float dot_value(std::vector<float> a, std::vector<float> b) {
    float result = 0.0f;

    std::size_t lenA = a.size();
    std::size_t lenB = b.size();
    
    if (lenA != lenB) 
        throw std::invalid_argument("dot_produce: size mismatch");
    
    for (std::size_t i = 0; i < lenA; ++i)
        result += a[i] * b[i];

    return result;
}

// 版本 2：const 引用传递
float dot_ref(const std::vector<float>& a, const std::vector<float>& b) {
    float result = 0.0f;

    // size() 返回 std::size_t, 64位无符号整数，传给 int 缩小了，可能溢出
    // int lenA = a.size();
    // int lenB = b.size();
    std::size_t lenA = a.size();
    std::size_t lenB = b.size();
    
    if (lenA != lenB) 
        throw std::invalid_argument("dot_produce: size mismatch");
    
    for (std::size_t i = 0; i < lenA; ++i)
        result += a[i] * b[i];

    return result;
}

// 版本 3：指针传递
float dot_ptr(const float* a, const float* b, std::size_t n) {
    float result = 0.0f;

    const float *pa = a;
    const float *pb = b;
    const float *end = a + n;

    for ( ; pa != end; ++pa, ++pb) {
        result += *pa * *pb;
    }

    
    // for (int i = 0; i < n; ++i)
    //     result += a[i] * b[i];

    return result;
}

int main() {
    std::vector<float> a = {1, 2, 3};
    std::vector<float> b = {1, 2, 3};

    std::cout<<dot_value(a, b)<<"\n";
    std::cout<<dot_ref(a, b)<<"\n";
    std::cout<<dot_ptr(a.data(), b.data(), a.size()) << "\n";
    return 0;
}