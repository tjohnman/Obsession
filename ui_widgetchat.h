/********************************************************************************
** Form generated from reading UI file 'widgetchat.ui'
**
** Created: Sat 23. Apr 02:30:56 2011
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGETCHAT_H
#define UI_WIDGETCHAT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QSpacerItem>
#include <QtGui/QTextBrowser>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "chatlineedit.h"
#include "listwidgetusers.h"

QT_BEGIN_NAMESPACE

class Ui_WidgetChat
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QTextBrowser *textEdit;
    QSpacerItem *verticalSpacer;
    ChatLineEdit *lineEdit;
    QSpacerItem *horizontalSpacer;
    ListWidgetUsers *listWidget;

    void setupUi(QWidget *WidgetChat)
    {
        if (WidgetChat->objectName().isEmpty())
            WidgetChat->setObjectName(QString::fromUtf8("WidgetChat"));
        WidgetChat->resize(780, 535);
        gridLayout = new QGridLayout(WidgetChat);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(10, 10, 10, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        textEdit = new QTextBrowser(WidgetChat);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        textEdit->setOpenExternalLinks(true);

        verticalLayout->addWidget(textEdit);

        verticalSpacer = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer);

        lineEdit = new ChatLineEdit(WidgetChat);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        verticalLayout->addWidget(lineEdit);


        horizontalLayout->addLayout(verticalLayout);

        horizontalSpacer = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        listWidget = new ListWidgetUsers(WidgetChat);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(listWidget->sizePolicy().hasHeightForWidth());
        listWidget->setSizePolicy(sizePolicy);
        listWidget->setMinimumSize(QSize(236, 0));
        listWidget->setMaximumSize(QSize(200, 16777215));
        listWidget->setIconSize(QSize(232, 18));
        listWidget->setResizeMode(QListView::Adjust);

        horizontalLayout->addWidget(listWidget);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);


        retranslateUi(WidgetChat);

        QMetaObject::connectSlotsByName(WidgetChat);
    } // setupUi

    void retranslateUi(QWidget *WidgetChat)
    {
        WidgetChat->setWindowTitle(QApplication::translate("WidgetChat", "Form", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class WidgetChat: public Ui_WidgetChat {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGETCHAT_H
