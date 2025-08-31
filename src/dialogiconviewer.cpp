#include "dialogiconviewer.h"
#include "ui_dialogiconviewer.h"

DialogIconViewer::DialogIconViewer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogIconViewer)
{
    ui->setupUi(this);
    ui->listWidget->setIconSize(QSize(232, 18));

    connect(ui->listWidget, SIGNAL(clicked(QModelIndex)), this, SLOT(onClicked(QModelIndex)));

    QDir iconsFolder = QDir(":/icons");
    iconsFolder.setSorting(QDir::Name | QDir::LocaleAware);
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
    QSettings settings("mir", "Contra");
    QString name = model.data(0).toString();
    if(name.endsWith(".png")) {
        name = name.left(name.indexOf('.'));
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
