#include "chatlineedit.h"

ChatLineEdit::ChatLineEdit(QWidget * parent)
{
    this->setParent(parent);
}

void ChatLineEdit::keyPressEvent(QKeyEvent * e) {
    if(e->key() == Qt::Key_Alt) {
        altPressed = true;
        qDebug() << "Alt is pressed";
    }
    QLineEdit::keyPressEvent(e);
}

void ChatLineEdit::keyReleaseEvent(QKeyEvent * e) {
    if(e->key() == Qt::Key_Alt) {
        altPressed = false;
        qDebug() << "Alt is released";
    }
    QLineEdit::keyReleaseEvent(e);
}

void ChatLineEdit::focusOutEvent(QFocusEvent *) {
    altPressed = false;
}
