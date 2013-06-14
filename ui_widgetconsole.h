/********************************************************************************
** Form generated from reading UI file 'widgetconsole.ui'
**
** Created: Thu 21. Oct 04:13:21 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGETCONSOLE_H
#define UI_WIDGETCONSOLE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WidgetConsole
{
public:
    QGridLayout *gridLayout;
    QPlainTextEdit *plainTextEdit;

    void setupUi(QWidget *WidgetConsole)
    {
        if (WidgetConsole->objectName().isEmpty())
            WidgetConsole->setObjectName(QString::fromUtf8("WidgetConsole"));
        WidgetConsole->resize(400, 300);
        gridLayout = new QGridLayout(WidgetConsole);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        plainTextEdit = new QPlainTextEdit(WidgetConsole);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
        plainTextEdit->setReadOnly(false);

        gridLayout->addWidget(plainTextEdit, 0, 0, 1, 1);


        retranslateUi(WidgetConsole);

        QMetaObject::connectSlotsByName(WidgetConsole);
    } // setupUi

    void retranslateUi(QWidget *WidgetConsole)
    {
        WidgetConsole->setWindowTitle(QApplication::translate("WidgetConsole", "Form", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class WidgetConsole: public Ui_WidgetConsole {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGETCONSOLE_H
