#include "dialogprivatemessaging.h"
#include "ui_dialogprivatemessaging.h"
#include <QSettings>
#include "TextHelper.h"
#include "SettingsManager.h"

DialogPrivateMessaging::DialogPrivateMessaging(qint16 id, ConnectionController * c, QWidget * parent) :
    QDialog(parent),
    ui(new Ui::DialogPrivateMessaging)
{
    ui->setupUi(this);

    connection = c;
    uid = id;

    user = connection->getUserByUid(uid);

    ui->label->setText(QString::fromUtf8("Private chat with ") + user->name);
    this->setWindowTitle(QString::fromUtf8("Private chat with ") + user->name);

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
        auto& settings = SettingsManager::instance();
        connection->sendPMToUser(uid, ui->lineEdit->text());
        ui->textEdit->moveCursor(QTextCursor::End);
        QString formatted = TextHelper::FormatMessageToHTML(ui->lineEdit->text());
        ui->textEdit->moveCursor(QTextCursor::End);
        ui->textEdit->ensureCursorVisible();
        ui->lineEdit->setFocus();
        ui->textEdit->insertHtml(QString::fromUtf8("<b>") + settings.value(QString::fromUtf8("nick"), QString::fromUtf8("unnamed")).toString()+QString::fromUtf8("</b>: ")+formatted+QString::fromUtf8("<p />"));
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
    ui->textEdit->insertHtml(QString::fromUtf8("<b>") + user->name + QString::fromUtf8("</b>: ") + formatted + QString::fromUtf8("<p />"));
    ui->textEdit->moveCursor(QTextCursor::End);
    ui->textEdit->ensureCursorVisible();
}

void DialogPrivateMessaging::onPreferencesSaved() {
    QFont font;
    auto& settings = SettingsManager::instance();
    font.setFamily(settings.value(QString::fromUtf8("fontFamily"), QString::fromUtf8("MS Shell Dlg2")).toString());
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
