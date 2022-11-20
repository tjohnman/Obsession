#ifndef DIALOGUPLOADSQUEUE_H
#define DIALOGUPLOADSQUEUE_H

#include <QWidget>
#include "uploadmanager.h"

namespace Ui {
    class DialogUploadsQueue;
}

class DialogUploadsQueue : public QWidget {
    Q_OBJECT
public:
    DialogUploadsQueue(UploadManager * u, QWidget *parent = 0);
    ~DialogUploadsQueue();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DialogUploadsQueue *ui;
    UploadManager * uploadManager;
};

#endif // DIALOGUPLOADSQUEUE_H
