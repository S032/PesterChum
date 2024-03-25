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

user_t QuerryHandler::log_querry() {
    user_t newuser = {"", "", ""};
    l_pos = r_pos + 1;
    if ((r_pos = query.find('/', l_pos)) == std::string::npos) 
        return newuser;
    newuser.login = {query.begin() + l_pos, query.begin() + r_pos};
    l_pos = r_pos + 1 ;
    newuser.password = {query.begin() + l_pos, query.end()};
    if (DB->log_user(&newuser)) {
        answer = "/status/" + newuser.status;
        SV->addClient(current_sock, newuser);
        std::string message = "/log/" + newuser.login + "/" + newuser.status;
        SV->sendMessageToFriends(newuser.login, message);
    }
    return newuser;
}

user_t QuerryHandler::reg_querry() {
    user_t newuser = {"", "", ""};
    l_pos = r_pos + 1;
    if ((r_pos = query.find('/', l_pos)) == std::string::npos)
        return newuser;
    newuser.login = {query.begin() + l_pos, query.begin() + r_pos};
    l_pos = r_pos + 1 ;
    newuser.password = {query.begin() + l_pos, query.end()};
    newuser.status = "smile";
    if (DB->reg_user(&newuser)) {
        *(auth_username) = newuser.login;
        answer = "/s";
        SV->addClient(current_sock, newuser);
    }
    return newuser;
}

std::string QuerryHandler::giveListOfUser() {
    answer = "/userlist/";
    client_t onlineList = SV->getClients();
    std::vector<std::string> friends = DB->getFriendList(*(auth_username));
    for (std::string friend_name : friends) {
        user_status_t friendZ;
        friendZ.name = friend_name;
        if (onlineList.find(friend_name) != onlineList.end()) {
            friendZ.status = onlineList[friend_name].second;
        }
        else {
            friendZ.status = "ofline";
        }
        answer += friendZ.name + "/" + friendZ.status + "/";
    } 
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
    SV->send_to_fast(recipient, message);
    return answer;
}

std::string QuerryHandler::sendreq() {
    std::string recipient;
    l_pos = r_pos + 1;
    //getting username
    recipient = {query.begin() + l_pos, query.end()};
    //trying add req in database
    if(DB->addRequest(*(auth_username), recipient, &answer)) {
        SV->send_to_fast(recipient, "/newreq/");
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
                SV->send_to_fast(requester
                    , "/reqanswer/" + *(auth_username) + "/accept");
            }
            else {
                SV->send_to_fast(requester
                    , "/reqanswer/" + requester + "/notexist");
            }
        }        
    }
    else if (req_answer == "denied") {
        if (DB->delRequest(requester, *(auth_username), &answer)) {
            SV->send_to_fast(requester
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
        SV->send_to_fast(friend_name , "/delfriend/"+*(auth_username));
        answer = "/s";
    }
    return answer;
}

std::string QuerryHandler::changeStatus() {
    std::string              status, username;
    client_t                 *clients;

    l_pos = r_pos + 1;
    status = {query.begin() + l_pos, query.end()};
    username = *(auth_username);
    clients = SV->getClientsAddr();

    if (status == "smile" || status == "angry" ) { //если станет больше статусов придумать что то получше
        (*clients)[username].second = status;
        SV->sendMessageToFriends(username, "/chstatus/" + username + "/" + status);
        return answer = "/s";
    }
    return answer = "/f";
}

bool QuerryHandler::make_querry(int cur_sock, std::string q_query, std::string *username) {
    answer = "/f";
    auth_username = username;
    current_sock = cur_sock;
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
        user_t log_usr = log_querry();
        *(username) = log_usr.login;
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
    else if (cmd == "chstatus") {
        changeStatus();
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

bool server::send_to_fast(std::string username, std::string message) {
    if (clients_fast.find(username) == clients_fast.end())
        return false;
    send(clients_fast[username].first, message.c_str(), message.size(), 0);
    return true;
}

void server::sendMessageToFriends(std::string username, std::string message) {
    for (std::string friend_name : DB->getFriendList(username)) {
        if (clients_fast.find(friend_name) != clients_fast.end())
            send(clients_fast[friend_name].first, message.c_str(), message.size(), 0);
        else 
            std::cout << friend_name << " is ofline\n";
    }
}

void server::addClient(int cur_sock, user_t user) {
    clientsToAdd.push_back(std::make_pair(cur_sock, user));
}

client_t server::getClients() {
    return clients_fast;
}

client_t* server::getClientsAddr() {
    return &clients_fast;
}

void server::recieve(int currentsockfd, std::string *username) {
    memset(buf, 0, sizeof(buf));
    n = recv(currentsockfd, buf, MAXLINE, 0);
    if (n == 0 || n == -1) {
        FD_CLR(currentsockfd, &readset);
        FD_CLR(currentsockfd, &allset);
        sockToDelete.push_back(currentsockfd);
        if(*(username) != "") {
            clientsToDelete.push_back(*(username));
            std::string message = "/left/" + *(username);
            sendMessageToFriends(*(username), message);
            DB->writeLastStatus(*(username), clients_fast[*(username)].second);
        }

        printf("'%s' disconected\n", username->c_str());
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
        for (std::string username : clientsToDelete) {
            clients_fast.erase(username);
        }
        for (std::pair<int, user_t> user : clientsToAdd) {
            clients_fast.insert({user.second.login
                                 , std::make_pair(user.first
                                                  , user.second.status)});
        }
        clientsToAdd.clear();
        clientsToDelete.clear();
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
