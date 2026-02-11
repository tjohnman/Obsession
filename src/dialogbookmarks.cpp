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
    QSettings settings(QString::fromUtf8("mir"), QString::fromUtf8("Contra"));
    if(ui->listWidget->currentItem()) {
        ui->lineEdit->setText(settings.value(QString::fromUtf8("bookmarkname")+QString::number(ui->listWidget->currentRow()),  QString::fromUtf8("")).toString());
        ui->lineEdit_2->setText(settings.value(QString::fromUtf8("bookmarkaddress")+QString::number(ui->listWidget->currentRow()),  QString::fromUtf8("")).toString());
        ui->lineEdit_3->setText(settings.value(QString::fromUtf8("bookmarklogin")+QString::number(ui->listWidget->currentRow()),  QString::fromUtf8("")).toString());
        ui->lineEdit_4->setText(settings.value(QString::fromUtf8("bookmarkpassword")+QString::number(ui->listWidget->currentRow()),  QString::fromUtf8("")).toString());

        QString auto_bookmark = settings.value(QString::fromUtf8("autoBookmark"), QString::fromUtf8("")).toString();
        if(auto_bookmark != QString::fromUtf8("") && auto_bookmark == settings.value(QString::fromUtf8("bookmarkaddress")+QString::number(ui->listWidget->currentRow())).toString()) {
            ui->autoConnectCheckBox->setChecked(true);
        } else {
            ui->autoConnectCheckBox->setChecked(false);
        }
    } else {
        clearFields();
    }
}

void DialogBookmarks::loadBookmarks() {
    QSettings settings(QString::fromUtf8("mir"), QString::fromUtf8("Contra"));
    ui->listWidget->clear();
    qint32 bookmarkCount = settings.value("bookmarkCount", 0).toInt();
    for(qint32 i=0; i<bookmarkCount; i++) {
        ui->listWidget->addItem(settings.value(QString::fromUtf8("bookmarkname")+QString::number(i)).toString());

        QString auto_bookmark = settings.value(QString::fromUtf8("autoBookmark"), QString::fromUtf8("")).toString();
        if(auto_bookmark != QString::fromUtf8("") && auto_bookmark == settings.value(QString::fromUtf8("bookmarkaddress")+QString::number(i)).toString()) {
            QFont font = ui->listWidget->item(i)->font();
            font.setBold(true);
            ui->listWidget->item(i)->setFont(font);
        }
    }
    ui->listWidget->setCurrentRow(0);
    selectedBookmark();
}

void DialogBookmarks::newBookmark() {
    QSettings settings(QString::fromUtf8("mir"), QString::fromUtf8("Contra"));
    qint32 bookmarkCount = settings.value("bookmarkCount", 0).toInt();
    ui->lineEdit->setText(QString::fromUtf8("New bookmark"));
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    ui->listWidget->addItem(QString::fromUtf8("New bookmark"));
    settings.setValue(QString::fromUtf8("bookmarkname")+QString::number(bookmarkCount), ui->lineEdit->text());
    settings.setValue(QString::fromUtf8("bookmarkaddress")+QString::number(bookmarkCount), ui->lineEdit_2->text());
    settings.setValue(QString::fromUtf8("bookmarklogin")+QString::number(bookmarkCount), ui->lineEdit_3->text());
    settings.setValue(QString::fromUtf8("bookmarkpassword")+QString::number(bookmarkCount), ui->lineEdit_4->text());
    bookmarkCount++;
    settings.setValue("bookmarkCount", bookmarkCount);

    ui->listWidget->setCurrentRow(ui->listWidget->count()-1);
    ui->lineEdit->setFocus();
}

void DialogBookmarks::deleteBookmark() {
    QSettings settings(QString::fromUtf8("mir"), QString::fromUtf8("Contra"));
    if(ui->listWidget->currentItem()) {
        qint32 row = ui->listWidget->currentRow();
        qint32 bookmarkCount = settings.value("bookmarkCount", 0).toInt();
        settings.remove(QString::fromUtf8("bookmarkname")+QString::number(row));
        settings.remove(QString::fromUtf8("bookmarkaddress")+QString::number(row));
        settings.remove(QString::fromUtf8("bookmarklogin")+QString::number(row));
        settings.remove(QString::fromUtf8("bookmarkpassword")+QString::number(row));
        for(qint32 i=row; i<bookmarkCount-1; i++) {
            settings.setValue(QString::fromUtf8("bookmarkname")+QString::number(i), settings.value(QString::fromUtf8("bookmarkname")+QString::number(i+1)).toString());
            settings.setValue(QString::fromUtf8("bookmarkaddress")+QString::number(i), settings.value(QString::fromUtf8("bookmarkaddress")+QString::number(i+1)).toString());
            settings.setValue(QString::fromUtf8("bookmarklogin")+QString::number(i), settings.value(QString::fromUtf8("bookmarklogin")+QString::number(i+1)).toString());
            settings.setValue(QString::fromUtf8("bookmarkpassword")+QString::number(i), settings.value(QString::fromUtf8("bookmarkpassword")+QString::number(i+1)).toString());
        }
        settings.remove(QString::fromUtf8("bookmarkname")+QString::number(bookmarkCount-1));
        settings.remove(QString::fromUtf8("bookmarkaddress")+QString::number(bookmarkCount-1));
        settings.remove(QString::fromUtf8("bookmarklogin")+QString::number(bookmarkCount-1));
        settings.remove(QString::fromUtf8("bookmarkpassword")+QString::number(bookmarkCount-1));
        bookmarkCount--;
        settings.setValue("bookmarkCount", bookmarkCount);

        loadBookmarks();
    }
}

void DialogBookmarks::saveBookmark() {
    QSettings settings(QString::fromUtf8("mir"), QString::fromUtf8("Contra"));
    if(ui->listWidget->currentItem()) {
        settings.setValue(QString::fromUtf8("bookmarkname")+QString::number(ui->listWidget->currentRow()), ui->lineEdit->text());
        settings.setValue(QString::fromUtf8("bookmarkaddress")+QString::number(ui->listWidget->currentRow()), ui->lineEdit_2->text());
        settings.setValue(QString::fromUtf8("bookmarklogin")+QString::number(ui->listWidget->currentRow()), ui->lineEdit_3->text());
        settings.setValue(QString::fromUtf8("bookmarkpassword")+QString::number(ui->listWidget->currentRow()), ui->lineEdit_4->text());

        ui->listWidget->item(ui->listWidget->currentRow())->setText(ui->lineEdit->text());
    }
}

void DialogBookmarks::changedAutoConnect(bool checked) {
    QSettings settings(QString::fromUtf8("mir"), QString::fromUtf8("Contra"));

    if(checked) {
        settings.setValue("autoBookmark", ui->lineEdit_2->text());
    } else {
        settings.setValue(QString::fromUtf8("autoBookmark"), QString::fromUtf8(""));
    }

    qint32 bookmarkCount = settings.value("bookmarkCount", 0).toInt();
    for(qint32 i=0; i<bookmarkCount; i++) {
        QString auto_bookmark = settings.value(QString::fromUtf8("autoBookmark"), QString::fromUtf8("")).toString();
        QFont font = ui->listWidget->item(i)->font();
        if(auto_bookmark == settings.value(QString::fromUtf8("bookmarkaddress")+QString::number(i)).toString()) {
            font.setBold(true);
        } else {
            font.setBold(false);
        }
        ui->listWidget->item(i)->setFont(font);
    }
}
