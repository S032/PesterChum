#ifndef WINDOWSSYSTEMHANDLER_H
#define WINDOWSSYSTEMHANDLER_H

#include <QWidget>
#include <QDebug>
#include <QObject>
#include <QDir>
#include <QPixmap>
#include <QSystemTrayIcon>
#include <QApplication>
#include <QMenu>
#include <qsystemtrayicon.h>
#include <windows.h>
#include <shellapi.h>
#include <queue>
#include <ctime>
#include <locale>
#include <codecvt>

#define MY_TRAY_ICON_ID 1
#define NEW_MSG "📫Новое сообщение:📫"
#define REQ_ACC "✅Ваш запрос принят!✅"
#define REQ_DEN "🚫Ваш запрос отклонен!🚫"
#define FRD_DEL "😱Вас удалили из друзей!!😱"

using notify_t = std::pair<std::string, std::string>;

class windowsSystemHandler : public QObject
{
    Q_OBJECT
private:
    QWidget *mainwindow;
    NOTIFYICONDATA nid;
    QSystemTrayIcon *trayIcon;
    QIcon trayImage;
    QMenu *trayContextMenu;
    QAction *minimizeAct;
    QAction *restoreAct;
    QAction *exitAct;
    std::queue<notify_t> notifyQueue;
    notify_t currentNotify;
public:
    windowsSystemHandler(QWidget *mainwin);
    QString getTime();
    void throwNotification(const std::string &header, const std::string &text);
    void setCurrentNotify();
    void notifyClickHandler();
private:
    void initNotify();
    void setTrayIconActions();
    std::wstring StringToWString(const std::string &text);
    std::string truncateString(const std::string &str, size_t maxLength);
    void pushNotifyInQueue(const std::string &header, const std::string &text);
    notify_t getCurrentNotify();
public slots:
    void messageClickedHandler();
signals:
    void openWindow();
};

#endif // WINDOWSSYSTEMHANDLER_H
