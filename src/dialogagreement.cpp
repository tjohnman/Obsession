#include "dialogagreement.h"
#include "ui_dialogagreement.h"

DialogAgreement::DialogAgreement(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAgreement)
{
    ui->setupUi(this);
}

DialogAgreement::~DialogAgreement()
{
    delete ui;
}

void DialogAgreement::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void DialogAgreement::setAgreement(QString s) {
    ui->plainTextEdit->clear();
    ui->plainTextEdit->setPlainText(s);
}
