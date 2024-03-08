#include "itemdelegate.h"

ColoredMessageDelegate::ColoredMessageDelegate(QObject *parent, QListView *m_listView, QString m_fontFamily, int m_fontSize)
    :
    QStyledItemDelegate(parent),
    listView(m_listView),
    fontFamily(m_fontFamily),
    fontSize(m_fontSize)
{}

QSize ColoredMessageDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // getting data from index
    QVariant data = index.data(Qt::DisplayRole);
    QString text = data.toString();

    text = splitUnitText(text, option);

    // creating object for size calculation
    QFont font(fontFamily, 23);
    QFontMetrics fm(font);

    // size caclulation
    QRect boundingRect = fm.boundingRect(QRect(0, 0, option.rect.width(), INT_MAX), Qt::TextWordWrap, text);
    QSize size = boundingRect.size();

    size.setHeight(size.height() + 2);

    return size;
}

QString ColoredMessageDelegate::splitUnitText(QString unitText, const QStyleOptionViewItem &option) const
{
    int listViewSizePx = listView->width() - 20;
    QString newText;
    QFont font(fontFamily, 23);
    QFontMetrics fm(font);

    int wordLength = 0;
    for (const QChar& ch : unitText) {
        if (ch == ' ') wordLength = 0;
        if (wordLength + fm.horizontalAdvance(ch) > listViewSizePx) {
            newText += " ";
            wordLength = 0;
        }
        newText += ch;
        wordLength += fm.horizontalAdvance(ch);
    }

    return newText;
}

void ColoredMessageDelegate::paint(QPainter *painter,
                                   const QStyleOptionViewItem &option,
                                   const QModelIndex &index) const
{
    if (!index.isValid()) return;

    QString text = splitUnitText(index.data(Qt::DisplayRole).toString(), option);
    QTextOption textOption;
    textOption.setWrapMode(QTextOption::WordWrap);

    QVariant colorData = index.data(Qt::AccessibleTextRole);
    QColor textColor = colorData.value<QColor>();


    QRect rect = option.rect;

    painter->save();
    painter->setPen(textColor);
    painter->drawText(rect, text, textOption);

}

//////////////////////////////////////////////////////////////////////

UserchatsDelegate::UserchatsDelegate(QObject *parent, QString m_fontFamily)
    :
    QStyledItemDelegate(parent),
    fontFamily(m_fontFamily)
{}

/*
QSize UserchatsDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // getting data from index
    QVariant data = index.data(Qt::DisplayRole);
    QString text = data.toString();

    // creating object for size calculation
    QFont font(fontFamily, 40);
    QFontMetrics fm(font);

    // size caclulation
    QSize size = fm.size(Qt::TextSingleLine, text);

    size.setHeight(size.height());

    return size;
}
*/

void UserchatsDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (!index.isValid()) return;

    QString username = index.data(Qt::DisplayRole).toString();
    QRect rect = option.rect;
    QStyleOptionViewItem opt = option;

    bool isSelected = option.state & QStyle::State_Selected;
    QColor background = isSelected ? Qt::gray : Qt::black;
    painter->fillRect(opt.rect, background);
    if (opt.state & QStyle::State_MouseOver) {
        painter->fillRect(opt.rect, QColor(Qt::lightGray));
    }

    painter->save();
    painter->drawText(rect, username);
}




