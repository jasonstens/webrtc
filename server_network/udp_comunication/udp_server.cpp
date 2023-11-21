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

    printf("Welcome! This is a UDP server.\n");
    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd < 0) {
        perror("create socket error!");
        exit(1);
    }

    struct sockaddr_in local_addr, remote_addr;
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(SOCK_PORT);
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bzero(&(local_addr.sin_zero), 8);

    ret = bind(socket_fd, (struct sockaddr*)&local_addr, sizeof(struct sockaddr_in));
    if (ret < 0) {
        perror("bind socket error!");
        exit(1);
    }

    char recvbuf[MESSAGE_LEN] = {0,};
    socklen_t sockaddr_len = sizeof(struct sockaddr_in);
    while(true) {
        ret = recvfrom(socket_fd, recvbuf, MESSAGE_LEN - 1, 0, (struct sockaddr*)&remote_addr, &sockaddr_len);
        if (ret <= 0) {
            perror("recv no data");
            break;
        }
        recvbuf[ret] = '\0';
        printf("recv data from client:%s %u says: %s\n", inet_ntoa(remote_addr.sin_addr), ntohs(remote_addr.sin_port), recvbuf);


        ret = sendto(socket_fd, recvbuf, ret, 0, (struct sockaddr*)&remote_addr, sockaddr_len);
        std::cout << "send data to client: " << recvbuf << std::endl;

        if (ret <= 0) {
            perror("send no data");
            break;
        }
    }
    close(socket_fd);

    return 0;
}
