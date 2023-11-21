#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define SOCK_PORT 9876
#define MESSAGE_LEN 1024
int main(int argc, char* argv[])
{
    int ret = -1;
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SOCK_PORT);
    server_addr.sin_addr.s_addr = inet_addr("192.168.91.131");
    bzero(&(server_addr.sin_zero), 8);

    printf("Welcome! This is a UDP client.\n");
    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd < 0) {
        perror("create socket error!");
        exit(1);
    }

    char sendbuf[MESSAGE_LEN] = {0,};
    char recvbuf[MESSAGE_LEN] = {0,};

    socklen_t sockaddr_len = sizeof(struct sockaddr_in);

    // 判断是否填入了IP地址
    if (server_addr.sin_addr.s_addr == INADDR_NONE)
    {
      printf("Incorrect ip address!");
      close(socket_fd);
      exit(1);
    }

    while(true) {
        memset(sendbuf, 0, MESSAGE_LEN);
        gets(sendbuf);

        if (strcmp(sendbuf, "quit") == 0) {
            std::cout << "quit success!" << std::endl;
            break;
        }
        ret = sendto(socket_fd, sendbuf, strlen(sendbuf), 0, (struct sockaddr*)&server_addr, sockaddr_len);
        std::cout << "send data to server: " << sendbuf << std::endl;
        if (ret <= 0) {
            perror("send no data");
            break;
        }
        ret = recvfrom(socket_fd, recvbuf, MESSAGE_LEN - 1, 0, (struct sockaddr*)&server_addr, &sockaddr_len);
        if (ret <= 0) {
            perror("recv no data");
            break;
        }
        recvbuf[ret] = '\0';
        printf("recv data from server:%s %u says: %s\n", inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port), recvbuf);
    }
    close(socket_fd);

    return 0;
}
