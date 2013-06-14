/********************************************************************************
** Form generated from reading UI file 'dialogagreement.ui'
**
** Created: Thu 21. Oct 04:13:21 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGAGREEMENT_H
#define UI_DIALOGAGREEMENT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPlainTextEdit>

QT_BEGIN_NAMESPACE

class Ui_DialogAgreement
{
public:
    QGridLayout *gridLayout;
    QPlainTextEdit *plainTextEdit;

    void setupUi(QDialog *DialogAgreement)
    {
        if (DialogAgreement->objectName().isEmpty())
            DialogAgreement->setObjectName(QString::fromUtf8("DialogAgreement"));
        DialogAgreement->resize(589, 450);
        gridLayout = new QGridLayout(DialogAgreement);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        plainTextEdit = new QPlainTextEdit(DialogAgreement);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
        plainTextEdit->setReadOnly(true);

        gridLayout->addWidget(plainTextEdit, 0, 0, 1, 1);


        retranslateUi(DialogAgreement);

        QMetaObject::connectSlotsByName(DialogAgreement);
    } // setupUi

    void retranslateUi(QDialog *DialogAgreement)
    {
        DialogAgreement->setWindowTitle(QApplication::translate("DialogAgreement", "Server Agreement", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DialogAgreement: public Ui_DialogAgreement {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGAGREEMENT_H
