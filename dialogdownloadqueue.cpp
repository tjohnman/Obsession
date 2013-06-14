#include "dialogdownloadqueue.h"
#include "ui_dialogdownloadqueue.h"

DialogDownloadQueue::DialogDownloadQueue(DownloadManager * d, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogDownloadQueue)
{
    ui->setupUi(this);

    downloadManager = d;
    downloadManager->listWidget = ui->listWidget;
    connect(ui->pushButton, SIGNAL(clicked()), downloadManager, SLOT(cleanIdle()));

    open = false;
}

DialogDownloadQueue::~DialogDownloadQueue()
{
    delete ui;
    delete downloadManager;
}

void DialogDownloadQueue::changeEvent(QEvent *e)
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
