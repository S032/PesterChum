#ifndef USERCHAT_H
#define USERCHAT_H

#include <QDialog>
#include "client.h"
#include "messagethread.h"
#include "customItemModel.h"
#include "itemdelegate.h"
#include <windows.h>
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

class UserChat : public QDialog
{
    Q_OBJECT
public:
    explicit UserChat(QWidget *parent = nullptr, std::string r_user = nullptr,
                      std::string s_user = nullptr, client *auth_cl = nullptr, QString m_fontFamily = nullptr);
    ~UserChat();
    void writeSenderMessage(std::string message);
    void writeLeftMessage();
    void writeJoinMessage();
private:
    void setup_listview();
    void setup_font();
    void throw_message();
private:
    Ui::UserChat   *ui;
    MessageModel     *model;
    ThreadController *ReadThread;
    ColoredMessageDelegate *delegate;
    client           *cl;
    std::string      username;
    std::string      recipient_name;
    QString          fontFamily;
private slots:
    void on_pushButton_clicked();
    void on_lineEdit_returnPressed();
};

class UserchatsController {
private:
    std::map<std::string, UserChat*> userchats;
    std::string username;
public:
    bool chatIsOpen(std::string login);
    void addChat(std::string login, UserChat *chat);
    void deleteChat(std::string login);
    void openChat(std::string login);
    void sendMessageToChat(std::string login, std::string message);
    void sendLeftMessage(std::string login);
    void sendJoinMessage(std::string login);
};

#endif // USERCHAT_H
