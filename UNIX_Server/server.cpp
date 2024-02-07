#include "server.h"

server::server() : clilen{sizeof(cliaddr)}
{
    FD_ZERO(&allset);
}

QuerryHandler::QuerryHandler(ChatDatabase *Q_DB) : DB{Q_DB}{};

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
    querry = new QuerryHandler(DB);

    printf("Server initialized\n");
}

bool QuerryHandler::log_querry() {
    l_pos = r_pos + 1;
    if ((r_pos = query.find('/', l_pos)) == std::string::npos)
        return false;
    usr.login = {query.begin() + l_pos, query.begin() + r_pos};
    l_pos = r_pos + 1 ;
    usr.password = {query.begin() + l_pos, query.end()};
    res = DB->log_user(&usr);
    if (res) *(auth_username) = usr.login;
    return res;
}

bool QuerryHandler::reg_querry() {
    l_pos = r_pos + 1;
    if ((r_pos = query.find('/', l_pos)) == std::string::npos)
        return false;
    usr.login = {query.begin() + l_pos, query.begin() + r_pos};
    l_pos = r_pos + 1 ;
    usr.password = {query.begin() + l_pos, query.end()};
    res = DB->reg_user(&usr);
    if (res) *(auth_username) = usr.login;
    return res;
}

bool QuerryHandler::sendto_querry() {
    /*/sendto/from_whom/to_whom/message*/
    return true;
}

bool QuerryHandler::make_querry(int cur_sock, std::string q_query, std::string *username) {
    auth_username = username;
    r_pos = l_pos = 0;
    query = q_query;
    bool result;
    if (query.size() <= 1) return false;
    if ((r_pos = query.find('/', 1)) == std::string::npos)
        return false;
    cmd = {query.begin() + 1, query.begin() + r_pos};
    if (cmd == "reg") {
        return reg_querry(); 
    }
    else if (cmd == "log") {
        return log_querry();
    }
    else if (cmd == "sendto") {
        std::cout << "it's send\n";
    }
    else {
        std::cout << "fuck\n";
    }
    return true;
}

void server::recieve(int currentsockfd, std::string *username) {
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
        printf("'%s' disconected from <%s:%d>\n", username->c_str(), ip, port);
        close(currentsockfd);
        return;
    }
    
    printf("%s\n", buf);
    if (buf[0] == '/') {
        std::string query(buf), answer, confirm_username;
        answer = "/f";
        /*
        if (query_handler(currentsockfd, query, username)) {
            answer = "/s";
        }
        */
        if (querry->make_querry(currentsockfd, query, username)) {
            answer = "/s";
        }
        send(currentsockfd, answer.c_str(), 2, 0);
        return;
    }

   if (clients[currentsockfd] != "") {
        for (const auto& [sock, username] : clients) {
            if (sock != currentsockfd && username != "")
                send(sock, buf, n, 0);
        }
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
        for (auto& [sock, username] : clients) {
            if (FD_ISSET(sock, &readset)) {
                recieve(sock, &username);
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
