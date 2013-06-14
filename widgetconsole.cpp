#include "widgetconsole.h"
#include "ui_widgetconsole.h"

WidgetConsole::WidgetConsole(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetConsole)
{
    ui->setupUi(this);
}

WidgetConsole::~WidgetConsole()
{
    delete ui;
}

void WidgetConsole::changeEvent(QEvent *e)
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

void WidgetConsole::addText(QString t) {
    ui->plainTextEdit->insertPlainText(t);
}
