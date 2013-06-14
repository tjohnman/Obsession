#include "widgetdownloaditem.h"
#include "ui_widgetdownloaditem.h"

WidgetDownloadItem::WidgetDownloadItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetDownloadItem)
{
    ui->setupUi(this);
}

QLabel * WidgetDownloadItem::nameLabel() {
    return ui->label;
}

QLabel * WidgetDownloadItem::infoLabel() {
    return ui->label_2;
}

QProgressBar * WidgetDownloadItem::progressBar() {
    return ui->progressBar;
}

QPushButton * WidgetDownloadItem::stopButton() {
    return ui->stopButton;
}

QPushButton * WidgetDownloadItem::goButton() {
    return ui->goButton;
}

QPushButton * WidgetDownloadItem::queueButton() {
    return ui->queueButton;
}

WidgetDownloadItem::~WidgetDownloadItem()
{
    delete ui;
}

void WidgetDownloadItem::changeEvent(QEvent *e)
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
