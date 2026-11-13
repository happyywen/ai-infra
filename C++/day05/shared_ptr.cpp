#include <iostream>
#include <memory>

class Demo {
    public:
        Demo() {
            std::cout << "Demo 构造\n";
        }

        ~Demo() {
            std::cout << "Demo 析构\n";
        }
};

int main() {

    auto a = std::make_shared<Demo>();

    std::cout<<"a.use_count = "<<a.use_count()<<"\n";

    {
        // 共享
        auto b = a;
        std::cout<<"块内 use_count = "<<a.use_count()<<"\n";
    }

    std::cout<<"块外 use_count = "<<a.use_count()<<"\n";    

    return 0;
}