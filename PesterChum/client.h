#ifndef CLIENT_H
#define CLIENT_H

#define WIN32_LEAN_AND_MEAN
#define MAXLINE 2048
#define SA struct sockaddr

#include <QMessageBox>
#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string>

class client
{
private:
    SOCKET      clientSock;
    int         servPort;
    std::string servIp;
    sockaddr_in servAddr;
    QWidget     *Parent;
    std::vector<char> buffer;
public:
    void giveNewParent(QMainWindow*);
    void fatal_err(QString);
    void err(QString);
    client(QWidget*);
    int reg_user(std::string, std::string);
    int log_user(std::string, std::string);
    int Connect();
    void Close();
    bool writeMessage(std::string);
    std::string readMessage();
private:
    void readIp(std::string);
};

#endif // CLIENT_H
