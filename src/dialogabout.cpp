#include "dialogabout.h"
#include "ui_dialogabout.h"
#include "version.h"

DialogAbout::DialogAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAbout)
{
    ui->setupUi(this);

    ui->label->setText(QString("Obsession Hotline Client %1.%2").arg(QString::number(VERSION_MAJOR).rightJustified(3, '0')).arg(QString::number(VERSION_MINOR).rightJustified(2, '0')));
}

DialogAbout::~DialogAbout()
{
    delete ui;
}

void DialogAbout::changeEvent(QEvent *e)
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
