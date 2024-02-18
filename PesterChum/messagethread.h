#ifndef MESSAGETHREAD_H
#define MESSAGETHREAD_H

#include <QThread>
#include <QObject>
#include <QMainWindow>
#include <string>
#include "client.h"
#include "messagemodel.h"

class MessageReader : public QObject
{
    Q_OBJECT
public:
    bool m_isRunning = true;
public slots:
    void doWork(client *cl);
signals:
    void resultReady(std::string message);
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
public:
    ThreadController(client *r_cl, MessageModel *r_model);
    ~ThreadController();
public slots:
    void handleResults(std::string message);
    void throwFatalErrorOccuried(QString errortext);
signals:
    void startRead(client *cl);
    void throwFatalError(QString errortext);
};

#endif // MESSAGETHREAD_H
