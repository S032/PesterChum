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

bool
client::reg_user(std::string username, std::string password) {
    std::string querry = "reg :" + username + ":" + password;
    if (send(clientSock, querry.c_str(), querry.size(), 0) < 0)
        err("regestration failed");
}
