#include "server.h"

server::server() : clilen{sizeof(cliaddr)}
{
    FD_ZERO(&allset);
}

QuerryHandler::QuerryHandler(ChatDatabase *Q_DB, server *Q_SV)
    :
    DB(Q_DB),
    SV(Q_SV){};

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
    querry = new QuerryHandler(DB, this);

    printf("Server started on %d\n", SERV_PORT);
}

std::string QuerryHandler::log_querry() {
    l_pos = r_pos + 1;
    if ((r_pos = query.find('/', l_pos)) == std::string::npos) 
        return answer;
    usr.login = {query.begin() + l_pos, query.begin() + r_pos};
    l_pos = r_pos + 1 ;
    usr.password = {query.begin() + l_pos, query.end()};
    if (DB->log_user(&usr)) *(auth_username) = usr.login, answer = "/s";
    return answer;
}

std::string QuerryHandler::reg_querry() {
    l_pos = r_pos + 1;
    if ((r_pos = query.find('/', l_pos)) == std::string::npos)
        return answer;
    usr.login = {query.begin() + l_pos, query.begin() + r_pos};
    l_pos = r_pos + 1 ;
    usr.password = {query.begin() + l_pos, query.end()};
    if (DB->reg_user(&usr)) *(auth_username) = usr.login, answer = "/s";
    return answer;
}

std::string QuerryHandler::giveListOfUser() {
    DB->getListOfUsers(&answer, *(auth_username)); 
    return answer;
}

std::string QuerryHandler::giveListOfIcReq() {
    DB->getListOfIcReq(&answer, *(auth_username)); 
    return answer;
}

std::string QuerryHandler::giveListOfOgReq() {
    DB->getListOfOgReq(&answer, *(auth_username)); 
    return answer;
}

std::string QuerryHandler::sendto_querry() {
    std::string recipient;
    std::string message = "";
    l_pos = r_pos + 1;
    if (*(auth_username) == "") return answer;
    //getting username
    if ((r_pos = query.find('/', l_pos)) == std::string::npos)
        return answer;
    recipient = {query.begin() + l_pos, query.begin() + r_pos};
    //getting message
    l_pos = r_pos + 1;
    message = {query.begin() + l_pos, query.end()};
    message = *(auth_username) + ": " + message;
    //sending
    if (!DB->check_user_in_chattable(*(auth_username), recipient)) return answer;
    answer = "/s";
    SV->send_to(current_sock, recipient, message);
    return answer;
}

std::string QuerryHandler::sendreq() {
    std::string recipient;
    l_pos = r_pos + 1;
    //getting username
    recipient = {query.begin() + l_pos, query.end()};
    //trying add req in database
    if(DB->addRequest(*(auth_username), recipient, &answer)) {
        SV->send_to(current_sock, recipient, "/newreq/");
    }
    return answer;
}

std::string QuerryHandler::reqanswer() {
    std::string requester, req_answer;
    l_pos = r_pos + 1;
    //getting requester name
    if ((r_pos = query.find('/', l_pos)) == std::string::npos)
        return answer;
    requester = {query.begin() + l_pos, query.begin() + r_pos};
    l_pos = r_pos + 1;
    req_answer = {query.begin() + l_pos, query.end()};
    if (req_answer == "accept") {
        if (DB->delRequest(requester, *(auth_username), &answer)) {
            if (DB->makeChat(*(auth_username), requester)) {
                SV->send_to(current_sock, requester
                , "/reqanswer/" + *(auth_username) + "/accept");
            }
            else {
                SV->send_to(current_sock, requester
                , "/reqanswer/" + requester + "/notexist");
            }
        }        
    }
    else if (req_answer == "denied") {
        if (DB->delRequest(requester, *(auth_username), &answer)) {
            SV->send_to(current_sock, requester
            , "/reqanswer/" + *(auth_username) + "/denied");
        }
    }
    else {
        return answer;
    }

    std::cout << "requester: " << requester << std::endl;
    std::cout << "req answer: " << req_answer << std::endl;
    return answer;
}

std::string QuerryHandler::deleteFriend() {
    std::string friend_name;
    l_pos = r_pos + 1;
    //getting username
    friend_name = {query.begin() + l_pos, query.end()};
    if (DB->delChat(*(auth_username), friend_name)) {
        SV->send_to(current_sock, friend_name , "/delfriend/"+*(auth_username));
        answer = "/s";
    }
    return answer;
}

bool QuerryHandler::make_querry(int cur_sock, std::string q_query, std::string *username) {
    /* 
    /reg/login/password - reg new acc
    /log/login/password - log in acc
    /sendto/to_whom/message - send message to user
    /sendreq/to_whom - send req to user
    /getlist/ - get list of friends
    /getreq/ - get list of requests
    /reqanswer/to_whom/(accept or denied) - answer to request
    */
    answer = "/f";
    auth_username = username;
    r_pos = l_pos = 0;
    query = q_query;
    bool result;
    if (query.size() <= 1) return false;
    if ((r_pos = query.find('/', 1)) == std::string::npos)
        return false;
    cmd = {query.begin() + 1, query.begin() + r_pos};
    if (cmd == "reg") {
        reg_querry(); 
    }
    else if (cmd == "log") {
        log_querry();
    }
    else if (*(auth_username) == "") {
        answer = "you have no right for this...";
    }
    else if (cmd == "sendto") {
        sendto_querry();
    }
    else if (cmd == "getlist") {
        giveListOfUser();
    }
    else if (cmd == "geticreq") {
        giveListOfIcReq();
    }
    else if (cmd == "getogreq") {
        giveListOfOgReq();
    }
    else if (cmd == "sendreq") {
        sendreq();
    }
    else if (cmd == "reqanswer") {
        reqanswer();
    }
    else if (cmd == "delfriend") {
        deleteFriend();
    }
    else {
        std::cout << "fuck\n";
    }
    std::cout << answer << std::endl;
    send(cur_sock, answer.c_str(), answer.size(), 0);
    return true;
}

void server::send_to(int currentsockfd, std::string recipient_name, std::string message) {
    for (const auto& [sock, username] : clients) {
        if (username == recipient_name)
            send(sock, message.c_str(), message.size(), 0);
        }
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
        std::string query(buf);
        querry->make_querry(currentsockfd, query, username);
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
