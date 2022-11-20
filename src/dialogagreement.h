#ifndef DIALOGAGREEMENT_H
#define DIALOGAGREEMENT_H

#include <QDialog>

namespace Ui {
    class DialogAgreement;
}

class DialogAgreement : public QDialog {
    Q_OBJECT
public:
    DialogAgreement(QWidget *parent = 0);
    ~DialogAgreement();

    void setAgreement(QString);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DialogAgreement *ui;
};

#endif // DIALOGAGREEMENT_H
