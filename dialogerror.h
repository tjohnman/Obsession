#ifndef DIALOGERROR_H
#define DIALOGERROR_H

#include <QDialog>

namespace Ui {
    class DialogError;
}

class DialogError : public QDialog {
    Q_OBJECT
public:
    DialogError(QString message, QWidget *parent = 0);
    ~DialogError();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DialogError *ui;
};

#endif // DIALOGERROR_H
