#include "widgetchat.h"
#include "ui_widgetchat.h"

#include <QSettings>

#include "TextHelper.h"
#include "connectioncontroller.h"

WidgetChat::WidgetChat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetChat)
{
    ui->setupUi(this);
    ui->lineEdit->altPressed = false;

    connect(ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(slotSendChat()));
    connect(ui->listWidget, SIGNAL(openMessagingWindow(uint)), this , SLOT(onOpenMessagingWindow(uint)));
    connect(ui->listWidget, SIGNAL(openUserInfo(uint)), this, SLOT(onOpenUserInfo(uint)));
}

void WidgetChat::printChat(QString str) {
    ui->textEdit->moveCursor(QTextCursor::End);
    QString formatted = TextHelper::FormatMessageToHTML(str);
    ui->textEdit->insertHtml(formatted+"<br />");
    ui->textEdit->moveCursor(QTextCursor::End);
    ui->textEdit->ensureCursorVisible();
    ui->lineEdit->setFocus();
}

void WidgetChat::clearChat() {
    ui->textEdit->clear();
}

void WidgetChat::clearUserList() {
    ui->listWidget->clear();
}

void WidgetChat::addUser(QListWidgetItem * item) {
    ui->listWidget->addItem(item);
}

WidgetChat::~WidgetChat()
{
    delete ui;
}

void WidgetChat::setEncodingLabel(QString enc)
{
    ui->encodingLabel->setText("Using encoding <b>"+enc+"</b>");
}

void WidgetChat::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void WidgetChat::setChatFont(QFont font) {
    ui->lineEdit->setFont(font);
    ui->textEdit->setFont(font);
}

///// SLOTS /////

void WidgetChat::slotSendChat() {
    QString text = ui->lineEdit->text();
    if(text.length()) {
        if(ui->lineEdit->altPressed)
        {
            connection->sendEmote(text);
        }
        else
        {
            connection->sendChatText(text);
        }
        ui->lineEdit->clear();
    }
}

void WidgetChat::onOpenMessagingWindow(uint i) {
    emit messagingWindowSignal(i);
}

void WidgetChat::onOpenUserInfo(uint i) {
    emit userInfoSignal(i);
}
