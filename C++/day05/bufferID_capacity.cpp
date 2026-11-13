#include <iostream>
#include <unordered_map>

int main() {

    std::unordered_map<int, std::size_t> buffers;

    // -----插入-----
    buffers[1] = 1024;
    buffers[2] = 4096;
    buffers[3] = 512;

    std::cout << "插入后，共有 " << buffers.size() << " 个 Buffer\n";

    // -----查询-----
    // 1.find()
    auto it = buffers.find(2);
    if (it != buffers.end()) {
        std::cout << "id=2 的容量是 " << it->second << '\n';
    }
    else {
        std::cout<<"id=2 不存在"<<"\n";
    }

    // 2.用count判断是否存在（ 返回 0 或 1 ）
    if (buffers.count(99) == 0) {
        std::cout<<"id=99 不存在"<<"\n";
    }

    // -----删除-----
    buffers.erase(1);
    std::cout << "删除 id=1 后，剩余 " << buffers.size() << " 个 Buffer\n";

    if (buffers.find(1) == buffers.end()) {
        std::cout<<"id=1 已删除\n";
    }

    return 0;
}