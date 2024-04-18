#include "windowssystemhandler.h"


windowsSystemHandler::windowsSystemHandler(QWidget *mainwin)
    : mainwindow(mainwin)
{
    initNotify();
}

QString windowsSystemHandler::getTime()
{
    std::time_t current_time = std::time(nullptr);
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%H:%M", std::localtime(&current_time));
    return QString(buffer);
}

void windowsSystemHandler::throwNotification(const std::string &header, const std::string &text)
{
    trayIcon->showMessage(header.c_str(), truncateString(text, 150).c_str(), QIcon(":/images/new_message_alert.png"), 5000);
}

void windowsSystemHandler::pushNotifyInQueue(const std::string &header, const std::string &text)
{
    notify_t notify = std::make_pair(header, text);
    notifyQueue.push(notify);
}

void windowsSystemHandler::setCurrentNotify()
{
    if (notifyQueue.empty()) {
        qDebug() << "list is empty";
        return;
    }
    currentNotify = notifyQueue.front();
    notifyQueue.pop();
}

notify_t windowsSystemHandler::getCurrentNotify()
{
    return currentNotify;
}

void windowsSystemHandler::messageClickedHandler()
{
    qDebug() << "message was clicked!";
}

void windowsSystemHandler::notifyClickHandler()
{
    qDebug() << "zzzzz";
    notify_t notify = getCurrentNotify();
    emit openWindow();
    if (notify.first == NEW_MSG) {
        return;
    }
}

void windowsSystemHandler::initNotify()
{
    trayIcon = new QSystemTrayIcon(mainwindow);
    trayImage = QIcon(":/images/smile_28x23.png");
    trayIcon->setIcon(trayImage);
    trayIcon->setToolTip("PesterChum");
    setTrayIconActions();
    trayIcon->setContextMenu(trayContextMenu);
    QObject::connect(trayIcon, &QSystemTrayIcon::messageClicked, this, &windowsSystemHandler::messageClickedHandler);
    trayIcon->show();
}

void windowsSystemHandler::setTrayIconActions()
{
    minimizeAct = new QAction("Свернуть", this);
    restoreAct = new QAction("Восстановить", this);
    exitAct = new QAction("Выход", this);

    connect(minimizeAct, &QAction::triggered, mainwindow, &QWidget::hide);
    connect(restoreAct, &QAction::triggered, mainwindow, &QWidget::show);
    connect(exitAct, &QAction::triggered, qApp, &QApplication::quit);

    trayContextMenu = new QMenu(mainwindow);
    trayContextMenu->addAction(minimizeAct);
    trayContextMenu->addAction(restoreAct);
    trayContextMenu->addAction(exitAct);
}

std::string windowsSystemHandler::truncateString(const std::string &str, size_t maxLength)
{
    if (str.size() <= maxLength) {
        return str;
    }
    else {
        return str.substr(0, maxLength - 4) + "...";
    }
}
