#include "messagemodel.h"

MessageModel::MessageModel(QObject *parent, Ui::MainWindow *m_ui_main)
    :
    QStandardItemModel(parent),
    ui_main(m_ui_main)
{}

void MessageModel::addMessage(const QString &text, QColor color)
{
    QStandardItem *item = new QStandardItem(QString("%1").arg(text));
    item->setData(QColor(color), Qt::AccessibleTextRole);
    appendRow(item);
    ui_main->listView->scrollToBottom();
}


