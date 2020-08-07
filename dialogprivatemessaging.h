#ifndef DIALOGPRIVATEMESSAGING_H
#define DIALOGPRIVATEMESSAGING_H

#include <QDialog>
#include "connectioncontroller.h"
#include "defines.h"

namespace Ui {
    class DialogPrivateMessaging;
}

class DialogPrivateMessaging : public QDialog {
    Q_OBJECT
public:
    DialogPrivateMessaging(qint16, ConnectionController *, QWidget *parent = 0);
    ~DialogPrivateMessaging();

    qint16 uid;
    ConnectionController * connection;
    s_user * user;

    void gotMessage(QString m);

public slots:
    void onPreferencesSaved();

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
