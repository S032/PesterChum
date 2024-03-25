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
////////////////////////////////////////////////
class UserchatModel : public QStandardItemModel
{
private:
    const QString smile_pic_path = "://images/smile_28x23.png";
    const QString inactive_pic_path = ":/images/inactive28x22.png";
    const QString angry_pic_path = "://images/angry_28x23.png";
public:
    UserchatModel(QObject *parent = nullptr);
    void addUserchat(const QString &username, const QString &status);
private slots:
    void on_friends_button_clicked();
};
//////////////////////////////////////////////////
class RequestModel : public QStandardItemModel
{
public:
    RequestModel(QObject *parent = nullptr);
    void addRequest(const QString &text);
};

#endif // CUSTOMITEMMODEL_H
