#ifndef MESSAGETHREAD_H
#define MESSAGETHREAD_H

#include <QThread>
#include <QObject>
#include <QMainWindow>
#include <string>
#include "client.h"
#include "customItemModel.h"

class MessageReader : public QObject
{
    Q_OBJECT
public:
    bool m_isRunning = true;
public slots:
    void doWork(client *cl);
signals:
    void itsMessage(std::string message);
    void itsAnswer(std::string message);
    void makeFatalError(QString errortext);
};

class ThreadController : public QObject
{
    Q_OBJECT
private:
    QThread readerThread;
    MessageReader *reader;
    client *cl;
    MessageModel *model;
    size_t r_pos, l_pos;
public:
    ThreadController(client *r_cl);
    ~ThreadController();
private:
    void requestAnswerHandler(std::string answer);
public slots:
    void messageHandler(std::string message);
    void answerHandler(std::string answer);
    void throwFatalErrorOccuried(QString errortext);
signals:
    void gotNewMessage(QString name);
    void sendLeaveMessage(std::string name);
    void sendJoinMessage(std::string name);
    void messageReady(std::string sender_name, std::string message);
    void listOfUsersReady(std::string userlist);
    void listOfIcReqReady(std::string icreqlist);
    void listOfOgReqReady(std::string ogreqlist);
    void getUsers();
    void getIcRequests();
    void getOgRequests();
    void startRead(client *cl);
    void throwFatalError(QString errortext);
    void throwError(QString errortext);
};

#endif // MESSAGETHREAD_H
