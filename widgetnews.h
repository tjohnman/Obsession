#ifndef WIDGETNEWS_H
#define WIDGETNEWS_H

#include <QWidget>
#include "ConnectionController.h"

class QTreeWidgetItem;

namespace Ui {
    class WidgetNews;
}

class WidgetNews : public QWidget {
    Q_OBJECT
public:
    WidgetNews(ConnectionController * c, QWidget *parent = 0);
    ~WidgetNews();

    ConnectionController * connection;
    void clear();

public slots:
    void onNewsCategory(unsigned char, QString);
    void onNewsItems(QString, quint32, quint32);
    void getNews();
    void onNewsArticleText(QString, QString, QString);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::WidgetNews *ui;
    QString getItemPath(QTreeWidgetItem *);

    void setRead(QTreeWidgetItem *, bool);
    bool checkIfRead(QTreeWidgetItem *);
};

#endif // WIDGETNEWS_H
