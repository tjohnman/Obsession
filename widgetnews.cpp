#include "widgetnews.h"
#include "ui_widgetnews.h"
#include "ctransaction.h"
#include <QSettings>
#include "TextHelper.h"

WidgetNews::WidgetNews(ConnectionController * c, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetNews)
{
    ui->setupUi(this);
    connection = c;
    connect(connection, SIGNAL(gotNewsCategory(unsigned char, QString)), this, SLOT(onNewsCategory(unsigned char,QString)));
    connect(connection, SIGNAL(gotNewsItem(QString, quint32, quint32)), this, SLOT(onNewsItems(QString, quint32, quint32)));
    connect(connection, SIGNAL(gotNewsArticleText(QString, QString, QString)), this, SLOT(onNewsArticleText(QString, QString, QString)));
    connect(ui->treeWidget, SIGNAL(clicked(QModelIndex)), this, SLOT(getNews()));
}

void WidgetNews::clear() {
    ui->treeWidget->clear();
}

void WidgetNews::getNews() {
    qDebug() << "Getting threaded news...";
    CTransaction * getTransaction = NULL;

    if(!ui->treeWidget->currentItem()) {
        ui->treeWidget->clear();
        qDebug() << "Creating transaction...";
        getTransaction = connection->createTransaction(370);
    } else {
        if(ui->treeWidget->currentItem()->data(1, 0).toInt() == 0) {
            qDebug() << "Opening news item...";
            setRead(ui->treeWidget->currentItem(), true);
            QFont font;
            font.setBold(false);
            ui->treeWidget->currentItem()->setFont(0, font);
            getTransaction = connection->createTransaction(400);
            getTransaction->addParameter(326, (quint16)ui->treeWidget->currentItem()->data(2, 0).toInt());
            getTransaction->addParameter(327, 10, (char *)"text/plain");
        }
        ui->treeWidget->currentItem()->takeChildren();
        if(ui->treeWidget->currentItem()->data(1, 0).toInt() == 2) {
            qDebug() << "Creating transaction...";
            getTransaction = connection->createTransaction(370);
        }
        if(ui->treeWidget->currentItem()->data(1, 0).toInt() == 3) {
            qDebug() << "Creating transaction...";
            getTransaction = connection->createTransaction(371);
        }
        QString path;
        if(ui->treeWidget->currentItem()->data(1, 0).toInt() == 0) {
            path = getItemPath(ui->treeWidget->currentItem()->parent());
        } else {
            path = getItemPath(ui->treeWidget->currentItem());
        }

        if(path.length() > 1) {

            qDebug() << "Calculating size for path data...";
            QStringList levels = path.split(":", QString::SkipEmptyParts);
            quint16 directorylevels = levels.count();
            quint16 pathlen = 2 + directorylevels * 3;
            for(qint32 i=0; i<levels.count(); i++) {
                QString level = levels.at(i);
                pathlen += level.length();
            }

            char * pathdata = (char *) malloc(sizeof(char)*pathlen);

            qDebug() << "Writing number of levels (" << directorylevels << ")...";
            directorylevels = qToBigEndian(directorylevels);
            memcpy(pathdata, &directorylevels, 2);

            qint32 offset = 0;
            for(qint32 i=0; i<levels.count(); i++) {
                qDebug() << "Writing zeros...";
                memset(pathdata+offset+2, 0, 2);
                QString level = levels.at(i);
                unsigned char len = level.length();
                qDebug() << "Writing name length... " << (quint16) len;
                memcpy(pathdata+offset+4, &len, 1);
                qDebug() << TextHelper::EncodeText(level).data();
                memcpy(pathdata+offset+5, TextHelper::EncodeText(level).data(), len);
                offset += 3+len;
            }

            getTransaction->addParameter(325, pathlen, pathdata);
        }
    }

    ui->treeWidget->setEnabled(false);

    qDebug() << "Sending transaction...";
    connection->sendTransaction(getTransaction, true);
}

WidgetNews::~WidgetNews()
{
    delete ui;
}

void WidgetNews::changeEvent(QEvent *e)
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

QString WidgetNews::getItemPath(QTreeWidgetItem * item) {
    QString _path = item->data(0, 0).toString();

    QTreeWidgetItem * c = item;
    while(c->parent()) {
        _path = c->parent()->data(0, 0).toString() + ":" + _path;
        c = c->parent();
    }

    qDebug() << "Item path is: " << _path;
    return _path;
}

void WidgetNews::setRead(QTreeWidgetItem * item, bool set) {
    QSettings settings("mir", "contra");
    QString key = item->data(0, 0).toString()+item->data(1, 0).toString()+item->data(2, 0).toString()+item->data(3, 0).toString();
    settings.setValue(key, set);
}

bool WidgetNews::checkIfRead(QTreeWidgetItem * item) {
    QSettings settings("mir", "contra");
    QString key = item->data(0, 0).toString()+item->data(1, 0).toString()+item->data(2, 0).toString()+item->data(3, 0).toString();
    return settings.value(key, false).toBool();
}

// SLOTS

void WidgetNews::onNewsArticleText(QString t, QString poster, QString timestamp) {
    ui->posterLabel->setText(poster);
    ui->dateLabel->setText(timestamp);
    ui->plainTextEdit->setPlainText(t);
    ui->treeWidget->setEnabled(true);
}

void WidgetNews::onNewsItems(QString _name, quint32 id, quint32 pid) {
    qDebug() << "Inserting news item in list...";
    QTreeWidgetItem * currentItem = NULL;
    if(ui->treeWidget->currentItem()) {
        currentItem = ui->treeWidget->currentItem();
    }

    QTreeWidgetItem * newItem = new QTreeWidgetItem();

    newItem->setData(0, 0, _name);
    newItem->setData(1, 0, 0);
    newItem->setData(2, 0, id);
    newItem->setData(3, 0, pid);
    newItem->setIcon(0, QIcon(":/news/interfaceIcons/newsItem.png"));

    if(!checkIfRead(newItem)) {
        QFont font;
        font.setBold(true);
        newItem->setFont(0, font);
    }

    if(currentItem) {
        currentItem->addChild(newItem);
        currentItem->setExpanded(true);
    } else {
        ui->treeWidget->addTopLevelItem(newItem);
    }

    ui->treeWidget->setEnabled(true);
}

void WidgetNews::onNewsCategory(unsigned char _type, QString _name) {
    qDebug() << "Inserting new categories in list...";
    QTreeWidgetItem * currentItem = NULL;
    if(ui->treeWidget->currentItem()) {
        currentItem = ui->treeWidget->currentItem();
    }

    QTreeWidgetItem * newItem = new QTreeWidgetItem();

    newItem->setData(0, 0, _name);
    newItem->setData(1, 0, (quint16) _type);

    switch(_type) {
    case 2:
        newItem->setIcon(0, QIcon(":/files/interfaceIcons/filesFolder.png"));
        break;
    case 3:
        newItem->setIcon(0, QIcon(":/news/interfaceIcons/newsCategory.png"));
        break;
    default:
        break;
    }

    if(currentItem) {
        currentItem->addChild(newItem);
        currentItem->setExpanded(true);
    } else {
        ui->treeWidget->addTopLevelItem(newItem);
    }

    ui->treeWidget->setEnabled(true);
}
