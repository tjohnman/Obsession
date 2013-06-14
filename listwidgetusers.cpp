#include "listwidgetusers.h"
#include <QDebug>

ListWidgetUsers::ListWidgetUsers(QWidget *parent) :
    QListWidget(parent)
{
    PMAction = new QAction("Send PM to user", this);
    userInfoAction = new QAction("See user info", this);

    connect(PMAction, SIGNAL(triggered()), this, SLOT(sendOpenRequest()));
    connect(userInfoAction, SIGNAL(triggered()), this, SLOT(sendInfoRequest()));

    connect(this, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(sendOpenRequest()));

}

void ListWidgetUsers::contextMenuEvent(QContextMenuEvent * event) {
    if(this->selectedItems().length() == 1) {
        QMenu menu(this);
        menu.addAction(PMAction);
        //menu.addAction(userInfoAction);
        menu.exec(event->globalPos());
    }
}

void ListWidgetUsers::sendOpenRequest() {
    emit openMessagingWindow((unsigned int)this->selectedIndexes().at(0).row());
}

void ListWidgetUsers::sendInfoRequest() {
    emit openUserInfo((unsigned int)this->selectedIndexes().at(0).row());
}
