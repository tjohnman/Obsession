/********************************************************************************
** Form generated from reading UI file 'dialoguserinfo.ui'
**
** Created by: Qt User Interface Compiler version 5.0.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGUSERINFO_H
#define UI_DIALOGUSERINFO_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_DialogUserInfo
{
public:

    void setupUi(QDialog *DialogUserInfo)
    {
        if (DialogUserInfo->objectName().isEmpty())
            DialogUserInfo->setObjectName(QStringLiteral("DialogUserInfo"));
        DialogUserInfo->resize(400, 300);

        retranslateUi(DialogUserInfo);

        QMetaObject::connectSlotsByName(DialogUserInfo);
    } // setupUi

    void retranslateUi(QDialog *DialogUserInfo)
    {
        DialogUserInfo->setWindowTitle(QApplication::translate("DialogUserInfo", "Dialog", 0));
    } // retranslateUi

};

namespace Ui {
    class DialogUserInfo: public Ui_DialogUserInfo {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGUSERINFO_H
