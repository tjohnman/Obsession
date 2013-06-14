#include "dialoglinearnews.h"
#include "ui_dialoglinearnews.h"
#include "CTransaction.h"

DialogLinearNews::DialogLinearNews(ConnectionController * c, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogLinearNews)
{
    ui->setupUi(this);
    connection = c;
}

void DialogLinearNews::requestNews() {
    ui->plainTextEdit->setPlainText("");
    connect(connection, SIGNAL(gotLinearNews(QString)), this, SLOT(updateNews(QString)));
    CTransaction * newsRequest = connection->createTransaction(101);
    connection->sendTransaction(newsRequest, true);
}

void DialogLinearNews::updateNews(QString news) {
    ui->plainTextEdit->setPlainText(news);
}

DialogLinearNews::~DialogLinearNews()
{
    delete ui;
}

void DialogLinearNews::changeEvent(QEvent *e)
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
