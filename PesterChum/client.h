#ifndef CLIENT_H
#define CLIENT_H

#define WIN32_LEAN_AND_MEAN
#define MAXLINE 2048
#define SA struct sockaddr

#include <QMessageBox>
#include <QApplication>
#include <QWidget>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <string>

class client
{
private:
    SOCKET      clientSock;
    std::string servIp = "192.168.1.103";
    std::string Username;
    std::string Password;
    int         servPort = 34543;
    sockaddr_in servAddr;
    QWidget     *Parent;
    std::vector<char> buffer;
public:
    void fatal_err(QString);
    void err(QString);
    client(QWidget*);
    int reg_user(std::string, std::string);
    int log_user(std::string, std::string);
    int Connect();
    void Close();
    bool writeMessage(std::string);
    std::string readMessage();
};

#endif // CLIENT_H
