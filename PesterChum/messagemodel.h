#ifndef MESSAGEMODEL_H
#define MESSAGEMODEL_H

#include "ui_mainwindow.h"
#include "coloredmessagedelegate.h"
#include <QListView>
#include <QColor>
#include <QStandardItem>
#include <QStandardItemModel>

class MessageModel : public QStandardItemModel
{
private:
    Ui::MainWindow *ui_main;
public:
    MessageModel(QObject *parent = nullptr, Ui::MainWindow *m_ui_main = nullptr);
    void addMessage(const QString &text, QColor color);
};

#endif // MESSAGEMODEL_H
