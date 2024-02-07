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
#include <unistd.h>
#include <iostream>
#include <unordered_map>
#include <map>

/* Conection Constants */
#define SERV_PORT 34543

/* Max Constants */
#define LISTENQ 1024
#define MAXLINE 4096
#define BUFFSIZE 8192

/* Acronym */
#define SA  struct sockaddr

class QuerryHandler {
private:
    bool         res;
    int          current_sock;
    std::string  *auth_username;
    std::string  query;
    ChatDatabase *DB;
    user_t       usr;
    std::string  cmd;
    size_t       l_pos, r_pos;
public:
    QuerryHandler(ChatDatabase *Q_DB);
    bool make_querry(int cur_sock, std::string q_query, std::string *username);
private:
    bool log_querry();
    bool reg_querry();
    bool sendto_querry();
};

class server
{
private:
    user_t             current_user;
    ChatDatabase       *DB;
    QuerryHandler      *querry;
    int                maxfd, listenfd, connfd;
    int                sockcount, client[FD_SETSIZE];
    std::map<int, std::string> clients;
    std::vector<int> sockToDelete; 
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
private:
    void exit_err(const char*);
    void init();
    void recieve(int, std::string*);
    void proccess();
    void Send();
};


#endif
