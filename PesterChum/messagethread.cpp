#include "messagethread.h"

ThreadController::ThreadController(client *r_cl, MessageModel *r_model)
    :
    cl(r_cl),
    model(r_model)
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
    cl->Close();
    readerThread.quit();
    readerThread.wait();
}

void MessageReader::doWork(client *cl)
{
    while(m_isRunning) {
        std::string result;
        if(!cl->readMessage(result)) {
            m_isRunning = false;
            emit makeFatalError("Server terminated");
        }
        emit resultReady(result);
    }
}


void ThreadController::handleResults(std::string message)
{
    model->addMessage(message.c_str(), "blue");
}

void ThreadController::throwFatalErrorOccuried(QString errortext)
{
    emit throwFatalError(errortext);
}
