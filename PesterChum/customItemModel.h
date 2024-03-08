#ifndef CUSTOMITEMMODEL_H
#define CUSTOMITEMMODEL_H

#include "ui_userchat.h"
#include "itemdelegate.h"
#include <QListView>
#include <QColor>
#include <QStandardItem>
#include <QStandardItemModel>

class MessageModel : public QStandardItemModel
{
private:
    Ui::UserChat *ui_chat;
public:
    MessageModel(QObject *parent = nullptr, Ui::UserChat *newChatUi = nullptr);
    void addMessage(const QString &text, QColor color);
private slots:
    void on_pushButton_clicked();
};

class UserchatModel : public QStandardItemModel
{
public:
    UserchatModel(QObject *parent = nullptr);
    void addUserchat(const QString &username);
private slots:
    void on_friends_button_clicked();
};

class RequestModel : public QStandardItemModel
{
public:
    RequestModel(QObject *parent = nullptr);
    void addRequest(const QString &text);
};

#endif // CUSTOMITEMMODEL_H
