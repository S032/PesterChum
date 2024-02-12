#ifndef MESSAGEMODEL_H
#define MESSAGEMODEL_H

#include "ui_mainwindow.h"
#include <QListView>
#include <QStandardItem>
#include <QStandardItemModel>

class MessageModel : public QStandardItemModel
{
private:
    Ui::MainWindow *ui_main;
public:
    MessageModel(QObject *parent = nullptr, Ui::MainWindow *m_ui_main = nullptr);
    void addMessage(const QString &text, const QString &colour);
};

#endif // MESSAGEMODEL_H
