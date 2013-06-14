#ifndef DIALOGPREFERENCES_H
#define DIALOGPREFERENCES_H

#include <QDialog>
#include "DialogBookmarks.h"
#include <QFontDialog>

namespace Ui {
    class DialogPreferences;
}

class DialogPreferences : public QDialog {
    Q_OBJECT
public:
    DialogPreferences(QWidget *parent = 0);
    ~DialogPreferences();
    QString serverIdent;

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DialogPreferences *ui;
    DialogBookmarks * bookmarksDialog;

private slots:
    void acceptSettings();
    void openBookmarks();
    void changeFont();

signals:
    void saved();
};

#endif // DIALOGPREFERENCES_H
