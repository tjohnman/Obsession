#include "dialogiconviewer.h"
#include "ui_dialogiconviewer.h"
#include <QDir>
#include <QImage>
#include <QListWidgetItem>
#include <QDebug>
#include <QThread>
#include <QSettings>


class LoaderThread : public QThread {
public:
    Ui::DialogIconViewer * ui;
    void run() {
        QDir iconsFolder = QDir("icons");
        qDebug() << QDir::currentPath();
        qDebug() << "Loading" << iconsFolder.count() << "icons...";
        QFileInfoList fileList = iconsFolder.entryInfoList();
        ui->progressBar->setMaximum(iconsFolder.count()-2);
        ui->progressBar->setValue(0);
        for(unsigned int i=2; i<iconsFolder.count(); i++) {
            QListWidgetItem * item = new QListWidgetItem();
            item->setSizeHint(QSize(232, 18));
            item->setIcon(QIcon(QPixmap::fromImage(QImage(fileList[i].absoluteFilePath()))));
            item->setData(Qt::DisplayRole, fileList[i].fileName());
            ui->listWidget->addItem(item);
            ui->progressBar->setValue(i);
        }
    }
};

DialogIconViewer::DialogIconViewer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogIconViewer)
{
    ui->setupUi(this);
    ui->listWidget->setIconSize(QSize(232, 18));

    LoaderThread * loaderThread = new LoaderThread();
    loaderThread->ui = ui;
    loaderThread->start();

    connect(ui->listWidget, SIGNAL(clicked(QModelIndex)), this, SLOT(onClicked(QModelIndex)));
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
