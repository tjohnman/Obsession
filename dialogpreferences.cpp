#include "dialogpreferences.h"
#include "ui_dialogpreferences.h"
#include <QSettings>
#include <QTextCodec>

DialogPreferences::DialogPreferences(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPreferences)
{
    ui->setupUi(this);

    QSettings settings("mir", "Contra");
    ui->lineEdit->setText(settings.value(QString("nick"), "unnamed").toString());

    QString encoding = settings.value(QString::fromUtf8("Encoding"), "macintosh").toString();
    if(encoding == "macintosh") ui->encodingCombo->setCurrentIndex(0);
    if(encoding == "Shift_JIS") ui->encodingCombo->setCurrentIndex(1);
    if(encoding == "ISO-8859-1") ui->encodingCombo->setCurrentIndex(2);
    if(encoding == "UTF-8") ui->encodingCombo->setCurrentIndex(3);

    QFont font;
    font.setFamily(settings.value("fontFamily", "Consolas").toString());
    qint32 style = settings.value("fontStyle", 9).toInt();
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
    ui->sampleEdit->setFont(font);

    ui->dlQueueEdit->setText(QString::number(settings.value("dlqueue", 1).toInt()));
    ui->soundCheckBox->setChecked(settings.value("soundsEnabled", true).toBool());
    ui->debugCheckBox->setChecked(settings.value("debugging", false).toBool());
    ui->checkKeepAlive->setChecked(settings.value("connectionKeepAlive", true).toBool());
    ui->checkReconnect->setChecked(settings.value("autoReconnect", false).toBool());

    bookmarksDialog = new DialogBookmarks(this);

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(acceptSettings()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(close()));
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(openBookmarks()));
    connect(ui->fontButton, SIGNAL(clicked()), this, SLOT(changeFont()));
}

DialogPreferences::~DialogPreferences()
{
    delete ui;
    delete bookmarksDialog;
}

void DialogPreferences::openBookmarks() {
    bookmarksDialog->loadBookmarks();
    bookmarksDialog->show();
}

void DialogPreferences::changeFont() {
    bool ok;
    QSettings settings("mir", "Contra");
    QFont font = QFontDialog::getFont(
                    &ok, ui->sampleEdit->font(), this );
    if ( ok ) {
        settings.setValue("fontFamily", font.family());
        settings.setValue("fontSize", font.pointSize());
        settings.setValue("fontStyle", font.style());
        settings.setValue("fontBold", font.bold());
    }
    ui->sampleEdit->setFont(font);
}

void DialogPreferences::changeEvent(QEvent *e)
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

QString DialogPreferences::selectedEncoding()
{
    return ui->encodingCombo->currentText();
}

void DialogPreferences::acceptSettings() {
    QSettings settings("mir", "Contra");
    settings.setValue(QString("nick"), ui->lineEdit->text());

    switch(ui->encodingCombo->currentIndex())
    {
        default:
        case 0:
            settings.setValue("Encoding", "macintosh");
        break;
        case 1:
            settings.setValue("Encoding", "Shift_JIS");
        break;
        case 2:
            settings.setValue("Encoding", "ISO-8859-1");
        break;
        case 3:
            settings.setValue("Encoding", "UTF-8");
        break;
    }
    settings.setValue("EncodingName", ui->encodingCombo->currentText());

    settings.setValue("dlqueue", ui->dlQueueEdit->text().toInt());
    settings.setValue("soundsEnabled", ui->soundCheckBox->isChecked());
    settings.setValue("debugging", ui->debugCheckBox->isChecked());
    settings.setValue("connectionKeepAlive", ui->checkKeepAlive->isChecked());
    settings.setValue("autoReconnect", ui->checkReconnect->isChecked());

    emit saved();
    ui->sampleEdit->setText("Sample text");
    close();
}
