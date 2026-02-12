#include "dialogpreferences.h"
#include "ui_dialogpreferences.h"
#include "ThemeManager.h"
#include <QSettings>
#include <QStringConverter>
#include "SettingsManager.h"

DialogPreferences::DialogPreferences(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPreferences)
{
    ui->setupUi(this);

    auto& settings = SettingsManager::instance();
    ui->lineEdit->setText(settings.value(QString::fromUtf8("nick"), QString::fromUtf8("unnamed")).toString());

    QString encoding = settings.value(QString::fromUtf8("Encoding"), QString::fromUtf8("Apple Roman")).toString();
    if(encoding == QString::fromUtf8("Apple Roman")) ui->encodingCombo->setCurrentIndex(0);
    if(encoding == QString::fromUtf8("Shift_JIS")) ui->encodingCombo->setCurrentIndex(1);
    if(encoding == QString::fromUtf8("ISO-8859-1")) ui->encodingCombo->setCurrentIndex(2);
    if(encoding == QString::fromUtf8("UTF-8")) ui->encodingCombo->setCurrentIndex(3);

    // Load theme preference
    AppTheme currentTheme = ThemeManager::loadThemePreference();
    switch(currentTheme) {
        case AppTheme::Auto:
            ui->themeCombo->setCurrentIndex(0);
            break;
        case AppTheme::Light:
            ui->themeCombo->setCurrentIndex(1);
            break;
        case AppTheme::Dark:
            ui->themeCombo->setCurrentIndex(2);
            break;
    }

    QFont font;
    font.setFamily(settings.value(QString::fromUtf8("fontFamily"), QString::fromUtf8("Consolas")).toString());
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
    ui->useLightColorsCheck->setChecked(settings.value("useLightColorNames", true).toBool());

    bookmarksDialog = new DialogBookmarks(this);

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(acceptSettings()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(close()));
    connect(ui->buttonBookmarks, SIGNAL(clicked()), this, SLOT(openBookmarks()));
    connect(ui->butonChangeFont, SIGNAL(clicked()), this, SLOT(changeFont()));
    connect(ui->themeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onThemeChanged(int)));
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
    auto& settings = SettingsManager::instance();
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

void DialogPreferences::onThemeChanged(int index) {
    // Apply theme immediately when combo box changes
    AppTheme selectedTheme;
    switch(index) {
        case 0:
            selectedTheme = AppTheme::Auto;
            break;
        case 1:
            selectedTheme = AppTheme::Light;
            break;
        case 2:
            selectedTheme = AppTheme::Dark;
            break;
        default:
            selectedTheme = AppTheme::Auto;
            break;
    }
    ThemeManager::applyTheme(selectedTheme);
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
    auto& settings = SettingsManager::instance();
    settings.setValue(QString::fromUtf8("nick"), ui->lineEdit->text());

    switch(ui->encodingCombo->currentIndex())
    {
        default:
        case 0:
            settings.setValue(QString::fromUtf8("Encoding"), QString::fromUtf8("Apple Roman"));
        break;
        case 1:
            settings.setValue(QString::fromUtf8("Encoding"), QString::fromUtf8("Shift_JIS"));
        break;
        case 2:
            settings.setValue(QString::fromUtf8("Encoding"), QString::fromUtf8("ISO-8859-1"));
        break;
        case 3:
            settings.setValue(QString::fromUtf8("Encoding"), QString::fromUtf8("UTF-8"));
        break;
    }
    settings.setValue("EncodingName", ui->encodingCombo->currentText());

    // Save theme preference (already applied in real-time via onThemeChanged)
    AppTheme selectedTheme;
    switch(ui->themeCombo->currentIndex()) {
        case 0:
            selectedTheme = AppTheme::Auto;
            break;
        case 1:
            selectedTheme = AppTheme::Light;
            break;
        case 2:
            selectedTheme = AppTheme::Dark;
            break;
        default:
            selectedTheme = AppTheme::Auto;
            break;
    }
    ThemeManager::saveThemePreference(selectedTheme);

    settings.setValue("dlqueue", ui->dlQueueEdit->text().toInt());
    settings.setValue("soundsEnabled", ui->soundCheckBox->isChecked());
    settings.setValue("debugging", ui->debugCheckBox->isChecked());
    settings.setValue("connectionKeepAlive", ui->checkKeepAlive->isChecked());
    settings.setValue("autoReconnect", ui->checkReconnect->isChecked());
    settings.setValue("useLightColorNames", ui->useLightColorsCheck->isChecked());

    settings.sync();
    emit saved();

    ui->sampleEdit->setText(QString::fromUtf8("Sample text"));
    close();
}
