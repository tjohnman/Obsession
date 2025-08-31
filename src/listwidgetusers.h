#ifndef LISTWIDGETUSERS_H
#define LISTWIDGETUSERS_H

#include <QListWidget>
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include <map>
#include <qpainter.h>
#include <QStyledItemDelegate>

class ListWidgetUsers : public QListWidget
{
Q_OBJECT

QAction * PMAction, * infoAction, * kickAction;

void contextMenuEvent(QContextMenuEvent *);

public:
    explicit ListWidgetUsers(QWidget *parent = 0);
    void addRow(QListWidgetItem * item, quint16 id);

private:
    std::map<QListWidgetItem *, quint16> m_UIDMap;

signals:
    void openMessagingWindow(quint16);
    void requestUserInfo(quint16);
    void kickUser(quint16);

public slots:
    void sendOpenRequest();
    void sendInfoRequest();
    void sendKickRequest();
};

class CenterIconDelegate : public QStyledItemDelegate {
public:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override {
        QStyleOptionViewItem opt = option;
        initStyleOption(&opt, index);

        opt.widget->style()->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, painter, opt.widget);

        QIcon icon = opt.icon;
        QString text = opt.text;
        QRect rect = opt.rect;

        opt.icon = QIcon();
        opt.text = QString();

        QSize actualIconSize = icon.actualSize(QSize(32, 32));
        int iconY = rect.top() + (rect.height() - actualIconSize.height()) / 2;
        QRect iconRect(rect.left(), iconY, 32, actualIconSize.height());

        int iconX = rect.left() + (32 - actualIconSize.width()) / 2;
        icon.paint(painter, iconX + 1, iconY, actualIconSize.width() + 1, actualIconSize.height());

        QFontMetrics fm(opt.font);
        QRect textRect(rect.left() + 32 + 5, rect.top(), rect.width() - 32, rect.height());
        painter->setFont(opt.font);
        painter->setPen(opt.palette.color(QPalette::Text));
        painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, text);
    }

    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override {
        QSize size = QStyledItemDelegate::sizeHint(option, index);
        return size;
    }
};

#endif // LISTWIDGETUSERS_H
