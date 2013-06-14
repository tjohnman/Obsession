#include "dialogbookmarks.h"
#include "ui_dialogbookmarks.h"
#include <QDebug>

DialogBookmarks::DialogBookmarks(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogBookmarks)
{
    ui->setupUi(this);

    connect(ui->listWidget, SIGNAL(clicked(QModelIndex)), this, SLOT(selectedBookmark()));
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(newBookmark()));
    connect(ui->pushButton_2, SIGNAL(clicked()), SLOT(deleteBookmark()));

    connect(ui->lineEdit, SIGNAL(textEdited(QString)), this, SLOT(saveBookmark()));
    connect(ui->lineEdit_2, SIGNAL(textEdited(QString)), this, SLOT(saveBookmark()));
    connect(ui->lineEdit_3, SIGNAL(textEdited(QString)), this, SLOT(saveBookmark()));
    connect(ui->lineEdit_4, SIGNAL(textEdited(QString)), this, SLOT(saveBookmark()));

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(saveBookmark()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(saveBookmark()));

    loadBookmarks();
}

DialogBookmarks::~DialogBookmarks()
{
    delete ui;
}

void DialogBookmarks::changeEvent(QEvent *e)
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

// SLOTS

void DialogBookmarks::clearFields() {
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    ui->lineEdit->setFocus();
}

void DialogBookmarks::selectedBookmark() {
    QSettings settings("mir", "Contra");
    if(ui->listWidget->currentItem()) {
        ui->lineEdit->setText(settings.value("bookmarkname"+QString::number(ui->listWidget->currentRow()), "").toString());
        ui->lineEdit_2->setText(settings.value("bookmarkaddress"+QString::number(ui->listWidget->currentRow()), "").toString());
        ui->lineEdit_3->setText(settings.value("bookmarklogin"+QString::number(ui->listWidget->currentRow()), "").toString());
        ui->lineEdit_4->setText(settings.value("bookmarkpassword"+QString::number(ui->listWidget->currentRow()), "").toString());
    }
}

void DialogBookmarks::loadBookmarks() {
    QSettings settings("mir", "Contra");
    ui->listWidget->clear();
    int bookmarkCount = settings.value("bookmarkCount", 0).toInt();
    for(int i=0; i<bookmarkCount; i++) {
        ui->listWidget->addItem(settings.value("bookmarkname"+QString::number(i)).toString());
    }
    ui->listWidget->setCurrentRow(0);
    selectedBookmark();
}

void DialogBookmarks::newBookmark() {
    QSettings settings("mir", "Contra");
    int bookmarkCount = settings.value("bookmarkCount", 0).toInt();
    ui->lineEdit->setText("New bookmark");
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    ui->listWidget->addItem("New bookmark");
    settings.setValue("bookmarkname"+QString::number(bookmarkCount), ui->lineEdit->text());
    settings.setValue("bookmarkaddress"+QString::number(bookmarkCount), ui->lineEdit_2->text());
    settings.setValue("bookmarklogin"+QString::number(bookmarkCount), ui->lineEdit_3->text());
    settings.setValue("bookmarkpassword"+QString::number(bookmarkCount), ui->lineEdit_4->text());
    bookmarkCount++;
    settings.setValue("bookmarkCount", bookmarkCount);
    qDebug() << bookmarkCount;
    ui->listWidget->setCurrentRow(ui->listWidget->count()-1);
    ui->lineEdit->setFocus();
}

void DialogBookmarks::deleteBookmark() {
    QSettings settings("mir", "Contra");
    if(ui->listWidget->currentItem()) {
        int row = ui->listWidget->currentRow();
        int bookmarkCount = settings.value("bookmarkCount", 0).toInt();
        settings.remove("bookmarkname"+QString::number(row));
        settings.remove("bookmarkaddress"+QString::number(row));
        settings.remove("bookmarklogin"+QString::number(row));
        settings.remove("bookmarkpassword"+QString::number(row));
        for(int i=row; i<bookmarkCount-1; i++) {
            settings.setValue("bookmarkname"+QString::number(i), settings.value("bookmarkname"+QString::number(i+1)).toString());
            settings.setValue("bookmarkaddress"+QString::number(i), settings.value("bookmarkaddress"+QString::number(i+1)).toString());
            settings.setValue("bookmarklogin"+QString::number(i), settings.value("bookmarklogin"+QString::number(i+1)).toString());
            settings.setValue("bookmarkpassword"+QString::number(i), settings.value("bookmarkpassword"+QString::number(i+1)).toString());
        }
        settings.remove("bookmarkname"+QString::number(bookmarkCount-1));
        settings.remove("bookmarkaddress"+QString::number(bookmarkCount-1));
        settings.remove("bookmarklogin"+QString::number(bookmarkCount-1));
        settings.remove("bookmarkpassword"+QString::number(bookmarkCount-1));
        bookmarkCount--;
        settings.setValue("bookmarkCount", bookmarkCount);

        qDebug() << "Deleted item " << ui->listWidget->currentRow();
        loadBookmarks();
    }
}

void DialogBookmarks::saveBookmark() {
    QSettings settings("mir", "Contra");
    if(ui->listWidget->currentItem()) {
        settings.setValue("bookmarkname"+QString::number(ui->listWidget->currentRow()), ui->lineEdit->text());
        settings.setValue("bookmarkaddress"+QString::number(ui->listWidget->currentRow()), ui->lineEdit_2->text());
        settings.setValue("bookmarklogin"+QString::number(ui->listWidget->currentRow()), ui->lineEdit_3->text());
        settings.setValue("bookmarkpassword"+QString::number(ui->listWidget->currentRow()), ui->lineEdit_4->text());

        ui->listWidget->item(ui->listWidget->currentRow())->setText(ui->lineEdit->text());
    }
}
