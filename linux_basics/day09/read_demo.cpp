#include <fcntl.h>
#include <unistd.h>
#include <iostream>


int main() {
    pid_t pid = getpid();
    std::cout<<"pid: "<<pid<<std::endl;
    int fd = open("sample.txt", O_RDONLY);
    char buf[20];
    size_t n;
    for (int i = 0; i < 10; ++i) {
        n = read(fd, &buf, sizeof(buf));
        if (n == -1) {
            std::cout<<" read failed \n";
        }
        else if (n == 0) {
            std::cout<<" read EOF\n";
        }
        else {
            int res_write = write(STDERR_FILENO, buf, n);
            if (res_write == -1) {
                std::cout<<" write failed\n";
                break;
            }
            std::cout<<"read "<<n<<" bytes in a loop\n";
        }
    }
    sleep(60);
    if (close(fd) == -1) {
        std::cout<<" close failed\n";
    }
    return 0;
}


// TODO: 循环调用 read()
// TODO: 处理 read() 返回 -1、0 和正数
// TODO: 使用 write() 输出实际读取的字节
// TODO: close(fd)