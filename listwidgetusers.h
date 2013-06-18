#ifndef LISTWIDGETUSERS_H
#define LISTWIDGETUSERS_H

#include <QListWidget>
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include <map>

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

#endif // LISTWIDGETUSERS_H
