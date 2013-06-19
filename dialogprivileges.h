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
    explicit DialogPrivileges(ConnectionController * c, QString user, QString password, quint64 privs, QWidget *parent = 0);
    explicit DialogPrivileges(ConnectionController * c, QString user, QWidget *parent = 0);
    ~DialogPrivileges();
    void readPrivileges();


signals:
    void savedPrivileges(quint64 privs);

private slots:
    void onAccepted();

private:
    quint64 pPrivileges;
    Ui::DialogPrivileges *ui;
    ConnectionController * connection;
};

#endif // DIALOGPRIVILEGES_H
