#include "dialogcreateaccount.h"
#include "ui_dialogcreateaccount.h"

#include "dialogprivileges.h"

DialogCreateAccount::DialogCreateAccount(ConnectionController * c, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCreateAccount)
{
    ui->setupUi(this);

    connection = c;

    ui->lineEdit->setFocus();

    connect(ui->buttonBox_2, SIGNAL(accepted()), this, SLOT(createAccountWindow()));
    connect(ui->buttonBox_2, SIGNAL(rejected()), this, SLOT(reject()));
}

DialogCreateAccount::~DialogCreateAccount()
{
    delete ui;
}

void DialogCreateAccount::createAccountWindow()
{
    DialogPrivileges * dialog = new DialogPrivileges(connection, ui->lineEdit->text());
    accept();
    dialog->show();
    connect(dialog, SIGNAL(finished(int)), dialog, SLOT(deleteLater()));
}
