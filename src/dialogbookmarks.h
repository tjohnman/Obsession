#ifndef DIALOGBOOKMARKS_H
#define DIALOGBOOKMARKS_H

#include <QDialog>
#include <QSettings>
#include <QAbstractButton>

namespace Ui {
    class DialogBookmarks;
}

class DialogBookmarks : public QDialog {
    Q_OBJECT
public:
    DialogBookmarks(QWidget *parent = 0);
    ~DialogBookmarks();

public slots:
    void loadBookmarks();
    void newBookmark();
    void deleteBookmark();
    void selectedBookmark();
    void saveBookmark();
    void clearFields();
    void changedAutoConnect(bool);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DialogBookmarks *ui;
};

#endif // DIALOGBOOKMARKS_H
