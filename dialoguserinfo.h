#ifndef DIALOGUSERINFO_H
#define DIALOGUSERINFO_H

#include <QDialog>
#include "ConnectionController.h"

namespace Ui {
    class DialogUserInfo;
}

class DialogUserInfo : public QDialog {
    Q_OBJECT
public:
    DialogUserInfo(qint16, ConnectionController *, QWidget *parent = 0);
    ~DialogUserInfo();

public slots:
    void gotUserInfo(QString text, QString username);

protected:
    void changeEvent(QEvent *e);

private:
    ConnectionController * connection;
    Ui::DialogUserInfo *ui;
};

#endif // DIALOGUSERINFO_H
