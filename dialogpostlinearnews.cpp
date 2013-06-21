#include "dialogpostlinearnews.h"
#include "ui_dialogpostlinearnews.h"

DialogPostLinearNews::DialogPostLinearNews(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPostLinearNews)
{
    ui->setupUi(this);

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(onPost()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

DialogPostLinearNews::~DialogPostLinearNews()
{
    delete ui;
}

void DialogPostLinearNews::onPost()
{
    emit postedText(ui->plainTextEdit->toPlainText());
}
