#include "dialogbroadcast.h"
#include "ui_dialogbroadcast.h"

DialogBroadcast::DialogBroadcast(ConnectionController * c, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogBroadcast)
{
    ui->setupUi(this);
    connection = c;

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(sendMessage()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

DialogBroadcast::~DialogBroadcast()
{
    delete ui;
}

void DialogBroadcast::sendMessage()
{
    connection->broadcast(ui->lineEdit->text());
    accept();
}
