#ifndef DIALOGTRACKERS_H
#define DIALOGTRACKERS_H

#include <QDialog>
#include "ConnectionController.h"
#include <QTcpSocket>
#include <vector>
#include "dialogaddtracker.h"
#include <QModelIndex>
#include "DialogOpenConnection.h"

namespace Ui {
    class DialogTrackers;
}

class DialogTrackers : public QDialog {
    Q_OBJECT
public:
    DialogTrackers(ConnectionController * c, QWidget *parent = 0);
    ~DialogTrackers();

    ConnectionController * connection;
    QTcpSocket * pSocket;
    std::vector<QString> pTrackerNames;
    std::vector<QString> pTrackerAddresses;

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DialogTrackers *ui;
    void updateTrackerList();
    DialogAddTracker * addTrackerDialog;
    void saveTrackerList();
    bool gotHeader;
    unsigned short totalBytes, dataLength, lastPassBytes;
    int numServers;
    unsigned short numberOfServers;;

private slots:
    void updateServerList(QString);
    void updateCurrentList();
    void sendRequest();
    void onSocketData();
    void addTracker();
    void deleteTracker();
    void socketError(QAbstractSocket::SocketError);
    void connectToServer(QModelIndex);
    void openConnectionWindow();
};

#endif // DIALOGTRACKERS_H
