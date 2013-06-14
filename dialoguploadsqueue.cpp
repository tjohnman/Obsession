#include "dialoguploadsqueue.h"
#include "ui_dialoguploadsqueue.h"

DialogUploadsQueue::DialogUploadsQueue(UploadManager * u, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DialogUploadsQueue)
{
    ui->setupUi(this);

    uploadManager = u;
    uploadManager->listWidget = ui->listWidget;
    connect(ui->pushButton, SIGNAL(clicked()), uploadManager, SLOT(cleanIdle()));
}

DialogUploadsQueue::~DialogUploadsQueue()
{
    delete ui;
    delete uploadManager;
}

void DialogUploadsQueue::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
