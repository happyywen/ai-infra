
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

class Counter {
private:
    int value_ = 0;
    std::mutex mutex_;

public:
    void increment() {
        // TODO: 使用 lock_guard 保护 value_
        std::lock_guard<std::mutex> lk(mutex_);
        ++value_;
    }

    int get() {
        // TODO: 加锁后读取 value_
        std::lock_guard<std::mutex> lk(mutex_);
        return value_;
    }
};

int main() {
    Counter counter;

    constexpr int num_threads = 8;
    constexpr int iterations = 100000;

    std::vector<std::thread> threads;

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([&counter]() {
            for (int j = 0; j < iterations; ++j) {
                counter.increment();
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Final counter: "
              << counter.get() << '\n';
}
