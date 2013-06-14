/********************************************************************************
** Form generated from reading UI file 'dialogiconviewer.ui'
**
** Created: Thu 21. Oct 04:13:21 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGICONVIEWER_H
#define UI_DIALOGICONVIEWER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QProgressBar>

QT_BEGIN_NAMESPACE

class Ui_DialogIconViewer
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QListWidget *listWidget;
    QProgressBar *progressBar;

    void setupUi(QDialog *DialogIconViewer)
    {
        if (DialogIconViewer->objectName().isEmpty())
            DialogIconViewer->setObjectName(QString::fromUtf8("DialogIconViewer"));
        DialogIconViewer->resize(280, 350);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(DialogIconViewer->sizePolicy().hasHeightForWidth());
        DialogIconViewer->setSizePolicy(sizePolicy);
        DialogIconViewer->setMinimumSize(QSize(280, 350));
        DialogIconViewer->setMaximumSize(QSize(280, 16777215));
        gridLayout = new QGridLayout(DialogIconViewer);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(DialogIconViewer);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        listWidget = new QListWidget(DialogIconViewer);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(listWidget->sizePolicy().hasHeightForWidth());
        listWidget->setSizePolicy(sizePolicy1);
        listWidget->setMinimumSize(QSize(242, 0));
        listWidget->setMaximumSize(QSize(99999, 16777215));

        gridLayout->addWidget(listWidget, 1, 0, 1, 1);

        progressBar = new QProgressBar(DialogIconViewer);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setValue(0);
        progressBar->setTextVisible(false);

        gridLayout->addWidget(progressBar, 2, 0, 1, 1);


        retranslateUi(DialogIconViewer);

        QMetaObject::connectSlotsByName(DialogIconViewer);
    } // setupUi

    void retranslateUi(QDialog *DialogIconViewer)
    {
        DialogIconViewer->setWindowTitle(QApplication::translate("DialogIconViewer", "Icon Viewer", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("DialogIconViewer", "Select the icon you want to use:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DialogIconViewer: public Ui_DialogIconViewer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGICONVIEWER_H
