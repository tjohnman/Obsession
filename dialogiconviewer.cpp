#include "dialogiconviewer.h"
#include "ui_dialogiconviewer.h"
#include "threadiconloader.h"

DialogIconViewer::DialogIconViewer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogIconViewer)
{
    ui->setupUi(this);
    ui->listWidget->setIconSize(QSize(232, 18));

    ThreadIconLoader * loaderThread = new ThreadIconLoader();

    connect(loaderThread, SIGNAL(startIconCount(qint32)), this, SLOT(startIconCount(qint32)));
    connect(loaderThread, SIGNAL(loadedItem(qint32, QListWidgetItem *)), this, SLOT(updateProgress(qint32, QListWidgetItem *)));
    connect(ui->listWidget, SIGNAL(clicked(QModelIndex)), this, SLOT(onClicked(QModelIndex)));

    loaderThread->start();
}

void DialogIconViewer::onClicked(QModelIndex model) {
    QSettings settings("mir", "contra");
    QString name = model.data(0).toString();
    if(name.endsWith(".png")) {
        name = name.left(name.length()-4);
    }
    settings.setValue("icon", name);
    qDebug() << name.toShort();
    emit iconChanged();
}

void DialogIconViewer::updateProgress(qint32 n, QListWidgetItem * item)
{
    ui->listWidget->addItem(item);
    ui->progressBar->setValue(n);
}

void DialogIconViewer::startIconCount(qint32 n)
{
    ui->progressBar->setMaximum(n);
    ui->progressBar->setValue(0);
}

DialogIconViewer::~DialogIconViewer()
{
    delete ui;
}

void DialogIconViewer::changeEvent(QEvent *e)
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
