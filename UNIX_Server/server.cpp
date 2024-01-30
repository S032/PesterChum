#include "server.h"

server::server() 
    :
    maxi{ -1 },
    clilen{sizeof(cliaddr)}
{
    FD_ZERO(&allset);
}

void server::exit_err(const char* err_text) {
    perror(err_text);
    exit(EXIT_FAILURE);
}

void server::init() {
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

    DB = new ChatDatabase;

    printf("Server initialized\n");
}

void server::query_handler(std::string query) {
    printf("it's query!\n");
}

void server::recieve(int currentsockfd, std::string username) {
    memset(buf, 0, sizeof(buf));
    if ((n = recv(currentsockfd, buf, MAXLINE, 0)) == 0) {
        close(currentsockfd);
        FD_CLR(currentsockfd, &allset);
        clients.erase(currentsockfd);

        char ip[32];
        uint16_t port = htons(cliaddr.sin_port);
        inet_ntop(AF_INET, &cliaddr.sin_addr, ip, INET_ADDRSTRLEN);
        printf("'%s' disconected form <%s:%d>\n", username, ip, port);
    }
    
    printf("%s\n", buf);
    if (buf[0] == '/') {
        std::string query(buf);
        query_handler(query);
        return;
    }

   for (const auto& [sock, username] : clients) {
        if (sock != currentsockfd)
            send(sock, buf, n, 0);
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

            if (clients.size() == FD_SETSIZE) {
                printf("Too many clients\n");
                exit(EXIT_FAILURE);
            }

            clients.insert({connfd, ""});

            FD_SET(connfd, &allset);
            if (connfd > maxfd)
                maxfd = connfd;

            char ip[32];
            uint16_t port = ntohs(cliaddr.sin_port);
            inet_ntop(AF_INET, &cliaddr.sin_addr, ip, INET_ADDRSTRLEN);
            printf("Client is connected from <%s:%d>\n", ip, port);

            sockcount--;
            if (sockcount <= 0)
                continue;
        }
        for (const auto& [sock, username] : clients) {
            if (FD_ISSET(sock, &readset)) {
                recieve(sock, username);
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
