#ifndef WIDGETDOWNLOADITEM_H
#define WIDGETDOWNLOADITEM_H

#include <QWidget>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>

namespace Ui {
    class WidgetDownloadItem;
}

class WidgetDownloadItem : public QWidget {
    Q_OBJECT
public:
    WidgetDownloadItem(QWidget *parent = 0);
    ~WidgetDownloadItem();

    QLabel * nameLabel();
    QLabel * infoLabel();
    QProgressBar * progressBar();
    QPushButton * stopButton();
    QPushButton * goButton();
    QPushButton * queueButton();
protected:
    void changeEvent(QEvent *e);

private:
    Ui::WidgetDownloadItem *ui;
};

#endif // WIDGETDOWNLOADITEM_H
