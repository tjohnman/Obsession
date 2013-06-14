#ifndef DIALOGDOWNLOADQUEUE_H
#define DIALOGDOWNLOADQUEUE_H

#include <QDialog>
#include "downloadmanager.h"
#include <QListWidgetItem>

namespace Ui {
    class DialogDownloadQueue;
}

class DialogDownloadQueue : public QDialog {
    Q_OBJECT
public:
    DialogDownloadQueue(DownloadManager * d, QWidget *parent = 0);
    ~DialogDownloadQueue();
    bool open;

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DialogDownloadQueue *ui;
    DownloadManager * downloadManager;

public slots:
};

#endif // DIALOGDOWNLOADQUEUE_H
