#include "dialoglinearnews.h"
#include "ui_dialoglinearnews.h"
#include "CTransaction.h"
#include "dialogpostlinearnews.h"
#include "TextHelper.h"

DialogLinearNews::DialogLinearNews(ConnectionController * c, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogLinearNews)
{
    ui->setupUi(this);
    connection = c;

    connect(ui->buttonPost, SIGNAL(clicked()), this, SLOT(onPostButton()));
    connect(ui->buttonRefresh, SIGNAL(clicked()), this, SLOT(onRefreshButton()));
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

void DialogLinearNews::onPostButton()
{
    DialogPostLinearNews * dialog = new DialogPostLinearNews(this);
    connect(dialog, SIGNAL(postedText(QString)), this, SLOT(onPostNews(QString)));
    connect(dialog, SIGNAL(finished(int)), dialog, SLOT(deleteLater()));
    dialog->show();
}

void DialogLinearNews::onRefreshButton()
{
    requestNews();
}

void DialogLinearNews::onPostNews(QString str)
{
    CTransaction * t = connection->createTransaction(103);
    QByteArray postData = TextHelper::EncodeText(str);
    t->addParameter(101, postData.length(), postData.data());
    connection->sendTransaction(t);
    requestNews();
}
