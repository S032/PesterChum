#ifndef CLIENT_H
#define CLIENT_H

#define WIN32_LEAN_AND_MEAN
#define MAXLINE 2048
#define SA struct sockaddr

#include <QMessageBox>
#include <QApplication>
#include <QWidget>
#include <QObject>
#include <QMainWindow>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <string>

class client : public QObject
{
    Q_OBJECT
private:
    SOCKET      clientSock;
    int         servPort;
    std::string servIp;
    sockaddr_in servAddr;
    std::vector<char> buffer;
    QWidget     *Parent;
public:
    bool *m_isRunning;
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
    bool readMessage(std::string &);
signals:
    void throwFatalError(QString errortext);
    void throwError(QString errortext);
};

#endif // CLIENT_H
