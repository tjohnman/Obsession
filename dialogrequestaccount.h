#ifndef DIALOGREQUESTACCOUNT_H
#define DIALOGREQUESTACCOUNT_H

#include <QDialog>
#include "connectioncontroller.h"

namespace Ui {
class DialogRequestAccount;
}

class DialogRequestAccount : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogRequestAccount(ConnectionController * c, QWidget *parent = 0);
    ~DialogRequestAccount();
    
private slots:
    void requestAccount();

private:
    ConnectionController * connection;
    Ui::DialogRequestAccount *ui;
};

#endif // DIALOGREQUESTACCOUNT_H
