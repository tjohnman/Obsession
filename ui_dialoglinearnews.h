/********************************************************************************
** Form generated from reading UI file 'dialoglinearnews.ui'
**
** Created: Thu 21. Oct 04:13:21 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGLINEARNEWS_H
#define UI_DIALOGLINEARNEWS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPlainTextEdit>

QT_BEGIN_NAMESPACE

class Ui_DialogLinearNews
{
public:
    QGridLayout *gridLayout;
    QPlainTextEdit *plainTextEdit;

    void setupUi(QDialog *DialogLinearNews)
    {
        if (DialogLinearNews->objectName().isEmpty())
            DialogLinearNews->setObjectName(QString::fromUtf8("DialogLinearNews"));
        DialogLinearNews->resize(400, 300);
        DialogLinearNews->setMinimumSize(QSize(400, 300));
        gridLayout = new QGridLayout(DialogLinearNews);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        plainTextEdit = new QPlainTextEdit(DialogLinearNews);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
        plainTextEdit->setReadOnly(true);

        gridLayout->addWidget(plainTextEdit, 0, 0, 1, 1);


        retranslateUi(DialogLinearNews);

        QMetaObject::connectSlotsByName(DialogLinearNews);
    } // setupUi

    void retranslateUi(QDialog *DialogLinearNews)
    {
        DialogLinearNews->setWindowTitle(QApplication::translate("DialogLinearNews", "Linear News", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DialogLinearNews: public Ui_DialogLinearNews {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGLINEARNEWS_H
