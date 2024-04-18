#ifndef USERCHAT_H
#define USERCHAT_H

#include <QDialog>
#include "client.h"
#include "customItemModel.h"
#include "itemdelegate.h"
#include "windowssystemhandler.h"
#include <QMessageBox>
#include <QListView>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QScrollBar>
#include <QThread>
#include <QDebug>

namespace Ui {
class UserChat;
}

struct chatPacket_t {
    std::string sender_name;
    std::string recipient_name;
    client *cl;
    QString fontFamily;
};

class UserChat : public QDialog
{
    Q_OBJECT
public:
    explicit UserChat(QWidget *parent = nullptr, chatPacket_t chatinfo = {}, windowsSystemHandler *m_winHandler = nullptr);
    ~UserChat();
    void writeSenderMessage(std::string message);
    void pre_initBuffAppend(std::string message);
    void pre_initBuffLoadout();
    bool wasItOpened();
    void setItOpened();
    void writeSystemMessage(QString message);
private:
    void setup_listview();
    void setup_font();
    void throw_message();
private:
    Ui::UserChat             *ui;
    MessageModel             *model;
    ColoredMessageDelegate   *delegate;
    windowsSystemHandler     *winHandler;
    client                   *cl;
    std::string              username;
    std::string              recipient_name;
    QString                  fontFamily;
    std::vector<std::string> pre_initBuffer;
    bool                     wasOpened = false;
private slots:
    void on_pushButton_clicked();
    void on_lineEdit_returnPressed();
};

class UserchatsController : public QObject{
    Q_OBJECT
private:
    QWidget *parent;
    windowsSystemHandler *winHandler;
    std::map<std::string, UserChat*> userchats;
public:
    UserchatsController(QWidget *m_parent = nullptr, windowsSystemHandler *m_winHandler = nullptr);
    bool chatIsOpen(std::string login);
    void addChat(chatPacket_t chatInfo);
    void deleteChat(std::string login);
    void openChat(std::string login);
    void sendMessageToChat(std::string login, std::string message);
    void sendLeftMessage(std::string login);
    void sendJoinMessage(std::string login);
signals:
    void throwNotification(std::string notifyType, std::string text);
};

#endif // USERCHAT_H
