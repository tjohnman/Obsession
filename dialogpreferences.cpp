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
    if(settings.value("JapaneseMode", true).toBool()) {
        ui->checkBox->setChecked(true);
    } else {
        ui->checkBox->setChecked(false);
    }

    QFont font;
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
    ui->sampleEdit->setFont(font);

    ui->dlQueueEdit->setText(QString::number(settings.value("dlqueue", 0).toInt()));
    ui->soundCheckBox->setChecked(settings.value("soundsEnabled", true).toBool());
    ui->debugCheckBox->setChecked(settings.value("debugging", false).toBool());

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

void DialogPreferences::acceptSettings() {
    QSettings settings("mir", "Contra");
    settings.setValue(QString("nick"), ui->lineEdit->text());
    if(ui->checkBox->isChecked()) {
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("Shift-JIS"));
        settings.setValue("JapaneseMode", true);
    } else {
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("Apple Roman"));
        settings.setValue("JapaneseMode", false);
    }
    settings.setValue("dlqueue", ui->dlQueueEdit->text().toInt());
    settings.setValue("soundsEnabled", ui->soundCheckBox->isChecked());
    settings.setValue("debugging", ui->debugCheckBox->isChecked());
    emit saved();
    ui->sampleEdit->setText("Sample text");
    close();
}
