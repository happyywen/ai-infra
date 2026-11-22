
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    int value = 10;

    pid_t pid = fork();

    if (pid < 0) {
        std::cerr << "fork failed\n";
        return 1;
    }

    if (pid == 0) {
        value = 100;

        std::cout << "Child PID: " << getpid()
                  << ", PPID: " << getppid()
                  << ", value: " << value 
                  << ", $value: " << &value << std::endl;

        _exit(0);
    }

    // TODO 1: 使用 waitpid() 等待子进程
    int status;
    pid_t wait_res =  waitpid(pid, &status, 0);
    // TODO 2: 检查 waitpid() 是否失败
    if (wait_res == -1){
        std::cout<<"waitpid 失败";
    }
    else {
        std::cout<<"子进程："<<pid<<"运行结束\n";
        // TODO 3: 使用 WIFEXITED / WEXITSTATUS 检查退出状态
        if (WIFEXITED(status)){
            std::cout<<"子进程退出码："<<WEXITSTATUS(status)<<std::endl;
        }
        else if (WIFSIGNALED(status)) {
            std::cout<<"子进程被信号终止："<<WTERMSIG(status)<<std::endl;
        }
    }
    // TODO 4: 输出父进程的 value
    std::cout<<"父进程 value: "<<value<<" &value: "<< &value <<std::endl;
    return 0;
}
