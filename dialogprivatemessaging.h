#ifndef DIALOGPRIVATEMESSAGING_H
#define DIALOGPRIVATEMESSAGING_H

#include <QDialog>
#include "ConnectionController.h"
#include "defines.h"

namespace Ui {
    class DialogPrivateMessaging;
}

class DialogPrivateMessaging : public QDialog {
    Q_OBJECT
public:
    DialogPrivateMessaging(short, ConnectionController *, QWidget *parent = 0);
    ~DialogPrivateMessaging();

    short uid;
    ConnectionController * connection;
    s_user * user;

    void gotMessage(QString m);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DialogPrivateMessaging *ui;

private slots:
    void sendMessage();

signals:
    void sentPM();
};

#endif // DIALOGPRIVATEMESSAGING_H
