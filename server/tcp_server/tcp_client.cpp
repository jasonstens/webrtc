#include <iostream>

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>

#define SOCK_PORT 8444
#define MESSAGE_LEN 1024

int main(int argc, char* argv[]) 
{
    int ret = -1;

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        perror("create socket fd failed!");
        exit(1);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SOCK_PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    bzero(&(server_addr.sin_zero), 8);

    ret = connect(socket_fd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr_in));
    if (ret < 0) {
        perror("connect socket fd failed!");
        exit(1);
    }
    printf("success to connect server...\n");

    char sendbuf[MESSAGE_LEN] = {0,};
    char recvbuf[MESSAGE_LEN] = {0,};

    while (true) {
        memset(sendbuf, 0, MESSAGE_LEN);

        if (strcmp(sendbuf, "quit") == 0) {
            break;
        }
        gets(sendbuf);

        ret = send(socket_fd, sendbuf, strlen(sendbuf), 0);
        if (ret <= 0) {
            perror("send socket error!");
            exit(1);
        }

        std::cout << "send msg: " << sendbuf << std::endl;
        recvbuf[0] = '\0';

        ret = recv(socket_fd, recvbuf, MESSAGE_LEN, 0);
        if (ret <= 0) {
            perror("recv socket error!");
            exit(1);
        }
        recvbuf[ret] = '\0';
        std::cout << "recv msg: " << recvbuf << std::endl;
    }
    close(socket_fd);

}