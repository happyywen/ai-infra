
#include "dot_product.h"

#include <cassert>
#include <cmath>
#include <stdexcept>
#include <vector>

int main() {
    // Test 1: 正常向量
    {
        std::vector<float> a{1, 2, 3};
        std::vector<float> b{4, 5, 6};

        assert(std::fabs(dot_product(a, b) - 32.0f) < 1e-5f);
    }

    // TODO Test 2: 空向量，预期为 0
    {
        std::vector<float> a{};
        std::vector<float> b{};

        assert(std::fabs(dot_product(a, b)) == 0.0f);
    }
    // TODO Test 3: 零向量，预期为 0
    {
        std::vector<float> a{0, 0, 0};
        std::vector<float> b{0, 0, 0};

        assert(std::fabs(dot_product(a, b)) == 0.0f);
    }
    // TODO Test 4: 长度不匹配，预期抛出异常
    {
        std::vector<float> a{1, 2, 3};
        std::vector<float> b{4, 5, };
        bool thrown = false;
        try {
            dot_product(a, b);
        } catch (const std::invalid_argument& e) {
            thrown = true;
            std::printf("Test 4 caught as expected: %s\n", e.what());
        }
        assert(thrown);
        // assert(std::fabs(dot_product(a, b) - 32.0f) < 1e-5f);
    }
    return 0;
}
