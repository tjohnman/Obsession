#include "dialoguserinfo.h"
#include "ui_dialoguserinfo.h"
#include "ctransaction.h"
#include "TextHelper.h"

DialogUserInfo::DialogUserInfo(qint16 id, ConnectionController * c, QWidget *parent) :
        QDialog(parent),
        ui(new Ui::DialogUserInfo)
{
    ui->setupUi(this);
    connection = c;
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

void DialogUserInfo::gotUserInfo(QString username, QString text)
{
    text.replace("\r", "\n");
    text.replace("\n", "<br>");
    ui->label->setText(QString("Username: <b>%1</b>").arg(username));
    ui->textBrowser->setHtml(text);
}
