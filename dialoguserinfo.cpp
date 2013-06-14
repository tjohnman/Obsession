#include "dialoguserinfo.h"
#include "ui_dialoguserinfo.h"

DialogUserInfo::DialogUserInfo(short id, ConnectionController * c, QWidget *parent) :
        QDialog(parent),
        ui(new Ui::DialogUserInfo)
{
    ui->setupUi(this);
}

DialogUserInfo::~DialogUserInfo()
{
    delete ui;
}

void DialogUserInfo::changeEvent(QEvent *e)
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
