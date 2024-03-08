#include "customItemModel.h"

MessageModel::MessageModel(QObject *parent, Ui::UserChat *newUserChat)
    :
    QStandardItemModel(parent),
    ui_chat(newUserChat)
{}

void MessageModel::addMessage(const QString &text, QColor color)
{
    QStandardItem *item = new QStandardItem(QString("%1").arg(text));
    item->setData(QColor(color), Qt::AccessibleTextRole);
    appendRow(item);
    ui_chat->listView->scrollToBottom();
}

///////////////////////////////////////////////////////////////////////////

UserchatModel::UserchatModel(QObject *parent)
    :
    QStandardItemModel(parent)
{}

void UserchatModel::addUserchat(const QString &username)
{
    QStandardItem *item = new QStandardItem(QString("%1").arg(username));
    appendRow(item);
}

///////////////////////////////////////////////////////////////////////////

RequestModel::RequestModel(QObject *parent)
    :
    QStandardItemModel(parent)
{}

void RequestModel::addRequest(const QString &text)
{
    QStandardItem *item = new QStandardItem(QString("%1").arg(text));
    appendRow(item);
}
