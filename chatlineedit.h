#ifndef CHATLINEEDIT_H
#define CHATLINEEDIT_H

#include <QLineEdit>
#include <QKeyEvent>
#include <QDebug>
#include <QFocusEvent>

class ChatLineEdit : public QLineEdit
{
public:
    ChatLineEdit(QWidget * parent);
    bool altPressed;
protected:
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
    void focusOutEvent(QFocusEvent *);
};

#endif // CHATLINEEDIT_H
