#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>

#define TCP_PORT 8444
#define MAX_BACK_LOG_LEN 10
#define MAX_BUF_LEN 1024

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
    while (true) {
        accept_fd = accept(socket_fd, (struct sockaddr*)&remote_addr, &addr_len);

        for (;;) {
            memset(buf, 0, MAX_BUF_LEN);

            // recv
            ret = recv(accept_fd, (void *)buf, MAX_BUF_LEN, 0);
            if (ret <= 0) {
                break;
            }
            std::cout << "receive: " << buf << std::endl;
            // send 回显
            ret = send(accept_fd, (void *)buf, MAX_BUF_LEN, 0);
            std::cout << "send: " << buf << std::endl;

        }
        printf("close socket connection...\n");
        close(accept_fd);
    }
    printf("quit server...\n");
    close(socket_fd);
    return 0;
}