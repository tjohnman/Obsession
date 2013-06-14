/********************************************************************************
** Form generated from reading UI file 'dialoguserinfo.ui'
**
** Created: Sat 23. Apr 02:40:29 2011
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGUSERINFO_H
#define UI_DIALOGUSERINFO_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_DialogUserInfo
{
public:

    void setupUi(QDialog *DialogUserInfo)
    {
        if (DialogUserInfo->objectName().isEmpty())
            DialogUserInfo->setObjectName(QString::fromUtf8("DialogUserInfo"));
        DialogUserInfo->resize(400, 300);

        retranslateUi(DialogUserInfo);

        QMetaObject::connectSlotsByName(DialogUserInfo);
    } // setupUi

    void retranslateUi(QDialog *DialogUserInfo)
    {
        DialogUserInfo->setWindowTitle(QApplication::translate("DialogUserInfo", "Dialog", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DialogUserInfo: public Ui_DialogUserInfo {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGUSERINFO_H
