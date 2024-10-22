#ifndef DIALOGTRACKERS_H
#define DIALOGTRACKERS_H

#include <QDialog>
#include "connectioncontroller.h"
#include <QTcpSocket>
#include <vector>
#include "dialogaddtracker.h"
#include <QModelIndex>
#include "dialogopenconnection.h"

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
    quint16 totalBytes, dataLength, lastPassBytes;
    qint32 numServers;
    quint16 numberOfServers;;

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
    void onServerSelectionChanged();
};

#endif // DIALOGTRACKERS_H
