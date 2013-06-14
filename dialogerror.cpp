#include "dialogerror.h"
#include "ui_dialogerror.h"

DialogError::DialogError(QString message, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogError)
{
    ui->setupUi(this);
    ui->label->setText(message);
}

DialogError::~DialogError()
{
    delete ui;
}

void DialogError::changeEvent(QEvent *e)
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
