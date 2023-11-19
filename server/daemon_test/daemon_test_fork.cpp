#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

void daemonize()
{
    int fd;
    // fork 创建子进程
    pid_t pid = fork();
    if (pid < 0) {
        std::cout << "fork process failed!" << std::endl;
        return;
    } else if (pid > 0) {
        // 父进程退出，子进程变成孤儿进程，由init进程接管
        exit(0);
    }
    
    // 创建新的进程会话, 以替换老的session， 应该使用被接管的init的session_id
    setsid();
    // 切换为根目录
    if (chdir("/") < 0) {
        std::cout << "change root dir failed!" << std::endl;
        exit(-1);
    }

    fd = open("/dev/null", O_RDWR);
    if (fd < 0) {
        exit(-1);
    }

    dup2(fd, STDIN_FILENO);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);
    return;
}

int main(int argc, char* argv[])
{

    daemonize();

    while(true) {
        sleep(1);
    }
    return 0;
}