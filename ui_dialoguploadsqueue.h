/********************************************************************************
** Form generated from reading UI file 'dialoguploadsqueue.ui'
**
** Created: Thu 21. Oct 04:13:21 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGUPLOADSQUEUE_H
#define UI_DIALOGUPLOADSQUEUE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DialogUploadsQueue
{
public:
    QGridLayout *gridLayout;
    QListWidget *listWidget;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;

    void setupUi(QWidget *DialogUploadsQueue)
    {
        if (DialogUploadsQueue->objectName().isEmpty())
            DialogUploadsQueue->setObjectName(QString::fromUtf8("DialogUploadsQueue"));
        DialogUploadsQueue->resize(400, 300);
        gridLayout = new QGridLayout(DialogUploadsQueue);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        listWidget = new QListWidget(DialogUploadsQueue);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setResizeMode(QListView::Adjust);

        gridLayout->addWidget(listWidget, 0, 0, 1, 2);

        horizontalSpacer = new QSpacerItem(288, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 0, 1, 1);

        pushButton = new QPushButton(DialogUploadsQueue);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy);
        pushButton->setMinimumSize(QSize(85, 0));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/general/interfaceIcons/generalClean.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton->setIcon(icon);
        pushButton->setIconSize(QSize(24, 24));

        gridLayout->addWidget(pushButton, 1, 1, 1, 1);


        retranslateUi(DialogUploadsQueue);

        QMetaObject::connectSlotsByName(DialogUploadsQueue);
    } // setupUi

    void retranslateUi(QWidget *DialogUploadsQueue)
    {
        DialogUploadsQueue->setWindowTitle(QApplication::translate("DialogUploadsQueue", "Form", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("DialogUploadsQueue", "Clean list", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DialogUploadsQueue: public Ui_DialogUploadsQueue {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGUPLOADSQUEUE_H
