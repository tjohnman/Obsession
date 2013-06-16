#ifndef DIALOGFILEBROWSER_H
#define DIALOGFILEBROWSER_H

#include <QDialog>
#include "ConnectionController.h"
#include <QModelIndex>
#include <QFile>
#include <QDragEnterEvent>
#include <QDropEvent>

namespace Ui {
    class DialogFileBrowser;
}

class DialogFileBrowser : public QDialog {
    Q_OBJECT
public:
    DialogFileBrowser(ConnectionController * c, QWidget *parent = 0);
    ~DialogFileBrowser();

    void load();
    void resetPath();

    void requestUpload();

protected:
    void changeEvent(QEvent *e);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);


private:
    Ui::DialogFileBrowser *ui;
    ConnectionController * connection;
    QString path;
    QString getExtension(QString name);
    QFile uploadedFile;

private slots:
    void onGotFileList(std::vector<s_hotlineFile *>);
    void onDoubleClick(QModelIndex);
    void goDirectoryUp();
    void requestFile();
    void selectFileToUpload();

signals:
    void requestedFile(QString, qint32, QString);
    void requestedUpload(QString, qint32);
};

#endif // DIALOGFILEBROWSER_H
