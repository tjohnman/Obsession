#ifndef DIALOGOPENCONNECTION_H
#define DIALOGOPENCONNECTION_H

#include <QDialog>
#include "connectioncontroller.h"
#include "DialogBookmarks.h"

namespace Ui {
    class DialogOpenConnection;
}

class DialogOpenConnection : public QDialog {
    Q_OBJECT
public:
    DialogOpenConnection(QWidget *parent = 0, ConnectionController * c = 0);
    ~DialogOpenConnection();

    void clear();
    void setAddress(QString);
    void setFocusOnLogin();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DialogOpenConnection *ui;
    ConnectionController * connection;

    DialogBookmarks * bookmarksDialog;

public slots:
    void openConnection();
    void updateBookmarkList();
    void choseBookmark();
    void bookmarkCurrent();
    void updateAutoConnectStatus();
};

#endif // DIALOGOPENCONNECTION_H
