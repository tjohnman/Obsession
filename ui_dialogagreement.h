/********************************************************************************
** Form generated from reading UI file 'dialogagreement.ui'
**
** Created by: Qt User Interface Compiler version 5.0.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGAGREEMENT_H
#define UI_DIALOGAGREEMENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPlainTextEdit>

QT_BEGIN_NAMESPACE

class Ui_DialogAgreement
{
public:
    QGridLayout *gridLayout;
    QPlainTextEdit *plainTextEdit;

    void setupUi(QDialog *DialogAgreement)
    {
        if (DialogAgreement->objectName().isEmpty())
            DialogAgreement->setObjectName(QStringLiteral("DialogAgreement"));
        DialogAgreement->resize(589, 450);
        gridLayout = new QGridLayout(DialogAgreement);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        plainTextEdit = new QPlainTextEdit(DialogAgreement);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));
        plainTextEdit->setReadOnly(true);

        gridLayout->addWidget(plainTextEdit, 0, 0, 1, 1);


        retranslateUi(DialogAgreement);

        QMetaObject::connectSlotsByName(DialogAgreement);
    } // setupUi

    void retranslateUi(QDialog *DialogAgreement)
    {
        DialogAgreement->setWindowTitle(QApplication::translate("DialogAgreement", "Server Agreement", 0));
    } // retranslateUi

};

namespace Ui {
    class DialogAgreement: public Ui_DialogAgreement {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGAGREEMENT_H
