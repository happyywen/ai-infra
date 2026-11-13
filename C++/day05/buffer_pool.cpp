
#include <iostream>
#include <cstddef>
#include <memory>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <string>

class BufferPool {
private:
    std::unordered_map<int, std::unique_ptr<std::vector<float>>> buffers_;

    int next_id_ = 0;

public:
    int allocate(std::size_t n) {
        int id = next_id_++;

        // TODO: 创建长度为 n 的 vector
        // std::vector<float> buffer(n);
        // TODO: 使用 unique_ptr 管理
        auto unique_p = std::make_unique<std::vector<float>>(n);
        // TODO: 保存到 buffers_
        buffers_.emplace(id, std::move(unique_p));
        return id;
    }

    std::vector<float>& get(int id) {
        // TODO: 查找对应 Buffer
        auto it = buffers_.find(id);
        // TODO: ID 不存在时抛出异常
        if (it == buffers_.end()) {
            throw std::out_of_range("Buffer id not found: " + std::to_string(id));
        }

        return *it->second;
    }

    void release(int id) {
        // TODO: 删除指定 Buffer
        buffers_.erase(id);
    }

    std::size_t active_count() const {
        return buffers_.size();
    }
};

int main() {

    // 1. 创建两个 Buffer，容量分别为 100 和 200
    BufferPool pool;
    int id1 = pool.allocate(100);
    int id2 = pool.allocate(200);

    std::cout<< "activate_count = " << pool.active_count() << "\n";

    // 2. 修改其中一个 Buffer 的元素
    std::vector<float>& buf1 = pool.get(id1);
    buf1[0] = 1.5f;
    buf1[3] = 0.1f;
    buf1[7] = 99.9f;
    std::vector<float>& buf2 = pool.get(id2);
    buf2[0]   = 100.0f;
    buf2[199] = 200.0f;

    // 3. 通过 ID 获取并验证数据
    std::cout << "\n验证 id1 的数据:\n";
    std::cout << "  buf1[0]  = "<< pool.get(id1)[0]<<"\n";
    std::cout << "  buf1[3]  = "<< pool.get(id1)[3]<<"\n";
    std::cout << "  buf1[7]  = "<< pool.get(id1)[7]<<"\n";
    std::cout << "\n验证 id2 的数据:\n";
    std::cout << "  buf2[0]  = "<< pool.get(id1)[0]<<"\n";
    std::cout << "  buf2[199]  = "<< pool.get(id2)[199]<<"\n";

    // 4. 释放第一个 Buffer
    pool.release(id1);

    // 5. 验证 active_count() 从 2 变为 1
    std::cout<< "activate_count = " << pool.active_count()<<"\n";

    // 6. 访问已经释放的 ID 时，抛出异常而不是发生未定义行为
    try
    {
        auto p = pool.get(id1);
        p[0] = 1.0;
    }
    catch(const std::exception& e)
    {
        std::cout << "捕获异常: " << e.what() << '\n';
    }
    

    return 0;
}
