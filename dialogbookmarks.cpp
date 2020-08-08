#include "dialogbookmarks.h"
#include "ui_dialogbookmarks.h"
#include <QDebug>

DialogBookmarks::DialogBookmarks(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogBookmarks)
{
    ui->setupUi(this);

    //setWindowFlags(Qt::WindowTitleHint);

    connect(ui->listWidget, SIGNAL(clicked(QModelIndex)), this, SLOT(selectedBookmark()));
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(newBookmark()));
    connect(ui->pushButton_2, SIGNAL(clicked()), SLOT(deleteBookmark()));

    connect(ui->lineEdit, SIGNAL(textEdited(QString)), this, SLOT(saveBookmark()));
    connect(ui->lineEdit_2, SIGNAL(textEdited(QString)), this, SLOT(saveBookmark()));
    connect(ui->lineEdit_3, SIGNAL(textEdited(QString)), this, SLOT(saveBookmark()));
    connect(ui->lineEdit_4, SIGNAL(textEdited(QString)), this, SLOT(saveBookmark()));
    connect(ui->autoConnectCheckBox, SIGNAL(clicked(bool)), this, SLOT(changedAutoConnect(bool)));

    connect(this, SIGNAL(finished(int)), this, SLOT(saveBookmark()));

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

        QString auto_bookmark = settings.value("autoBookmark", "").toString();
        if(auto_bookmark != "" && auto_bookmark == settings.value("bookmarkaddress"+QString::number(ui->listWidget->currentRow())).toString()) {
            ui->autoConnectCheckBox->setChecked(true);
        } else {
            ui->autoConnectCheckBox->setChecked(false);
        }
    }
}

void DialogBookmarks::loadBookmarks() {
    QSettings settings("mir", "Contra");
    ui->listWidget->clear();
    qint32 bookmarkCount = settings.value("bookmarkCount", 0).toInt();
    for(qint32 i=0; i<bookmarkCount; i++) {
        ui->listWidget->addItem(settings.value("bookmarkname"+QString::number(i)).toString());

        QString auto_bookmark = settings.value("autoBookmark", "").toString();
        if(auto_bookmark != "" && auto_bookmark == settings.value("bookmarkaddress"+QString::number(i)).toString()) {
            QFont font = ui->listWidget->item(i)->font();
            font.setBold(true);
            ui->listWidget->item(i)->setFont(font);
        }
    }
    ui->listWidget->setCurrentRow(0);
    selectedBookmark();
}

void DialogBookmarks::newBookmark() {
    QSettings settings("mir", "Contra");
    qint32 bookmarkCount = settings.value("bookmarkCount", 0).toInt();
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
        qint32 row = ui->listWidget->currentRow();
        qint32 bookmarkCount = settings.value("bookmarkCount", 0).toInt();
        settings.remove("bookmarkname"+QString::number(row));
        settings.remove("bookmarkaddress"+QString::number(row));
        settings.remove("bookmarklogin"+QString::number(row));
        settings.remove("bookmarkpassword"+QString::number(row));
        for(qint32 i=row; i<bookmarkCount-1; i++) {
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

void DialogBookmarks::changedAutoConnect(bool checked) {
    QSettings settings("mir", "Contra");

    if(checked) {
        settings.setValue("autoBookmark", ui->lineEdit_2->text());
    } else {
        settings.setValue("autoBookmark", "");
    }

    qint32 bookmarkCount = settings.value("bookmarkCount", 0).toInt();
    for(qint32 i=0; i<bookmarkCount; i++) {
        QString auto_bookmark = settings.value("autoBookmark", "").toString();
        QFont font = ui->listWidget->item(i)->font();
        if(auto_bookmark == settings.value("bookmarkaddress"+QString::number(i)).toString()) {
            font.setBold(true);
        } else {
            font.setBold(false);
        }
        ui->listWidget->item(i)->setFont(font);
    }
}
