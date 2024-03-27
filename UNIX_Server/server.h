#ifndef SERVER_H
#define SERVER_H

#include "DataBase/ChatDatabase.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <signal.h>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <iostream>
#include <utility>
#include <map>

/* Conection Constants */
#define SERV_PORT 34543

/* Max Constants */
#define LISTENQ 1024
#define MAXLINE 4096
#define BUFFSIZE 8192

/* Acronym */
#define SA  struct sockaddr

using client_t = std::map<std::string, std::pair<int, std::string>>;

struct user_status_t {
    std::string name;
    std::string status;
};

class QuerryHandler;

class server
{
private:
    user_t             current_user;
    ChatDatabase       *DB;
    QuerryHandler      *querry;
    int                maxfd, listenfd, connfd;
    int                sockcount, client[FD_SETSIZE];
    std::map<int, std::string> clients;
    client_t           clients_fast; 
    std::vector<int>   sockToDelete;
    std::vector<std::pair<int, user_t>> clientsToAdd;
    std::vector<std::string> clientsToDelete;
    ssize_t            n;
    fd_set             readset, allset;
    char               buf[MAXLINE];
    socklen_t          clilen;
    struct sockaddr_in cliaddr, servaddr;
public:
    server();
public:
    void start();
    void stop();
    bool SendMessage(int sockfd, std::string message);
    void send_to(int currentsockfd, std::string recipient_name, std::string message);
    bool send_to_fast(std::string username, std::string message);
    void sendMessageToFriends(std::string username, std::string message);
    void addClient(int cur_sock, user_t user);
    client_t getClients();
    client_t* getClientsAddr();
private:
    bool ReadMessage(int sockfd, void *buff);
    void exit_err(const char*);
    void init();
    void recieve(int, std::string*);
    void proccess();
};

class QuerryHandler {
private:
    bool         res;
    int          current_sock;
    std::string  answer;
    std::string  *auth_username;
    std::string  query;
    ChatDatabase *DB;
    server       *SV;
    user_t       usr;
    std::string  cmd;
    size_t       l_pos, r_pos;
public:
    QuerryHandler(ChatDatabase *Q_DB, server *Q_SV);
    bool make_querry(int cur_sock, std::string q_query, std::string *username);
private:
    user_t log_querry();
    user_t reg_querry();
    std::string sendto_querry();
    std::string giveListOfUser();
    std::string giveListOfIcReq();
    std::string giveListOfOgReq();
    std::string sendreq();
    std::string reqanswer();
    std::string deleteFriend();
    std::string changeStatus();
};

#endif
