#ifndef ITEMDELEGATE_H
#define ITEMDELEGATE_H

#include <QApplication>
#include <QListView>
#include <QStyledItemDelegate>
#include <QStyleOptionViewItem>
#include <QEvent>
#include <QEnterEvent>
#include <QFontMetrics>
#include <QTextDocument>
#include <QPainter>
#include <QtGlobal>

class ColoredMessageDelegate : public QStyledItemDelegate
{
private:
    QListView *listView;
    QString fontFamily;
    int fontSize;
public:
    ColoredMessageDelegate(QObject *parent = nullptr, QListView *m_listView = nullptr, QString m_fontFamily = nullptr, int m_fontSize = 0);
private:
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QString splitUnitText(QString unitText, const QStyleOptionViewItem &option) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};



class UserchatsDelegate : public QStyledItemDelegate
{
private:
    QString fontFamily;
public:
    UserchatsDelegate(QObject *parent = nullptr, QString m_fontFamily = nullptr);
private:
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // ITEMDELEGATE_H
