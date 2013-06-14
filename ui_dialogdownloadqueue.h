/********************************************************************************
** Form generated from reading UI file 'dialogdownloadqueue.ui'
**
** Created by: Qt User Interface Compiler version 5.0.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGDOWNLOADQUEUE_H
#define UI_DIALOGDOWNLOADQUEUE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_DialogDownloadQueue
{
public:
    QGridLayout *gridLayout;
    QListWidget *listWidget;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;

    void setupUi(QDialog *DialogDownloadQueue)
    {
        if (DialogDownloadQueue->objectName().isEmpty())
            DialogDownloadQueue->setObjectName(QStringLiteral("DialogDownloadQueue"));
        DialogDownloadQueue->resize(400, 372);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(DialogDownloadQueue->sizePolicy().hasHeightForWidth());
        DialogDownloadQueue->setSizePolicy(sizePolicy);
        DialogDownloadQueue->setMinimumSize(QSize(400, 0));
        DialogDownloadQueue->setMaximumSize(QSize(16777215, 16777215));
        gridLayout = new QGridLayout(DialogDownloadQueue);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        listWidget = new QListWidget(DialogDownloadQueue);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setResizeMode(QListView::Adjust);

        gridLayout->addWidget(listWidget, 0, 0, 1, 2);

        horizontalSpacer = new QSpacerItem(288, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 0, 1, 1);

        pushButton = new QPushButton(DialogDownloadQueue);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy1);
        pushButton->setMinimumSize(QSize(85, 0));
        QIcon icon;
        icon.addFile(QStringLiteral(":/general/interfaceIcons/generalClean.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton->setIcon(icon);
        pushButton->setIconSize(QSize(24, 24));

        gridLayout->addWidget(pushButton, 1, 1, 1, 1);


        retranslateUi(DialogDownloadQueue);

        QMetaObject::connectSlotsByName(DialogDownloadQueue);
    } // setupUi

    void retranslateUi(QDialog *DialogDownloadQueue)
    {
        DialogDownloadQueue->setWindowTitle(QApplication::translate("DialogDownloadQueue", "Download queue", 0));
        pushButton->setText(QApplication::translate("DialogDownloadQueue", "Clean list", 0));
    } // retranslateUi

};

namespace Ui {
    class DialogDownloadQueue: public Ui_DialogDownloadQueue {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGDOWNLOADQUEUE_H
