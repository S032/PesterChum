#ifndef ERPROC
#define ERPROC
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>

/* Conection Constants */
#define LOCAL_IP "127.0.0.1"
#define SERV_IP "192.168.1.103"
#define SERV_PORT 34543

/* Max Constants */
#define LISTENQ 1024
#define MAXLINE 4096
#define BUFFSIZE 8192

/* Acronym */
#define SA  struct sockaddr

void err_sys(const char*);

void err_quit(const char* err_text);

int Socket(int domain, int type, int protocol);

void Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

void Listen(int sockfd, int backlog);

int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

void Inet_pton(int af, const char *scr, void *dst);

void Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

ssize_t readn(int fd, void *vptr, size_t n);

ssize_t writen(int fd, const void *vptr, size_t n);

void str_serv(int sockfd);

void str_cli(int fp, int sockfd);

#endif
