#include "client.h"

void client::giveNewParent(QMainWindow *NewParent)
{
    Parent = NewParent;
}

client::client(QWidget *parent)
    :
    servPort(34543),
    servIp("192.168.1.103")
    //84.201.157.25:26098 - real server
{
    buffer.reserve(MAXLINE);
    Parent = parent;
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        emit throwFatalError("Failed to init Winsock");

    servAddr.sin_family = AF_INET;
    inet_pton(AF_INET, servIp.c_str(), &servAddr.sin_addr.s_addr);
    servAddr.sin_port = htons(servPort);
}

int
client::Connect() {
    if ((clientSock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
        emit throwFatalError("Failed to create socket");
    int erStat;
    if ((erStat = ::connect(clientSock, (SA*) &servAddr, sizeof(servAddr))) == SOCKET_ERROR)
        emit throwError("Failed to connect to server");
    return erStat;
}

void
client::Close() {
    closesocket(clientSock);
}

bool client::writeMessage(std::string message) {
    if(send(clientSock, message.c_str(), message.size(), 0) < 0)
        return false;
    return true;
}

int client::readMessage(std::string &message) {
    std::vector<char> buff(MAXLINE);
    memset(buff.data(), 0, buff.size());
    int res = recv(clientSock, buff.data(), buff.size(), 0);
    message = buff.data();
    return res;
}

int client::reg_user(std::string username, std::string password) {
    std::string querry = "/reg/" + username + "/" + password;
    if (send(clientSock, querry.c_str(), querry.size(), 0) < 0)
        return -1;
    std::vector<char> buff(MAXLINE);
    recv(clientSock, buff.data(), buff.size(), 0);
    std::string answer(buff.data());
    if (answer == "/f") return -2;
    return 0;
}

int client::log_user(std::string username, std::string password) {
    std::string querry = "/log/" + username + "/" + password;
    if (send(clientSock, querry.c_str(), querry.size(), 0) < 0)
        return -1;
    std::vector<char> buff(MAXLINE);
    recv(clientSock, buff.data(), buff.size(), 0);
    std::string answer(buff.data());
    if (answer == "/f") return -2;
    return 0;
}
