#ifndef DIALOGICONVIEWER_H
#define DIALOGICONVIEWER_H

#include <QDialog>
#include <QModelIndex>

namespace Ui {
    class DialogIconViewer;
}

class DialogIconViewer : public QDialog {
    Q_OBJECT
public:
    DialogIconViewer(QWidget *parent = 0);
    ~DialogIconViewer();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DialogIconViewer *ui;

signals:
    void iconChanged();

private slots:
    void onClicked(QModelIndex);
};

#endif // DIALOGICONVIEWER_H
