#include "client.h"

void
client::fatal_err(QString err) {
    QMessageBox::critical(Parent, "FATAL ERROR", err);
    QApplication::quit();
}

void
client::err(QString err) {
    QMessageBox::warning(Parent, "WARNING", err);
}

client::client(QWidget *parent) {
    buffer.reserve(MAXLINE);
    Parent = parent;
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        fatal_err("Failed to init Winsock");

    servAddr.sin_family = AF_INET;
    inet_pton(AF_INET, servIp.c_str(), &servAddr.sin_addr.s_addr);
    servAddr.sin_port = htons(servPort);
}

int
client::Connect() {
    if ((clientSock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
        fatal_err("Failed to create socket");
    int erStat;
    if ((erStat = connect(clientSock, (SA*) &servAddr, sizeof(servAddr))) == SOCKET_ERROR)
        err("Failed to connect to server");
    return erStat;
}

void
client::Close() {
    closesocket(clientSock);
}

bool client::writeMessage(std::string message)
{
    if(send(clientSock, message.c_str(), message.size(), 0) < 0)
        return false;
    return true;
}

std::string client::readMessage()
{
    std::vector<char> buff(MAXLINE);
    memset(buff.data(), 0, buff.size());
    if(recv(clientSock, buff.data(), buff.size(), 0) == 0) {
        fatal_err("Server terminated prematurely");
    }
    return buff.data();
}

int
client::reg_user(std::string username, std::string password) {
    std::string querry = "/reg/" + username + "/" + password;
    if (send(clientSock, querry.c_str(), querry.size(), 0) < 0)
        return -1;
    std::vector<char> buff(MAXLINE);
    recv(clientSock, buff.data(), buff.size(), 0);
    std::string answer(buff.data());
    if (answer == "/f") return -2;
    return 0;
}

int
client::log_user(std::string username, std::string password) {
    std::string querry = "/log/" + username + "/" + password;
    if (send(clientSock, querry.c_str(), querry.size(), 0) < 0)
        return -1;
    std::vector<char> buff(MAXLINE);
    recv(clientSock, buff.data(), buff.size(), 0);
    std::string answer(buff.data());
    if (answer == "/f") return -2;
    return 0;
}
