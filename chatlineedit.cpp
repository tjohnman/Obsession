#include "chatlineedit.h"

ChatLineEdit::ChatLineEdit(QWidget * parent)
{
    this->setParent(parent);
}

void ChatLineEdit::keyPressEvent(QKeyEvent * e) {
    if(e->key() == Qt::Key_Alt) {
        altPressed = true;
    }
    QLineEdit::keyPressEvent(e);
}

void ChatLineEdit::keyReleaseEvent(QKeyEvent * e) {
    if(e->key() == Qt::Key_Alt) {
        altPressed = false;
    }
    QLineEdit::keyReleaseEvent(e);
}

void ChatLineEdit::focusOutEvent(QFocusEvent *) {
    altPressed = false;
}
