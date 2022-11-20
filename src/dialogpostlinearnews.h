#ifndef DIALOGPOSTLINEARNEWS_H
#define DIALOGPOSTLINEARNEWS_H

#include <QDialog>

namespace Ui {
class DialogPostLinearNews;
}

class DialogPostLinearNews : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogPostLinearNews(QWidget *parent = 0);
    ~DialogPostLinearNews();

signals:
    void postedText(QString text);

private slots:
    void onPost();
    
private:
    Ui::DialogPostLinearNews *ui;
};

#endif // DIALOGPOSTLINEARNEWS_H
