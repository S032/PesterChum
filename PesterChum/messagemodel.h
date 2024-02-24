#ifndef MESSAGEMODEL_H
#define MESSAGEMODEL_H

#include "ui_userchat.h"
#include "coloredmessagedelegate.h"
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

#endif // MESSAGEMODEL_H
