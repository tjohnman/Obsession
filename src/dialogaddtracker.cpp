#include "dialogaddtracker.h"
#include "ui_dialogaddtracker.h"

DialogAddTracker::DialogAddTracker(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAddTracker)
{
    ui->setupUi(this);
    connect(ui->lineEdit, SIGNAL(textChanged(QString)), this, SLOT(updateName(QString)));
    connect(ui->lineEdit_2, SIGNAL(textChanged(QString)), this, SLOT(updateAddress(QString)));
}

DialogAddTracker::~DialogAddTracker()
{
    delete ui;
}

void DialogAddTracker::changeEvent(QEvent *e)
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

void DialogAddTracker::updateName(QString s) {
    pName = s;
}

void DialogAddTracker::updateAddress(QString s) {
    pAddress = s;
}

QString DialogAddTracker::name() {
    return pName;
}

QString DialogAddTracker::address() {
    return pAddress;
}
