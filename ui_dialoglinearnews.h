/********************************************************************************
** Form generated from reading UI file 'dialoglinearnews.ui'
**
** Created by: Qt User Interface Compiler version 5.0.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGLINEARNEWS_H
#define UI_DIALOGLINEARNEWS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPlainTextEdit>

QT_BEGIN_NAMESPACE

class Ui_DialogLinearNews
{
public:
    QGridLayout *gridLayout;
    QPlainTextEdit *plainTextEdit;

    void setupUi(QDialog *DialogLinearNews)
    {
        if (DialogLinearNews->objectName().isEmpty())
            DialogLinearNews->setObjectName(QStringLiteral("DialogLinearNews"));
        DialogLinearNews->resize(400, 300);
        DialogLinearNews->setMinimumSize(QSize(400, 300));
        gridLayout = new QGridLayout(DialogLinearNews);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        plainTextEdit = new QPlainTextEdit(DialogLinearNews);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));
        plainTextEdit->setReadOnly(true);

        gridLayout->addWidget(plainTextEdit, 0, 0, 1, 1);


        retranslateUi(DialogLinearNews);

        QMetaObject::connectSlotsByName(DialogLinearNews);
    } // setupUi

    void retranslateUi(QDialog *DialogLinearNews)
    {
        DialogLinearNews->setWindowTitle(QApplication::translate("DialogLinearNews", "Linear News", 0));
    } // retranslateUi

};

namespace Ui {
    class DialogLinearNews: public Ui_DialogLinearNews {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGLINEARNEWS_H
