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
    connect(ui->listWidget, SIGNAL(openMessagingWindow(quint16)), this , SLOT(onOpenMessagingWindow(quint16)));
    connect(ui->listWidget, SIGNAL(requestUserInfo(quint16)), this, SLOT(requestUserInfo(quint16)));
    connect(ui->listWidget, SIGNAL(kickUser(quint16)), this, SLOT(onKickUser(quint16)));
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

void WidgetChat::addUser(QListWidgetItem * item, quint16 id) {
    ui->listWidget->addRow(item, id);
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

void WidgetChat::onOpenMessagingWindow(quint16 i) {
    emit messagingWindowSignal(i);
}

void WidgetChat::requestUserInfo(quint16 i) {
    emit userInfoSignal(i);
}

void WidgetChat::onKickUser(quint16 i) {
    emit kickUserSignal(i);
}
