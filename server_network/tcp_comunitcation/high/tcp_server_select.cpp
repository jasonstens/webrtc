#include <errno.h>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>


#define TCP_PORT 8444
#define MAX_BACK_LOG_LEN 10
#define MAX_BUF_LEN 1024
#define ACCEPT_FD_MAX_SIZE 1024

int main()
{
    int ret = -1;
    int socket_fd = -1;
    int accept_fd = -1;
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    int on = 1;

    if (socket_fd < 0) {
        std::cout << "socketfd create failed!" << std::endl;
        exit(1); 
    }

    // 设置参数
    // 服务器关闭后会有个time_wait, 设置SO_REUSEADDR此参数后，可以建立新的socket不会一直等
    socklen_t on_size = sizeof(on);
    ret = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &on, on_size);
    if (ret < 0) { 
        perror("setsocketopt error!");
        exit(1);
    }
    // 设定ip port 结构体
    struct sockaddr_in local_addr, remote_addr;
    
    local_addr.sin_family = AF_INET;
    // 指定服务器上所有的ip地址都用于监听用户的socket
    local_addr.sin_addr.s_addr = INADDR_ANY;
    local_addr.sin_port = htons(TCP_PORT);
    bzero(&local_addr.sin_zero, 8);

    // bind 绑定端口和地址
    socklen_t addr_len = sizeof(struct sockaddr_in);
    ret = bind(socket_fd, (struct sockaddr*)&local_addr, addr_len);
    if (ret < 0) {
        std::cout << "bind socket failed!" << std::endl;
        exit(1);

    }
    // listen 监听
    ret = listen(socket_fd, MAX_BACK_LOG_LEN);
    if (ret < 0) {
        perror("listen socket failed!\n");
        exit(1);
    }
    // accept 接收，并创建用于数据收发的socket套接字
    char buf[MAX_BUF_LEN] = {0,};
    pid_t pid;

    // 设置为非阻塞模式
    int flags = fcntl(socket_fd, F_GETFL, 0);
    fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK);
    
    fd_set fd_sets;
    int max_fd = socket_fd;
    int accept_fds[ACCEPT_FD_MAX_SIZE];
    // accept fd初始化
    for (int i = 0; i < ACCEPT_FD_MAX_SIZE; ++i) {
        accept_fds[i] = -1;
    }

    // 将连接fd放入fdset中以监听新的连接过来
    int cur_pos = -1;
    int max_pos = 0;
    while (true) {

        FD_ZERO(&fd_sets);
        FD_SET(socket_fd, &fd_sets);
        for (int i = 0; i < max_pos; i++) {
            if (accept_fds[i] != -1) {
                if (max_fd < accept_fds[i]) {
                    max_fd = accept_fds[i];
                }
                printf("fd:%d, i:%d, max_fd:%d\n", accept_fds[i], i, max_fd);
                FD_SET(accept_fds[i], &fd_sets);
            }
        }

        int events = select(max_fd + 1, &fd_sets, NULL, NULL, NULL);
        if (events < 0) {
            perror("select fd error!");
            break;
        } else if (events == 0) {
            perror("time out!");
            continue;
        } else if (events > 0) {
            printf("events:%d\n", events);

            if (FD_ISSET(socket_fd, &fd_sets)) {
                printf("listen event :1\n");
                for (int i = 0; i < ACCEPT_FD_MAX_SIZE; i++) {
                    if (accept_fds[i] == -1) {
                        cur_pos = i;
                        break;
                    }
                }
                // 查看accept_fds数组是否满了，满了则不能再创建新连接了
                if (cur_pos == ACCEPT_FD_MAX_SIZE) {
                    std::cout << "the connection is full!\n" << std::endl;
                    continue;
                }
                // 创建新连接，待下一次循环再加入f_sets中，因为这是刚创建的连接，其发生的事件不会在本次select中监听到
                accept_fd = accept(socket_fd, (struct sockaddr*)&remote_addr, &addr_len);
                // 设置非阻塞
                flags = fcntl(accept_fd, F_GETFL, 0);
                fcntl(accept_fd, F_SETFL, flags | O_NONBLOCK);
                accept_fds[cur_pos] = accept_fd;

                if ((cur_pos + 1) > max_pos) {
                    max_pos = cur_pos + 1;
                }

                if (accept_fd > max_fd) {
                    max_fd = accept_fd;
                }
                printf("new connection fd:%d, curpos = %d \n",accept_fd, cur_pos);

            } else {
                for (int i = 0; i < max_pos; i++) {
                    if ((accept_fds[i] != -1) && FD_ISSET(accept_fds[i], &fd_sets)) {
                        // 由于设置的是非阻塞模式，则recv在无数据可读的情况下会立刻返回，不应该循环等待
                        // for (;;) {
                        memset(buf, 0, MAX_BUF_LEN);

                        // recv
                        ret = recv(accept_fds[i], (void *)buf, MAX_BUF_LEN, 0);
                        if (ret > 0) {
                            std::cout << "receive: " << buf << std::endl;
                            // send 回显 使用实际接收到的字节数
                            ret = send(accept_fds[i], (void *)buf, ret, 0);
                            std::cout << "send: " << buf << std::endl;
                        }
                        else {
                            if (ret == 0) {
                                // 连接已经正常关闭
                                std::cout << "Client disconnected\n";
                            } else if (errno != EAGAIN && errno != EWOULDBLOCK) {
                                // 发生了其他错误
                                perror("recv error");
                            }
                            close(accept_fds[i]);
                            accept_fds[i] = -1;
                        }
                    }
                }
            }
        }      
    
    }
    printf("quit server...\n");
    close(socket_fd);
    return 0;
}