#include "widgetnews.h"
#include "ui_widgetnews.h"
#include "ctransaction.h"
#include <QSettings>
#include "TextHelper.h"
#include "dialognewnewsmessage.h"

WidgetNews::WidgetNews(ConnectionController * c, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetNews)
{
    ui->setupUi(this);

    ui->pushButtonNew->setEnabled(false);
    ui->pushButtonDelete->setEnabled(false);
    ui->pushButtonReply->setEnabled(false);
    ui->pushButtonNewCategory->setEnabled(false); //true);
    ui->pushButtonNewFolder->setEnabled(false); //true);

    connection = c;
    connect(connection, SIGNAL(gotNewsCategory(unsigned char, QString)), this, SLOT(onNewsCategory(unsigned char,QString)));
    connect(connection, SIGNAL(gotNewsItem(QString, quint32, quint32)), this, SLOT(onNewsItems(QString, quint32, quint32)));
    connect(connection, SIGNAL(gotNewsArticleText(QString, QString, QString)), this, SLOT(onNewsArticleText(QString, QString, QString)));

    connect(ui->treeWidget, SIGNAL(clicked(QModelIndex)), this, SLOT(getNews()));
    connect(ui->treeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(onItemSelectionChanged()));

    connect(ui->pushButtonDelete, SIGNAL(clicked()), this, SLOT(onDeleteClicked()));
    connect(ui->pushButtonNew, SIGNAL(clicked()), this, SLOT(onNewClicked()));
    connect(ui->pushButtonReply, SIGNAL(clicked()), this, SLOT(onReplyClicked()));
}

void WidgetNews::clear() {
    ui->treeWidget->clear();
}

void WidgetNews::getNews() {
    CTransaction * getTransaction = NULL;

    if(!ui->treeWidget->currentItem()) {
        ui->treeWidget->clear();
        getTransaction = connection->createTransaction(370);
    } else {
        if(ui->treeWidget->currentItem()->data(1, 0).toInt() == 0) {
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
            getTransaction = connection->createTransaction(370);
        }
        if(ui->treeWidget->currentItem()->data(1, 0).toInt() == 3) {
            getTransaction = connection->createTransaction(371);
        }
        QString path;
        if(ui->treeWidget->currentItem()->data(1, 0).toInt() == 0) {
            path = getItemPath(ui->treeWidget->currentItem()->parent());
        } else {
            path = getItemPath(ui->treeWidget->currentItem());
        }

        if(path.length() > 1) {
            QStringList levels = path.split(":", Qt::SkipEmptyParts);
            quint16 directorylevels = levels.count();
            quint16 pathlen = 2 + directorylevels * 3;
            for(qint32 i=0; i<levels.count(); i++) {
                QString level = levels.at(i);
                pathlen += level.length();
            }

            char * pathdata = (char *) malloc(sizeof(char)*pathlen);

            directorylevels = qToBigEndian(directorylevels);
            memcpy(pathdata, &directorylevels, 2);

            qint32 offset = 0;
            for(qint32 i=0; i<levels.count(); i++) {
                memset(pathdata+offset+2, 0, 2);
                QString level = levels.at(i);
                unsigned char len = level.length();
                memcpy(pathdata+offset+4, &len, 1);
                memcpy(pathdata+offset+5, TextHelper::EncodeText(level).data(), len);
                offset += 3+len;
            }

            getTransaction->addParameter(325, pathlen, pathdata);
        }
    }

    ui->treeWidget->setEnabled(false);

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

    return _path;
}

void WidgetNews::setRead(QTreeWidgetItem * item, bool set) {
    QSettings settings("mir", "Contra");
    QString key = item->data(0, 0).toString()+item->data(1, 0).toString()+item->data(2, 0).toString()+item->data(3, 0).toString();
    settings.setValue(key, set);
}

bool WidgetNews::checkIfRead(QTreeWidgetItem * item) {
    QSettings settings("mir", "Contra");
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

    ui->pushButtonNew->setEnabled(true);
}

void WidgetNews::onNewsCategory(unsigned char _type, QString _name) {
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

void WidgetNews::onItemSelectionChanged() {
    /*const QTreeWidgetItem * item = ui->treeWidget->currentItem();

    if (item) {
        ui->pushButtonDelete->setEnabled(true);
        ui->pushButtonReply->setEnabled(true);
    } else {
        ui->pushButtonDelete->setEnabled(false);
        ui->pushButtonReply->setEnabled(false);
    }*/
}

void WidgetNews::onDeleteClicked() {

}

void WidgetNews::onReplyClicked() {

}

void WidgetNews::onNewClicked() {
    DialogNewNewsMessage * dialog = new DialogNewNewsMessage(this);

    connect(dialog, SIGNAL(accepted()), dialog, SLOT(deleteLater()));
    connect(dialog, SIGNAL(rejected()), dialog, SLOT(deleteLater()));
    connect(dialog, SIGNAL(messageSubmitted(QString,QString,QString,quint32)), this, SLOT(onMessageSubmitted(QString,QString,QString,quint32)));

    dialog->show();
}

void WidgetNews::onMessageSubmitted(const QString title, const QString message, const QString path, const quint32 parentId)
{
    CTransaction * transaction = connection->createTransaction(410);

    const QByteArray pathData = TextHelper::EncodeText(path);
    transaction->addParameter(325, pathData.length(), pathData.constData());

    if (parentId) transaction->addParameter(326, parentId);

    const QByteArray flavorData = TextHelper::EncodeText("text/plain");
    transaction->addParameter(327, flavorData.length(), flavorData.constData());

    const QByteArray titleData = TextHelper::EncodeText(title);
    transaction->addParameter(328, titleData.length(), titleData.constData());

    const QByteArray messageData = TextHelper::EncodeText(message);
    transaction->addParameter(333, messageData.length(), messageData.constData());

    connection->sendTransaction(transaction);
}

































