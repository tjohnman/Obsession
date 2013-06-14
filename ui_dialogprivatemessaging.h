/********************************************************************************
** Form generated from reading UI file 'dialogprivatemessaging.ui'
**
** Created by: Qt User Interface Compiler version 5.0.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGPRIVATEMESSAGING_H
#define UI_DIALOGPRIVATEMESSAGING_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTextBrowser>

QT_BEGIN_NAMESPACE

class Ui_DialogPrivateMessaging
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QTextBrowser *textEdit;

    void setupUi(QDialog *DialogPrivateMessaging)
    {
        if (DialogPrivateMessaging->objectName().isEmpty())
            DialogPrivateMessaging->setObjectName(QStringLiteral("DialogPrivateMessaging"));
        DialogPrivateMessaging->resize(509, 382);
        DialogPrivateMessaging->setMinimumSize(QSize(400, 286));
        gridLayout = new QGridLayout(DialogPrivateMessaging);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label = new QLabel(DialogPrivateMessaging);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        lineEdit = new QLineEdit(DialogPrivateMessaging);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        gridLayout->addWidget(lineEdit, 2, 0, 1, 1);

        textEdit = new QTextBrowser(DialogPrivateMessaging);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setOpenExternalLinks(true);

        gridLayout->addWidget(textEdit, 1, 0, 1, 1);


        retranslateUi(DialogPrivateMessaging);

        QMetaObject::connectSlotsByName(DialogPrivateMessaging);
    } // setupUi

    void retranslateUi(QDialog *DialogPrivateMessaging)
    {
        DialogPrivateMessaging->setWindowTitle(QApplication::translate("DialogPrivateMessaging", "Dialog", 0));
        label->setText(QApplication::translate("DialogPrivateMessaging", "Private messages with: ", 0));
    } // retranslateUi

};

namespace Ui {
    class DialogPrivateMessaging: public Ui_DialogPrivateMessaging {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGPRIVATEMESSAGING_H
