#ifndef MESSAGETHREAD_H
#define MESSAGETHREAD_H

#include <QThread>
#include "client.h"
#include "messagemodel.h"

class MessageThread : public QThread
{
private:
    bool threadIsRunning;
    client *cl;
    MessageModel *model;
public:
    MessageThread();
    //void run();
    void stop();
private:
    void GotMessageHandler();
};

#endif // MESSAGETHREAD_H
