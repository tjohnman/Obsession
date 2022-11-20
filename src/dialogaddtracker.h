#ifndef DIALOGADDTRACKER_H
#define DIALOGADDTRACKER_H

#include <QDialog>

namespace Ui {
    class DialogAddTracker;
}

class DialogAddTracker : public QDialog {
    Q_OBJECT
public:
    DialogAddTracker(QWidget *parent = 0);
    ~DialogAddTracker();

    QString name();
    QString address();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DialogAddTracker *ui;
    QString pName;
    QString pAddress;

private slots:
    void updateName(QString);
    void updateAddress(QString);
};

#endif // DIALOGADDTRACKER_H
