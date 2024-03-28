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

    printf("Server started on %d at ", SERV_PORT);

    std::cout << getTime() << std::endl;
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

    SV->SendMessage(cur_sock, answer, *(auth_username));
    return true;
}

void server::send_to(int currentsockfd, std::string recipient_name, std::string message) {
    for (const auto& [sock, username] : clients) {
        if (username == recipient_name)
            SendMessage(sock, message, username);
        }
}

bool server::send_to_fast(std::string username, std::string message) {
    if (clients_fast.find(username) == clients_fast.end())
        return false;
    SendMessage(clients_fast[username].first, message, username);
    return true;
}

void server::sendMessageToFriends(std::string username, std::string message) {
    for (std::string friend_name : DB->getFriendList(username)) {
        if (clients_fast.find(friend_name) != clients_fast.end())
            SendMessage(clients_fast[friend_name].first, message, friend_name);
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

bool server::ReadMessage(int sockfd, void *buff) {
    uint32_t messageLength;
    ssize_t needToRead = sizeof(uint32_t);
    ssize_t readLength;
    //char *readedMessage = nullptr;
    uint32_t totalLength = 0;

    //reading a usefull information size
    while(needToRead > 0) {
        if ((readLength = recv(sockfd, &messageLength, needToRead, 0)) <= 0) return false;
        needToRead -= readLength;
    }
    messageLength = ntohl(messageLength);
    //readedMessage = new char[messageLength + 1];
    memset(buff, 0, messageLength);
    std::cout << "reading a message...\n";
    std::cout << "messageLength: " << messageLength << std::endl;


    //reading a message
    while(messageLength > 0) {
        if ((readLength = recv(sockfd, static_cast<char*>(buff) + totalLength, messageLength, 0)) <= 0) {
            //delete[] readedMessage;
            return false;
        }
        messageLength -= readLength;
        totalLength += readLength;
    }
    //readedMessage[totalLength] = '\0';
    //buff = static_cast<void*>(readedMessage);
    //delete[] readedMessage;
    return true;
}

bool server::SendMessage(int sockfd, std::string message, std::string recipient_name) {
    uint32_t messageLength = htonl(message.size());
    int needToSend = sizeof(uint32_t);
    int sendedLength;

    //sending a useful information size
    while(needToSend > 0) {
        if ((sendedLength = send(sockfd, &messageLength, needToSend, 0)) <= 0)
            return false;
        needToSend -= sendedLength;
    }
    //sending a message
    messageLength = ntohl(messageLength);
    std::cout << "sending a message...\n";
    std::cout << "messageLength: " << messageLength << std::endl; 
    while(messageLength > 0) {
        if ((sendedLength = send(sockfd, message.c_str(), message.size(), 0)) <= 0)
            return false;
        messageLength -= sendedLength;
    }
    std::cout << "sended["+recipient_name+"]: "+message+" "+getTime() << std::endl;
    return true;
}

std::string server::getTime() {
    std::time_t currentTime = std::time(nullptr);
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "[%d-%m-%Y %H:%M:%S]", std::localtime(&currentTime));
    return std::string(buffer);
}

void server::recieve(int currentsockfd, std::string *username) {
    memset(buf, 0, sizeof(buf));
    if (!ReadMessage(currentsockfd, buf)) {
        FD_CLR(currentsockfd, &readset);
        FD_CLR(currentsockfd, &allset);
        sockToDelete.push_back(currentsockfd);
        if(*(username) != "") {
            clientsToDelete.push_back(*(username));
            std::string message = "/left/" + *(username);
            sendMessageToFriends(*(username), message);
            DB->writeLastStatus(*(username), clients_fast[*(username)].second);
        }
        std::cout << *(username)+"'s disconected at " << getTime() << std::endl;

        close(currentsockfd);
        return;
    }
    
    std::cout << "("+*(username)+"): "+buf+" "+getTime() << std::endl; 
    if (buf[0] == '/') {
        std::string query(buf);
        querry->make_querry(currentsockfd, query, username);
        return;
    }

   if (clients[currentsockfd] != "") {
        for (const auto& [sock, username] : clients) {
            if (sock != currentsockfd && username != "")
                SendMessage(sock, buf, username);
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
            std::cout << "client's connected from <"<<ip<<":"<<port<<"> at "
                                                  << getTime() << std::endl;
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
