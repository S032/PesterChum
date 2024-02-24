#ifndef USERCHAT_H
#define USERCHAT_H

#include <QDialog>
#include "client.h"
#include "messagethread.h"
#include "messagemodel.h"
#include "coloredmessagedelegate.h"
#include <windows.h>
#include <QMessageBox>
#include <QListView>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QThread>
#include <QDebug>

namespace Ui {
class UserChat;
}

class UserChat : public QDialog
{
    Q_OBJECT

public:
    explicit UserChat(QWidget *parent = nullptr);
    ~UserChat();
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
    QString          fontFamily;
public slots:
    void startChat(std::string S_user, client *c, QString m_fontFamily);
private slots:
    void on_pushButton_clicked();
    void on_lineEdit_returnPressed();
};

#endif // USERCHAT_H
