#include "widgetchat.h"
#include "ui_widgetchat.h"

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

void WidgetChat::printChat(QString s) {
    ui->textEdit->moveCursor(QTextCursor::End);
    while(s.at(0) == '\n' || s.at(0) == '\r') {
        s = s.right(s.length()-1);
        qDebug() << "r";
    }
    QStringList words = s.split(" ");
    QStringList newWords;
    for(qint32 i=0; i<words.size(); i++) {
        if(words.at(i).startsWith("www.")) {
            QString URL = "<a href=\"http://"+words.at(i)+"\">"+words.at(i)+"</a>";
            newWords.append(URL);
        } else if(words.at(i).startsWith("http://") || words.at(i).startsWith("https://")) {
                QString URL = "<a href=\""+words.at(i)+"\">"+words.at(i)+"</a>";
                newWords.append(URL);
        } else if(words.at(i) == "<")
        {
            newWords.append("&lt;");
        } else
        {
            newWords.append(words.at(i));
        }
    }
    QString formatted = newWords.join("&nbsp; ");

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
        if(text.startsWith("/") || ui->lineEdit->altPressed) {
            if(text.startsWith("/em ") || text.startsWith("/me ")) {
                connection->sendEmote(text.right(text.length()-4));
            }
            if(ui->lineEdit->altPressed) {
                connection->sendEmote(text);
            }
            if(text.startsWith("/afk")) {
                connection->toggleAFK();
            }
        } else {
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
