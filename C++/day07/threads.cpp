#include <iostream>
#include <thread>

void worker(int id) {
    std::cout << "Worker " << id << '\n';
}

int main() {
    std::thread t1(worker, 1);
    std::thread t2(worker, 2);

    t1.join();
    t2.join();
}