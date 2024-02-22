#ifndef COLOREDMESSAGEDELEGATE_H
#define COLOREDMESSAGEDELEGATE_H

#include <QApplication>
#include <QListView>
#include <QStyledItemDelegate>
#include <QFontMetrics>
#include <QTextDocument>
#include <QPainter>
#include <QtGlobal>

class ColoredMessageDelegate : public QStyledItemDelegate
{
private:
    QListView *listView;
public:
    ColoredMessageDelegate(QObject *parent = nullptr, QListView *m_listView = nullptr);
private:
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    QString splitUnitText(QString unitText, const QStyleOptionViewItem &option) const;

    void setMessageType(const QModelIndex &index, bool isSentByUser);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // COLOREDMESSAGEDELEGATE_H
