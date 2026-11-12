
#include <iostream>
#include <cstddef>

/*
=================================================================
==27081==ERROR: LeakSanitizer: detected memory leaks

Direct leak of 20 byte(s) in 1 object(s) allocated from:
    #0 0x741a3a2fe6c8 in operator new[](unsigned long) ../../../../src/libsanitizer/asan/asan_new_delete.cpp:98
    #1 0x5e7d857ec300 in main /home/yjw/projects/ai-infra/C++/day03/dynamic_array_mem_leak.cpp:23
    #2 0x741a39a2a1c9 in __libc_start_call_main ../sysdeps/nptl/libc_start_call_main.h:58
    #3 0x741a39a2a28a in __libc_start_main_impl ../csu/libc-start.c:360
    #4 0x5e7d857ec184 in _start (/home/yjw/projects/ai-infra/C++/day03/dynamic_array_mem_leak+0x1184) (BuildId: 9199eb24175ee53b677a8d8927950bac59a703e2)

SUMMARY: AddressSanitizer: 20 byte(s) leaked in 1 allocation(s).
*/

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
    // delete[] data;
    // TODO 4: 避免继续使用已经释放的内存
    // data = nullptr;
    return 0;
}
