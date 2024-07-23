#ifndef DIALOGNEWNEWSMESSAGE_H
#define DIALOGNEWNEWSMESSAGE_H

#include <QDialog>

namespace Ui {
class DialogNewNewsMessage;
}

class DialogNewNewsMessage : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNewNewsMessage(QWidget *parent, const QString path = "", const quint32 parentId = 0);
    ~DialogNewNewsMessage();

public slots:
    void onTitleChanged();
    void onMessageChanged();
    void onSendPressed();

signals:
    void messageSubmitted(const QString title, const QString message, const QString path, const quint32 parentId);

private:
    Ui::DialogNewNewsMessage *ui;

    QString pPath;
    quint32 pParentId;
};

#endif // DIALOGNEWNEWSMESSAGE_H
