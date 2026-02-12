#include "dialogopenconnection.h"
#include "ui_dialogopenconnection.h"
#include "mainwindow.h"
#include <QSettings>
#include <QMessageBox>
#include "SettingsManager.h"

DialogOpenConnection::DialogOpenConnection(QWidget *parent, ConnectionController * c) :
    QDialog(parent),
    ui(new Ui::DialogOpenConnection)
{
    ui->setupUi(this);

    bookmarksDialog = new DialogBookmarks(this);

    if(c->isConnected()) {
        ui->checkBox->setChecked(true);
    }

    connection = c;

    connect(ui->buttonBox_2, SIGNAL(accepted()), this, SLOT(openConnection()));
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(choseBookmark(int)));
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(bookmarkCurrent()));
    connect(bookmarksDialog, SIGNAL(accepted()), this, SLOT(updateBookmarkList()));
    connect(bookmarksDialog, SIGNAL(rejected()), this, SLOT(updateBookmarkList()));
    connect(ui->autoConnectCheckbox, SIGNAL(clicked()), this, SLOT(updateAutoConnectStatus()));
    updateBookmarkList();
}

DialogOpenConnection::~DialogOpenConnection()
{
    delete ui;
    delete bookmarksDialog;
}

void DialogOpenConnection::setAddress(QString s) {
    ui->lineEdit->setText(s);
}

void DialogOpenConnection::changeEvent(QEvent *e)
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

void DialogOpenConnection::clear() {
    ui->comboBox->setCurrentIndex(0);
}

void DialogOpenConnection::_clearFields() {
    ui->autoConnectCheckbox->setDisabled(true);
    ui->autoConnectCheckbox->setChecked(false);
    ui->lineEdit->setText(QString::fromUtf8(""));
    ui->lineEdit_2->setText(QString::fromUtf8(""));
    ui->lineEdit_3->setText(QString::fromUtf8(""));
    ui->lineEdit->setFocus();
}

void DialogOpenConnection::setFocusOnLogin() {
    ui->lineEdit_2->setFocus();
}

// SLOTS

void DialogOpenConnection::bookmarkCurrent() {

    if (ui->lineEdit->text().trimmed().length() == 0) {
        QMessageBox::information(this, QString::fromUtf8("Missing server address"), QString::fromUtf8("Please provide an address."));
        return;
    }

    auto& settings = SettingsManager::instance();
    qint32 bookmarkCount = settings.value("bookmarkCount", 0).toInt();
    settings.setValue(QString::fromUtf8("bookmarkname")+QString::number(bookmarkCount), ui->lineEdit->text().trimmed());
    settings.setValue(QString::fromUtf8("bookmarkaddress")+QString::number(bookmarkCount), ui->lineEdit->text().trimmed());
    settings.setValue(QString::fromUtf8("bookmarklogin")+QString::number(bookmarkCount), ui->lineEdit_2->text().trimmed());
    settings.setValue(QString::fromUtf8("bookmarkpassword")+QString::number(bookmarkCount), ui->lineEdit_3->text().trimmed());
    bookmarkCount++;
    settings.setValue("bookmarkCount", bookmarkCount);
    updateBookmarkList();
    ui->comboBox->setCurrentIndex(ui->comboBox->count() - 1);
}

void DialogOpenConnection::choseBookmark(int selectedIndex) {
    auto& settings = SettingsManager::instance();

    if (selectedIndex == 0) {
        this->_clearFields();
        ui->lineEdit->setFocus();
        return;
    }

    ui->autoConnectCheckbox->setDisabled(false);
    ui->lineEdit->setText(settings.value(QString::fromUtf8("bookmarkaddress")+QString::number(selectedIndex-1)).toString());
    ui->lineEdit_2->setText(settings.value(QString::fromUtf8("bookmarklogin")+QString::number(selectedIndex-1)).toString());
    ui->lineEdit_3->setText(settings.value(QString::fromUtf8("bookmarkpassword")+QString::number(selectedIndex-1)).toString());

    QString auto_bookmark = settings.value(QString::fromUtf8("autoBookmark"), QString::fromUtf8("")).toString();
    if(auto_bookmark != QString::fromUtf8("") && auto_bookmark == settings.value(QString::fromUtf8("bookmarkaddress")+QString::number(selectedIndex-1)).toString()) {
        ui->autoConnectCheckbox->setChecked(true);
    } else {
        ui->autoConnectCheckbox->setChecked(false);
    }

    setFocusOnLogin();
}

void DialogOpenConnection::updateAutoConnectStatus()
{
    auto& settings = SettingsManager::instance();
    if(ui->autoConnectCheckbox->isChecked()) {
        settings.setValue("autoBookmark", ui->lineEdit->text());
    } else {
        settings.setValue(QString::fromUtf8("autoBookmark"), QString::fromUtf8(""));
    }
}

void DialogOpenConnection::updateBookmarkList() {
    auto& settings = SettingsManager::instance();

    ui->comboBox->clear();
    ui->comboBox->addItem(QString::fromUtf8(""));

    qint32 bookmarkCount = settings.value("bookmarkCount", 0).toInt();
    for(qint32 i=0; i<bookmarkCount; i++) {
        ui->comboBox->addItem(settings.value(QString::fromUtf8("bookmarkname")+QString::number(i)).toString());
    }
}

void DialogOpenConnection::openConnection() {
    if(ui->checkBox->isChecked()) {
        MainWindow * newWindow = new MainWindow();
        connection = newWindow->connection;
        newWindow->show();
        connection->closeConnection();
        connection->connectToServer(ui->lineEdit->text(), ui->lineEdit_2->text(), ui->lineEdit_3->text());
    } else {
        connection->closeConnection();
        connection->connectToServer(ui->lineEdit->text(), ui->lineEdit_2->text(), ui->lineEdit_3->text());
    }
    emit accepted();
    close();
}
