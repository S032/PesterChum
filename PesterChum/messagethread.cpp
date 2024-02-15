#include "messagethread.h"

MessageThread::MessageThread(QObject *parent, client *m_cl)
    :
    QThread(parent),
    m_isRunning(true),
    cl(m_cl)
{}

void MessageThread::run()
{
    while(m_isRunning) {
        std::string result;
        result = cl->readMessage();
        emit resultReady(result);
    }
}

void MessageThread::stop() {
    //m_isRunning = false;
}
