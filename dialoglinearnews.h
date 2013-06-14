#ifndef DIALOGLINEARNEWS_H
#define DIALOGLINEARNEWS_H

#include <QDialog>
#include "ConnectionController.h"

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

public slots:
    void updateNews(QString);
};

#endif // DIALOGLINEARNEWS_H
