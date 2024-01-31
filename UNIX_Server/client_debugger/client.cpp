#include "erproc.h"
#include <strings.h>
#include <string>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "usage: ./client <IP_ADRRES>" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string username;
    int sockfd[10];
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    servaddr.sin_port = htons(SERV_PORT);

    for (int i = 0; i < 10; i++) {
        sockfd[i] = Socket(AF_INET, SOCK_STREAM, 0);
        Connect(sockfd[i], (const struct sockaddr *) &servaddr, sizeof(servaddr));
        std::cout << "successfully connected" << std::endl;
    }
    str_cli(STDIN_FILENO, sockfd[0]);
}
