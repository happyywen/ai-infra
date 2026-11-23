
#include <cerrno>
#include <cstring>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    int fd[2];
    // fd[0]: read
    // fd[1]: write

    if (pipe(fd) == -1) {
        std::cerr << "pipe failed\n";
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        close(fd[0]);
        close(fd[1]);
        return 1;
    }

    if (pid == 0) {
        close(fd[1]);  // 子进程不写

        char buffer[10]{};

        // TODO: 循环 read(fd[0], ...)
        // TODO: 正确处理部分读取、EINTR 和 EOF
        // TODO: 输出接收到的消息
        // TODO: 关闭 fd[0]
        for(int i = 0; i < 5; ++i) {
            ssize_t n = read(fd[0], &buffer, sizeof(buffer));
            if (n == -1) {
                if (errno == EINTR) {
                    continue; //被信号中断，重试
                }
                std::cerr<< "Child: read failed: "<<strerror(errno)<<std::endl;
                break;
            }
            else if (n == 0) {
                // EOF: 父进程的写端全部关闭
                std::cout<< "Child: read EOF\n";
            }
            else {
                int written = write(STDERR_FILENO, buffer, n);
                if (written == -1) {
                    std::cout<< "Child: write failed\n";
                }
            }
        }
        if (close(fd[0]) == -1) {
            std::cout<< "Child: close fd[0] failed\n";
        }
        _exit(0);
    }

    close(fd[0]);  // 父进程不读

    const char msg[] = "Hello from parent!\n";

    // TODO: 循环 write(fd[1], ...)
    // TODO: 处理部分写入与 EINTR
    // TODO: 关闭 fd[1]，使子进程最终看到 EOF
    int i = 0;
    while(i < 5) {
        ssize_t written = write(fd[1], msg, sizeof(msg));
        if (written == -1) {
            if (errno == EINTR) {
                continue;
            }
            std::cerr<< "Parent: write failed" << strerror(errno)<< std::endl;
            break;
        }
        else if (written > 0) {
            std::cout<< "Parent: write sucess\n";
        }
        ++i;
    }
    if (close(fd[1]) == -1) {
        std::cout<< "Parent: close fd[1] failed\n";
    }
    int status = 0;
    if (waitpid(pid, &status, 0) == -1) {
        std::cerr << "Parent: waitpid failed\n";
        return 1;
    }

    return WIFEXITED(status) && WEXITSTATUS(status) == 0
        ? 0 : 1;
}
