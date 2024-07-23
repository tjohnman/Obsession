#include "dialogabout.h"
#include "ui_dialogabout.h"
#include "version.h"
#include <QPainter>

DialogAbout::DialogAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAbout)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    ui->label->setText(QString("Obsession Hotline Client %1.%2").arg(QString::number(VERSION_MAJOR).rightJustified(3, '0'), QString::number(VERSION_MINOR).rightJustified(2, '0')));

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(accept()));
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
