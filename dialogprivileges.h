#ifndef DIALOGPRIVILEGES_H
#define DIALOGPRIVILEGES_H

#include <QDialog>

class ConnectionController;

namespace Ui {
class DialogPrivileges;
}

class DialogPrivileges : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogPrivileges(ConnectionController * c, QString user, QString password, quint8, quint8, quint8, quint8, quint8, quint8, quint8, quint8, QWidget *parent = 0);
    explicit DialogPrivileges(ConnectionController * c, QString user, QWidget *parent = 0);
    ~DialogPrivileges();
    void readPrivileges();


signals:

private slots:
    void onAccepted();

private:
    quint8 pPrivileges1, pPrivileges2, pPrivileges3, pPrivileges4, pPrivileges5, pPrivileges6, pPrivileges7, pPrivileges8;
    Ui::DialogPrivileges *ui;
    ConnectionController * connection;
};

#endif // DIALOGPRIVILEGES_H
