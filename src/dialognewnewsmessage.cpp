#include "dialognewnewsmessage.h"
#include "ui_dialognewnewsmessage.h"

DialogNewNewsMessage::DialogNewNewsMessage(QWidget *parent, const QString path, const quint32 parentId)
    : QDialog(parent)
    , ui(new Ui::DialogNewNewsMessage)
{
    ui->setupUi(this);

    pPath = path;
    pParentId = parentId;

    connect(ui->lineEditTitle, SIGNAL(textChanged(QString)), this, SLOT(onTitleChanged()));
    connect(ui->plainTextEditMessage, SIGNAL(textChanged()), this, SLOT(onMessageChanged()));
    connect(ui->pushButtonSend, SIGNAL(clicked()), this, SLOT(onSendPressed()));

    ui->pushButtonSend->setEnabled(false);

    setModal(true);
}

DialogNewNewsMessage::~DialogNewNewsMessage()
{
    delete ui;
}

void DialogNewNewsMessage::onTitleChanged() {
    ui->pushButtonSend->setEnabled(ui->lineEditTitle->text().trimmed() != QString::fromUtf8("") && ui->plainTextEditMessage->toPlainText().trimmed() != QString::fromUtf8(""));
}

void DialogNewNewsMessage::onMessageChanged() {
    ui->pushButtonSend->setEnabled(ui->lineEditTitle->text().trimmed() != QString::fromUtf8("") && ui->plainTextEditMessage->toPlainText().trimmed() != QString::fromUtf8(""));
}

void DialogNewNewsMessage::onSendPressed() {
    emit messageSubmitted(ui->lineEditTitle->text().trimmed(), ui->plainTextEditMessage->toPlainText().trimmed(), pPath, pParentId);
    accept();
}
