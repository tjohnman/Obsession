#ifndef DIALOGBOOKMARKS_H
#define DIALOGBOOKMARKS_H

#include <QDialog>
#include <QSettings>

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
    void saveBookmark();
    void selectedBookmark();
    void clearFields();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DialogBookmarks *ui;
};

#endif // DIALOGBOOKMARKS_H
