#include "server.h"

server::server() : clilen{sizeof(cliaddr)}
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
    /*
    /reg/user/password - reg query
    /log/user/password - log query
    */
    size_t l_pos, r_pos;
    std::string cmd, q_user, q_password;

    r_pos = query.find('/', 1);
    cmd = {query.begin() + 1, query.begin() + r_pos};
    l_pos = r_pos + 1;
    r_pos = query.find('/', l_pos);
    current_user.login = {query.begin() + l_pos, query.begin() + r_pos};
    l_pos = r_pos + 1 ;
    current_user.password = {query.begin() + l_pos, query.end()};

    if (cmd == "reg") {
        if (!DB->reg_user(&current_user)){
            printf("%s's already taken!\n", current_user.login.c_str());
            return;
        }
        printf("%s registred!\n", current_user.login.c_str());
    }
    else if (cmd == "log") {
        if (!DB->log_user(&current_user)){
            printf("Check login or password\n");
            return;
        }
        printf("%s logged!\n", current_user.login.c_str());
    }
}

void server::recieve(int currentsockfd, std::string username) {
    memset(buf, 0, sizeof(buf));
    n = recv(currentsockfd, buf, MAXLINE, 0);
    if (n == 0 || n == -1) {
        FD_CLR(currentsockfd, &readset);
        FD_CLR(currentsockfd, &allset);
        sockToDelete.push_back(currentsockfd);

        getpeername(currentsockfd, (SA*) &cliaddr, &clilen);
        char ip[32];
        uint16_t port = htons(cliaddr.sin_port);
        inet_ntop(AF_INET, &cliaddr.sin_addr, ip, INET_ADDRSTRLEN);
        printf("'%s' disconected from <%s:%d>\n", username.c_str(), ip, port);
        close(currentsockfd);
        return;
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
        for (int sock : sockToDelete) {
            clients.erase(sock);   
        }
        sockToDelete.clear();
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
