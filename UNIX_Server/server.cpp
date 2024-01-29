#include "server.h"

server::server() {
    for (i; i < FD_SETSIZE; i++)
        client[i] = -1;
    maxi = -1;
    FD_ZERO(&allset);
    clilen = sizeof(cliaddr);
}

void
server::exit_err(const char* err_text) {
    perror(err_text);
    exit(EXIT_FAILURE);
}

void
server::init() {
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    printf("Creating socket\n");
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        exit_err("Socket Failed");
    printf("Success\n");

    printf("Binding socket\n");
    if (bind(listenfd, (SA*) &servaddr, sizeof(servaddr)) < 0)
        exit_err("Bind Failed");
    printf("Success\n");

    printf("Server initialized\n");
}

void
server::recieve(int currentsockfd, int index) {
    memset(buf, 0, sizeof(buf));
    if ((n = recv(currentsockfd, buf, MAXLINE, 0)) == 0) {
        close(currentsockfd);
        FD_CLR(currentsockfd, &allset);
        client[index] = -1;

        char ip[32];
        uint16_t port = htons(cliaddr.sin_port);
        inet_ntop(AF_INET, &cliaddr.sin_addr, ip, INET_ADDRSTRLEN);
        printf("client is disconected form <%s:%d>\n", ip, port);
    }

    printf("%s\n", buf);

    int sockfd;
    for (int k = 0; k <= maxi; k++) {
        sockfd = client[k];
        if (sockfd < 0 || sockfd == currentsockfd)
            continue;
        if(FD_ISSET(sockfd, &allset))
        send(sockfd, buf, n, 0);
    }
}

void
server::proccess() {
    while(true) {
        readset = allset;
        sockcount = select(maxfd + 1, &readset, NULL, NULL, NULL);

        if (FD_ISSET(listenfd, &readset)) {
            if ((connfd = accept(listenfd, (SA*) &cliaddr, &clilen)) < 0)
                exit_err("Accept Failed");

            for (i = 0; i < FD_SETSIZE; i++) {
                if (client[i] < 0) {
                    client[i] = connfd;
                    break;
                }
            }
            if (i == FD_SETSIZE) {
                printf("Too many clients\n");
                exit(EXIT_FAILURE);
            }

            FD_SET(connfd, &allset);
            if (connfd > maxfd)
                maxfd = connfd;
            if (i > maxi)
                maxi = i;

            char ip[32];
            uint16_t port = ntohs(cliaddr.sin_port);
            inet_ntop(AF_INET, &cliaddr.sin_addr, ip, INET_ADDRSTRLEN);
            printf("Client is connected from <%s:%d>\n", ip, port);

            sockcount--;
            if (sockcount <= 0)
                continue;
        }
        for (i = 0; i <= maxi; i++) {
            if ((currentsockfd = client[i]) < 0)
                continue;
            if (FD_ISSET(currentsockfd, &readset)) {
                recieve(currentsockfd, i);
                sockcount--;
                if (sockcount <= 0)
                    break;
            }
        }
    }
}

void
server::start() {
    init();

    if (listen(listenfd, LISTENQ) < 0)
        exit_err("Listen Failed");
    printf("listening...\n");

    maxfd = listenfd;
    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);
    proccess();
}
