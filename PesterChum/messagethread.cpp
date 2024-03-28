#include "messagethread.h"

ThreadController::ThreadController(client *r_cl)
    :
    cl(r_cl)
{
    reader = new MessageReader;
    reader->moveToThread(&readerThread);
    connect(&readerThread, &QThread::finished, reader, &QObject::deleteLater);
    connect(this, &ThreadController::startRead, reader, &MessageReader::doWork);
    connect(reader, &MessageReader::itsMessage, this, &ThreadController::messageHandler);
    connect(reader, &MessageReader::itsAnswer, this, &ThreadController::answerHandler);
    connect(reader, &MessageReader::makeFatalError, this, &ThreadController::throwFatalErrorOccuried);
    readerThread.start();
}

ThreadController::~ThreadController()
{
    readerThread.quit();
    readerThread.wait();
}

void ThreadController::requestAnswerHandler(std::string answer)
{
    l_pos = r_pos + 1;
    r_pos = answer.find("/", l_pos);
    std::string recipient_name = {answer.begin() + l_pos, answer.begin() + r_pos};
    l_pos = r_pos + 1;
    std::string answ = {answer.begin() + l_pos, answer.end()};
    qDebug() << "rec_name: " << recipient_name;
    qDebug() << "answ: " << answ;
    if (answ == "accept") {

    }
    else if (answ == "denie") {

    }
    else if (answ == "exist") {
        emit throwError("запрос уже существует!");
    }
    else if (answ == "notexist") {
        emit throwError("такого пользователя не существует!");
    }
    else if (answ == "already") {
        emit throwError("пользователь уже ваш друг!");
    }
    emit getOgRequests();
}



void MessageReader::doWork(client *cl)
{
    while(m_isRunning) {
        std::string result;

        int z = cl->readMessage(result);
        if(z == 0) {
            m_isRunning = false;
            emit makeFatalError("Server terminated");
        }
        if(z == -1) {
            m_isRunning = false;
        }
        qDebug() << "server answer: " <<result;
        if (result[0] == '/') emit itsAnswer(result);
        else emit itsMessage(result);
    }
}

void ThreadController::messageHandler(std::string message)
{
    std::string sender = {message.begin(), message.begin() + message.find(":")};
    emit gotNewMessage(sender.c_str());
    emit messageReady(sender, message);
}

void ThreadController::answerHandler(std::string answer)
{
    if (answer == "/f") {

    }
    if (answer == "/s") return;
    std::string cmd;
    r_pos = l_pos = 0;
    if((r_pos = answer.find('/', 1)) == std::string::npos) return;
    cmd = {answer.begin() + 1, answer.begin() + r_pos};
    qDebug() << "cmd: " << cmd;
    if (cmd == "userlist") {
        std::string userlist = {answer.begin() + r_pos, answer.end()};
        emit listOfUsersReady(userlist);
    }
    else if (cmd == "icreqlist") {
        std::string icreqlist = {answer.begin() + r_pos, answer.end()};
        emit listOfIcReqReady(icreqlist);
    }
    else if (cmd == "ogreqlist") {
        std::string ogreqlist = {answer.begin() + r_pos, answer.end()};
        emit listOfOgReqReady(ogreqlist);
    }
    else if (cmd == "newreq") {
        emit getIcRequests();
    }
    else if (cmd == "reqanswer") {
        requestAnswerHandler(answer);
    }
    else if (cmd == "delfriend") {
        std::string notfriend_name; // добавить уведомление об удалении из др
        emit getUsers();
    }
    else if (cmd == "left") {
        emit getUsers();
        l_pos = r_pos + 1;
        std::string name = {answer.begin() + l_pos, answer.end()};
        emit sendLeaveMessage(name);
    }
    else if (cmd == "log") {
        emit getUsers();
        l_pos = r_pos + 1;
        if((r_pos = answer.find("/", l_pos)) == std::string::npos) return;
        std::string name = {answer.begin() + l_pos, answer.begin() + r_pos};
        emit sendJoinMessage(name);
    }
    else if (cmd == "chstatus") {
        emit getUsers();
    }
    else {
        qDebug() << "fuck";
    }
}

void ThreadController::throwFatalErrorOccuried(QString errortext)
{
    emit throwFatalError(errortext);
}
