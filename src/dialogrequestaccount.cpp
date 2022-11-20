#include "dialogrequestaccount.h"
#include "ui_dialogrequestaccount.h"

DialogRequestAccount::DialogRequestAccount(ConnectionController * c, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogRequestAccount)
{
    ui->setupUi(this);
    connection = c;

    ui->lineEdit->setFocus();

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(requestAccount()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

DialogRequestAccount::~DialogRequestAccount()
{
    delete ui;
}

void DialogRequestAccount::requestAccount()
{
    connection->requestAccount(ui->lineEdit->text());
    accept();
}
