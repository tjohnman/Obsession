#ifndef WIDGETCHAT_H
#define WIDGETCHAT_H

#include <QWidget>
#include <QListWidgetItem>
#include <QSound>
#include <QFont>

class ConnectionController;

namespace Ui {
    class WidgetChat;
}

class WidgetChat : public QWidget {
    Q_OBJECT
public:
    WidgetChat(QWidget *parent = 0);
    ~WidgetChat();

    void printChat(QString s);
    void clearChat();
    void clearUserList();
    void setChatFont(QFont);
    void setEncodingLabel(QString);

    void addUser(QListWidgetItem *, quint16 id);

    QSound * chatSound;

    ConnectionController * connection;

public slots:
    void slotSendChat();
    void onOpenMessagingWindow(quint16);
    void requestUserInfo(quint16);
    void onKickUser(quint16);

signals:
    void messagingWindowSignal(quint16);
    void userInfoSignal(quint16);
    void kickUserSignal(quint16);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::WidgetChat *ui;

};

#endif // WIDGETCHAT_H
