#include "dialogopenconnection.h"
#include "ui_dialogopenconnection.h"
#include "mainwindow.h"
#include <QSettings>

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
    connect(ui->comboBox, SIGNAL(currentIndexChanged(qint32)), this, SLOT(choseBookmark()));
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
    ui->autoConnectCheckbox->setDisabled(true);
    ui->comboBox->setCurrentIndex(-1);
    ui->lineEdit->setText("");
    ui->lineEdit_2->setText("");
    ui->lineEdit_3->setText("");
    ui->lineEdit->setFocus();
}

void DialogOpenConnection::setFocusOnLogin() {
    ui->lineEdit_2->setFocus();
}

// SLOTS

void DialogOpenConnection::bookmarkCurrent() {
    QSettings settings("mir", "Contra");
    qint32 bookmarkCount = settings.value("bookmarkCount", 0).toInt();
    settings.setValue("bookmarkname"+QString::number(bookmarkCount), ui->lineEdit->text());
    settings.setValue("bookmarkaddress"+QString::number(bookmarkCount), ui->lineEdit->text());
    settings.setValue("bookmarklogin"+QString::number(bookmarkCount), ui->lineEdit_2->text());
    settings.setValue("bookmarkpassword"+QString::number(bookmarkCount), ui->lineEdit_3->text());
    bookmarkCount++;
    settings.setValue("bookmarkCount", bookmarkCount);
    updateBookmarkList();
    ui->comboBox->setCurrentIndex(bookmarkCount-1);
}

void DialogOpenConnection::choseBookmark() {
    QSettings settings("mir", "Contra");
    qint32 current = ui->comboBox->currentIndex();
    if(ui->comboBox->currentText() == "Edit bookmarks...") {
        bookmarksDialog->loadBookmarks();
        bookmarksDialog->show();
        return;
    }
    ui->autoConnectCheckbox->setDisabled(false);
    ui->lineEdit->setText(settings.value("bookmarkaddress"+QString::number(current)).toString());
    ui->lineEdit_2->setText(settings.value("bookmarklogin"+QString::number(current)).toString());
    ui->lineEdit_3->setText(settings.value("bookmarkpassword"+QString::number(current)).toString());

    QString auto_bookmark = settings.value("autoBookmark", "").toString();
    if(auto_bookmark != "" && auto_bookmark == settings.value("bookmarkaddress"+QString::number(current)).toString()) {
        ui->autoConnectCheckbox->setChecked(true);
    } else {
        ui->autoConnectCheckbox->setChecked(false);
    }

    setFocusOnLogin();
}

void DialogOpenConnection::updateAutoConnectStatus()
{
    QSettings settings("mir", "Contra");
    if(ui->autoConnectCheckbox->isChecked()) {
        settings.setValue("autoBookmark", ui->lineEdit->text());
    } else {
        settings.setValue("autoBookmark", "");
    }
}

void DialogOpenConnection::updateBookmarkList() {
    QSettings settings("mir", "Contra");
    ui->comboBox->clear();
    ui->comboBox->insertSeparator(0);
    ui->comboBox->addItem("Edit bookmarks...");
    qint32 bookmarkCount = settings.value("bookmarkCount", 0).toInt();
    for(qint32 i=0; i<bookmarkCount; i++) {
        ui->comboBox->insertItem(ui->comboBox->count()-2, settings.value("bookmarkname"+QString::number(i)).toString());
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
