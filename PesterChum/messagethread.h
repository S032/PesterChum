#ifndef MESSAGETHREAD_H
#define MESSAGETHREAD_H

#include <QThread>
#include <QObject>
#include <QMainWindow>
#include <string>
#include "client.h"
#include "messagemodel.h"

class MessageThread : public QThread
{
    Q_OBJECT
private:
    bool m_isRunning;
    client *cl;
    MessageModel *model;
    QMainWindow *main_w;
public:
    MessageThread(QObject *parent, client *m_cl);
    void stop();
protected:
    void run() override;
signals:
    void threadFinished();
    void resultReady(std::string result);
};

#endif // MESSAGETHREAD_H
