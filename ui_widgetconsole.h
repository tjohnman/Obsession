/********************************************************************************
** Form generated from reading UI file 'widgetconsole.ui'
**
** Created by: Qt User Interface Compiler version 5.0.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGETCONSOLE_H
#define UI_WIDGETCONSOLE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WidgetConsole
{
public:
    QGridLayout *gridLayout;
    QPlainTextEdit *plainTextEdit;

    void setupUi(QWidget *WidgetConsole)
    {
        if (WidgetConsole->objectName().isEmpty())
            WidgetConsole->setObjectName(QStringLiteral("WidgetConsole"));
        WidgetConsole->resize(400, 300);
        gridLayout = new QGridLayout(WidgetConsole);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        plainTextEdit = new QPlainTextEdit(WidgetConsole);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));
        plainTextEdit->setReadOnly(false);

        gridLayout->addWidget(plainTextEdit, 0, 0, 1, 1);


        retranslateUi(WidgetConsole);

        QMetaObject::connectSlotsByName(WidgetConsole);
    } // setupUi

    void retranslateUi(QWidget *WidgetConsole)
    {
        WidgetConsole->setWindowTitle(QApplication::translate("WidgetConsole", "Form", 0));
    } // retranslateUi

};

namespace Ui {
    class WidgetConsole: public Ui_WidgetConsole {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGETCONSOLE_H
