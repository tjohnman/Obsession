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

    void addUser(QListWidgetItem *);

    QSound * chatSound;

    ConnectionController * connection;

public slots:
    void slotSendChat();
    void onOpenMessagingWindow(uint);
    void onOpenUserInfo(uint);

signals:
    void messagingWindowSignal(uint);
    void userInfoSignal(uint);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::WidgetChat *ui;

};

#endif // WIDGETCHAT_H
