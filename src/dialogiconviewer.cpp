#include "dialogiconviewer.h"
#include "ui_dialogiconviewer.h"
#include "threadiconloader.h"
#include "SettingsManager.h"

DialogIconViewer::DialogIconViewer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogIconViewer)
{
    ui->setupUi(this);
    ui->listWidget->setIconSize(QSize(232, 18));

    connect(ui->listWidget, SIGNAL(clicked(QModelIndex)), this, SLOT(onClicked(QModelIndex)));

    QDir iconsFolder = QDir(QString::fromUtf8(":/icons"));
    QFileInfoList fileList = iconsFolder.entryInfoList();

    for(quint32 i=2; i<iconsFolder.count(); i++) {
        QListWidgetItem * item = new QListWidgetItem();
        item->setSizeHint(QSize(232, 18));
        item->setIcon(QIcon(QPixmap::fromImage(QImage(fileList[i].absoluteFilePath()))));
        item->setData(Qt::DisplayRole, fileList[i].fileName());
        ui->listWidget->addItem(item);
    }
}

void DialogIconViewer::onClicked(QModelIndex model) {
    auto& settings = SettingsManager::instance();
    QString name = model.data(0).toString();
    if(name.endsWith(QString::fromUtf8(".png"))) {
        name = name.left(name.length()-4);
    }
    settings.setValue("icon", name);

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
