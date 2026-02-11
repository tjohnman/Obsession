#include "listwidgetusers.h"
#include <QDebug>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QPen>
#include <QFont>

ListWidgetUsers::ListWidgetUsers(QWidget *parent) :
    QListWidget(parent)
{
    // Disable stylesheet for this widget to preserve custom icon backgrounds
    this->setStyleSheet(QString::fromUtf8(""));
    
    PMAction = new QAction(QString::fromUtf8("Send PM to user"), this);
    PMAction->setIcon(QIcon(QString::fromUtf8(":/main/interfaceIcons/mainNew.png")));
    infoAction = new QAction(QString::fromUtf8("Get user info"), this);
    infoAction->setIcon(QIcon(QString::fromUtf8(":/files/interfaceIcons/filesUnknown.png")));
    kickAction = new QAction(QString::fromUtf8("Kick user"), this);
    kickAction->setIcon(QIcon(QString::fromUtf8(":/main/interfaceIcons/mainClose.png")));

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
