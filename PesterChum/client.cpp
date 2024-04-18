#include "client.h"

void client::giveNewParent(QMainWindow *NewParent)
{
    Parent = NewParent;
}

client::client(QWidget *parent)
    :
    servPort(34543),
    servIp("81.94.156.108")
    //84.201.157.25:26098 - vpn server
    //81.94.156.108:34543 - vds server
    //192.168.1.103:34543 - local server
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
    uint32_t messageLength = htonl(message.size());
    int needToSend = sizeof(uint32_t);
    int sendedLength;

    //sending a usefull information size
    while(needToSend > 0) {
        if((sendedLength = send(clientSock, reinterpret_cast<char*>(&messageLength), needToSend, 0)) <= 0) {
            return false;
        }
        needToSend -= sendedLength;
    }

    //sending a message
    messageLength = ntohl(messageLength);
    qDebug() << "sending message...";
    qDebug() << "messageLength: " << messageLength;
    while(messageLength > 0) {
        if((sendedLength = send(clientSock, message.c_str(), messageLength, 0)) <= 0) {
            return false;
        }
        messageLength -= sendedLength;
    }
    qDebug() << "sendtoserver: " << message;
    return true;
}

int client::readMessage(std::string &message) {
    uint32_t messageLength;
    int needToRead = sizeof(uint32_t);
    int readLength;
    char *readedMessage = nullptr;
    int totalReadLength = 0;

    //reading a useful infromation size
    while(needToRead > 0) {
        if ((readLength = recv(clientSock, reinterpret_cast<char*>(&messageLength), needToRead, 0)) <= 0) {
            delete[] readedMessage;
            emit throwFatalError("server prematurely terminated!");
            return readLength;
        }
        needToRead -= readLength;
    }
    messageLength = ntohl(messageLength);
    qDebug() << "messagelength: " << messageLength;
    readedMessage = new char[messageLength + 1];
    memset(readedMessage, 0, messageLength + 1);
    //reading a message
    while(messageLength > 0) {
        if ((readLength = recv(clientSock, readedMessage + totalReadLength, messageLength, 0)) <= 0) {
            delete[] readedMessage;
            emit throwFatalError("server prematurely terminated!");
            return readLength;
        }
        messageLength -= readLength;
        totalReadLength += readLength;
    }

    readedMessage[totalReadLength] = '\0';
    message = readedMessage;
    delete[] readedMessage;
    return totalReadLength;
}

int client::reg_user(std::string username, std::string password) {
    std::string querry = "/reg/" + username + "/" + password;
    if (!writeMessage(querry))
        return -1;
    std::string answer;
    readMessage(answer);
    if (answer == "/s") return 0;
    return -2;
}

int client::log_user(std::string username, std::string password) {
    std::string querry = "/log/" + username + "/" + password;
    if (!writeMessage(querry))
        return -1;
    std::string answer;
    readMessage(answer);
    qDebug() << answer;
    if (answer == "/status/smile") return 0;
    if (answer == "/status/angry") return 1;
    return -2;
}
