
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include <iostream>

int main() {
    int fd = open("sample.txt", O_RDONLY);

    if (fd == -1) {
        std::cerr << "open failed\n";
        return 1;
    }

    // linux 提供的结构体，存放文件元数据
    struct stat st{};

    // fstat: 根据已打开文件的文件描述符，读取文件元数据并且放到 st 结构体中
    if (fstat(fd, &st) == -1 || st.st_size <= 0) {
        std::cerr << "invalid file\n";
        close(fd);
        return 1;
    }

    void* addr = mmap(
        nullptr,
        static_cast<std::size_t>(st.st_size),
        PROT_READ,
        MAP_PRIVATE,
        fd,
        0
    );

    if (addr == MAP_FAILED) {
        std::cerr << "mmap failed\n";
        close(fd);
        return 1;
    }

    // TODO: 使用 write() 输出映射的文件内容
    int res_write = write(STDOUT_FILENO, addr, st.st_size);
    if (res_write == -1) {
        std::cout<<" write failed\n";
    }
    // TODO: 调用 munmap() 解除映射
    int res_munmap = munmap(addr, st.st_size);
    if (res_munmap == -1) {
        std::cout<<" munmap failed\n";
    }
    // TODO: 调用 close() 关闭文件描述符
    if (close(fd) == -1) {
        std::cout<<" close file failed\n";
    }

    return 0;
}
