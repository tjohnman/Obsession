#include "dialogprivatemessaging.h"
#include "ui_dialogprivatemessaging.h"
#include <QSettings>
#include "TextHelper.h"

DialogPrivateMessaging::DialogPrivateMessaging(qint16 id, ConnectionController * c, QWidget * parent) :
    QDialog(parent),
    ui(new Ui::DialogPrivateMessaging)
{
    ui->setupUi(this);

    connection = c;
    uid = id;

    user = connection->getUserByUid(uid);

    ui->label->setText(QString("Private chat with ") + QString(user->name));
    this->setWindowTitle(QString("Private chat with ") + QString(user->name));

    connect(ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(sendMessage()));

    this->onPreferencesSaved();
    ui->lineEdit->setFocus();
}

DialogPrivateMessaging::~DialogPrivateMessaging()
{
    delete ui;
}

void DialogPrivateMessaging::changeEvent(QEvent *e)
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

void DialogPrivateMessaging::sendMessage() {
    if(!ui->lineEdit->text().isEmpty()) {
        QSettings settings("mir", "Contra");
        connection->sendPMToUser(uid, ui->lineEdit->text());
        ui->textEdit->moveCursor(QTextCursor::End);
        QString formatted = TextHelper::FormatMessageToHTML(ui->lineEdit->text());
        ui->textEdit->moveCursor(QTextCursor::End);
        ui->textEdit->ensureCursorVisible();
        ui->lineEdit->setFocus();
        ui->textEdit->insertHtml(QString("<b>") + settings.value("nick", "unnamed").toString()+QString("</b>: ")+formatted+QString("<p />"));
        ui->textEdit->moveCursor(QTextCursor::End);
        ui->textEdit->ensureCursorVisible();
        ui->lineEdit->clear();
        emit sentPM();
    }
}

void DialogPrivateMessaging::gotMessage(QString m) {
    ui->textEdit->moveCursor(QTextCursor::End);
    QString formatted = TextHelper::FormatMessageToHTML(m);
    ui->textEdit->moveCursor(QTextCursor::End);
    ui->textEdit->ensureCursorVisible();
    ui->lineEdit->setFocus();
    ui->textEdit->insertHtml(QString("<b>") + QString(user->name)+QString("</b>: ")+formatted+QString("<p />"));
    ui->textEdit->moveCursor(QTextCursor::End);
    ui->textEdit->ensureCursorVisible();
}

void DialogPrivateMessaging::onPreferencesSaved() {
    QFont font;
    QSettings settings("mir", "Contra");
    font.setFamily(settings.value("fontFamily", "MS Shell Dlg2").toString());
    qint32 style = settings.value("fontStyle", 0).toInt();
    switch(style) {
    case 0:
        font.setStyle(QFont::StyleNormal);
        break;
    case 1:
        font.setStyle(QFont::StyleItalic);
        break;
    case 2:
        font.setStyle(QFont::StyleOblique);
        break;
    }

    font.setPointSize(settings.value("fontSize", 8).toInt());
    font.setBold(settings.value("fontBold", false).toBool());
    ui->textEdit->setFont(font);
}
