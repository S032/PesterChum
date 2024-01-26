#ifndef SERVER_H
#define SERVER_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

/* Conection Constants */
#define SERV_PORT 34543

/* Max Constants */
#define LISTENQ 1024
#define MAXLINE 4096
#define BUFFSIZE 8192

/* Acronym */
#define SA  struct sockaddr

class server
{
private:
    int i, maxi, maxfd, listenfd, connfd, currentsockfd;
    int sockcount, client[FD_SETSIZE];
    ssize_t n;
    fd_set readset, allset;
    char buf[MAXLINE];
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;
public:
    server();
public:
    void start();
    void stop();
private:
    void exit_err(const char*);
    void init();
    void recieve(int, int);
    void proccess();
    void Send();
};


#endif
