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
    DialogUserInfo(short, ConnectionController *, QWidget *parent = 0);
    ~DialogUserInfo();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DialogUserInfo *ui;
};

#endif // DIALOGUSERINFO_H
