#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

void daemonise()
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
    
    // 切换为根目录
    if (chdir("/") == -1) {
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

    daemonise();

    while(true) {
        sleep(1);
    }
    return 0;
}