#ifndef LISTWIDGETUSERS_H
#define LISTWIDGETUSERS_H

#include <QListWidget>
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>

class ListWidgetUsers : public QListWidget
{
Q_OBJECT

QAction * PMAction;
QAction * userInfoAction;

void contextMenuEvent(QContextMenuEvent *);

public:
    explicit ListWidgetUsers(QWidget *parent = 0);

signals:
    void openMessagingWindow(quint32);
    void openUserInfo(quint32);

public slots:
    void sendOpenRequest();
    void sendInfoRequest();
};

#endif // LISTWIDGETUSERS_H
