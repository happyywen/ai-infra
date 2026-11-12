
#include <iostream>
#include <cstddef>

float sum_array(const float* data, std::size_t n) {
    float result = 0.0f;

    // TODO: 累加数组中的所有元素
    for (std::size_t i = 0; i < n; i++)
        result += data[i];

    // const float *ptr = data;
    // const float *end = data + n;
    // for (; ptr != end; ++ptr)
    //     result += *ptr;

    return result;
}

int main() {
    const std::size_t n = 5;

    float* data = new float[n]{1, 2, 3, 4, 5};

    // TODO 1: 调用 sum_array()
    float sum = sum_array(data, n);
    // TODO 2: 打印结果，预期为 15
    std::cout<<"Sum: "<<sum<<"\n";
    // TODO 3: 正确释放动态数组
    delete[] data;
    // TODO 4: 避免继续使用已经释放的内存
    std::cout<<data[0];
    return 0;
}
