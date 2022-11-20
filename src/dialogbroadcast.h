#ifndef DIALOGBROADCAST_H
#define DIALOGBROADCAST_H

#include <QDialog>
#include "connectioncontroller.h"

namespace Ui {
class DialogBroadcast;
}

class DialogBroadcast : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogBroadcast(ConnectionController * c, QWidget *parent = 0);
    ~DialogBroadcast();
    
private slots:
    void sendMessage();

private:
    ConnectionController * connection;
    Ui::DialogBroadcast *ui;
};

#endif // DIALOGBROADCAST_H
