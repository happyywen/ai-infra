#include <iostream>

class Demo {
public:
    // construct func
    Demo() {
        std::cout << "Constructor\n";
    }

    // deconstruct func
    ~Demo() {
        std::cout << "Destructor\n";
    }
};

int main() {
    std::cout << "Start\n";

    {
        Demo obj;
        std::cout << "Inside scope\n";
    }

    std::cout << "End\n";
}