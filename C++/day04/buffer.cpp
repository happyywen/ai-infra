
#include <cstddef>
#include <iostream>
#include <memory>

class Buffer {
private:
    std::size_t size_;
    std::unique_ptr<float[]> data_;

public:
    explicit Buffer(std::size_t size)
        : size_(size),
          data_(std::make_unique<float[]>(size)) {
        std::cout << "Buffer created\n";
    }

    ~Buffer() {
        std::cout << "Buffer destroyed\n";
    }

    std::size_t size() const {
        return size_;
    }

    float& operator[](std::size_t i) {
        return data_[i];
    }

    const float& operator[](std::size_t i) const {
        return data_[i];
    }

    // TODO: 实现 fill(float value)
    void fill(float value) {
        for (std::size_t i = 0; i < size_; ++i) {
            data_[i] = value;
        }
    }
    // TODO: 实现 sum() const
    float sum() const {
        float sum = 0.0f;
        for (std::size_t i = 0; i < size_; ++i) {
            sum += data_[i];
        }
        return sum;
    }
};

int main() {
    Buffer buf(5);

    // TODO: 将元素初始化为 1, 2, 3, 4, 5
    for (std::size_t i = 0; i < buf.size(); ++i) {
        buf[i] = static_cast<float>(i + 1);
    }
    // TODO: 输出 sum()，预期为 15
    std::cout<<buf.sum()<<"\n";
    // TODO: 观察析构函数的调用时机

    return 0;
}
