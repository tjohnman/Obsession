#ifndef DIALOGCREATEACCOUNT_H
#define DIALOGCREATEACCOUNT_H

#include <QDialog>
#include "connectioncontroller.h"

namespace Ui {
class DialogCreateAccount;
}

class DialogCreateAccount : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogCreateAccount(ConnectionController * c, QWidget *parent = 0);
    ~DialogCreateAccount();
    
private slots:
    void createAccountWindow();

private:
    Ui::DialogCreateAccount *ui;
    ConnectionController * connection;
};

#endif // DIALOGCREATEACCOUNT_H
