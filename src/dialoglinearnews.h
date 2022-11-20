#ifndef DIALOGLINEARNEWS_H
#define DIALOGLINEARNEWS_H

#include <QDialog>
#include "connectioncontroller.h"

namespace Ui {
    class DialogLinearNews;
}

class DialogLinearNews : public QDialog {
    Q_OBJECT
public:
    DialogLinearNews(ConnectionController * c, QWidget *parent = 0);
    ~DialogLinearNews();
    void requestNews();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DialogLinearNews *ui;
    ConnectionController * connection;

private slots:
    void onPostButton();
    void onRefreshButton();

public slots:
    void updateNews(QString);
    void onPostNews(QString);
};

#endif // DIALOGLINEARNEWS_H
