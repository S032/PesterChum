#include "messagemodel.h"

MessageModel::MessageModel(QObject *parent, Ui::MainWindow *m_ui_main)
    :
    QStandardItemModel(parent),
    ui_main(m_ui_main)
{}

void MessageModel::addMessage(const QString &text, const QString &colour)
{
    QStandardItem *item = new QStandardItem(QString("%1").arg(text));
    item->setData(QColor(Qt::blue));
    appendRow(item);
    ui_main->listView->scrollToBottom();
}


