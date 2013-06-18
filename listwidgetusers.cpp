#include "listwidgetusers.h"
#include <QDebug>

ListWidgetUsers::ListWidgetUsers(QWidget *parent) :
    QListWidget(parent)
{
    PMAction = new QAction("Send PM to user", this);
    PMAction->setIcon(QIcon(":/main/interfaceIcons/mainNew.png"));
    infoAction = new QAction("Get user info", this);
    infoAction->setIcon(QIcon(":/files/interfaceIcons/filesUnknown.png"));
    kickAction = new QAction("Kick user", this);
    kickAction->setIcon(QIcon(":/main/interfaceIcons/mainClose.png"));

    connect(PMAction, SIGNAL(triggered()), this, SLOT(sendOpenRequest()));
    connect(kickAction, SIGNAL(triggered()), this, SLOT(sendKickRequest()));
    connect(infoAction, SIGNAL(triggered()), this, SLOT(sendInfoRequest()));

    connect(this, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(sendOpenRequest()));

}

void ListWidgetUsers::contextMenuEvent(QContextMenuEvent * event) {
    if(this->selectedItems().length() == 1) {
        QMenu menu(this);
        menu.addAction(PMAction);
        menu.addAction(infoAction);
        menu.addAction(kickAction);
        menu.exec(event->globalPos());
    }
}

void ListWidgetUsers::addRow(QListWidgetItem * item, quint16 id)
{
    this->addItem(item);
    m_UIDMap[item] = id;
}

void ListWidgetUsers::sendOpenRequest() {
    emit openMessagingWindow(m_UIDMap[this->selectedItems().at(0)]);
}

void ListWidgetUsers::sendInfoRequest() {
    emit requestUserInfo(m_UIDMap[this->selectedItems().at(0)]);
}

void ListWidgetUsers::sendKickRequest()
{
    emit kickUser(m_UIDMap[this->selectedItems().at(0)]);
}
