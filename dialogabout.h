#ifndef DIALOGABOUT_H
#define DIALOGABOUT_H

#include <QDialog>

namespace Ui {
    class DialogAbout;
}

class DialogAbout : public QDialog {
    Q_OBJECT
public:
    DialogAbout(QWidget *parent = 0);
    ~DialogAbout();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DialogAbout *ui;
};

#endif // DIALOGABOUT_H
