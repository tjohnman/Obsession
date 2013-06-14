/********************************************************************************
** Form generated from reading UI file 'dialogprivatemessaging.ui'
**
** Created: Thu 21. Oct 04:13:21 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGPRIVATEMESSAGING_H
#define UI_DIALOGPRIVATEMESSAGING_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QTextBrowser>

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
            DialogPrivateMessaging->setObjectName(QString::fromUtf8("DialogPrivateMessaging"));
        DialogPrivateMessaging->resize(509, 382);
        DialogPrivateMessaging->setMinimumSize(QSize(400, 286));
        gridLayout = new QGridLayout(DialogPrivateMessaging);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(DialogPrivateMessaging);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        lineEdit = new QLineEdit(DialogPrivateMessaging);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        gridLayout->addWidget(lineEdit, 2, 0, 1, 1);

        textEdit = new QTextBrowser(DialogPrivateMessaging);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setOpenExternalLinks(true);

        gridLayout->addWidget(textEdit, 1, 0, 1, 1);


        retranslateUi(DialogPrivateMessaging);

        QMetaObject::connectSlotsByName(DialogPrivateMessaging);
    } // setupUi

    void retranslateUi(QDialog *DialogPrivateMessaging)
    {
        DialogPrivateMessaging->setWindowTitle(QApplication::translate("DialogPrivateMessaging", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("DialogPrivateMessaging", "Private messages with: ", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DialogPrivateMessaging: public Ui_DialogPrivateMessaging {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGPRIVATEMESSAGING_H
