#include "messagethread.h"

ThreadController::ThreadController(client *r_cl)
    :
    cl(r_cl)
{
    reader = new MessageReader;
    reader->moveToThread(&readerThread);
    connect(&readerThread, &QThread::finished, reader, &QObject::deleteLater);
    connect(this, &ThreadController::startRead, reader, &MessageReader::doWork);
    connect(reader, &MessageReader::resultReady, this, &ThreadController::handleResults);
    connect(reader, &MessageReader::makeFatalError, this, &ThreadController::throwFatalErrorOccuried);
    readerThread.start();
}

ThreadController::~ThreadController()
{
    readerThread.quit();
    readerThread.wait();
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
        emit resultReady(result);
    }
}

void ThreadController::handleResults(std::string message)
{
    //model->addMessage(message.c_str(), Qt::red);
}

void ThreadController::throwFatalErrorOccuried(QString errortext)
{
    emit throwFatalError(errortext);
}
