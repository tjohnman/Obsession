/********************************************************************************
** Form generated from reading UI file 'dialogerror.ui'
**
** Created: Thu 21. Oct 04:13:21 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGERROR_H
#define UI_DIALOGERROR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_DialogError
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QDialog *DialogError)
    {
        if (DialogError->objectName().isEmpty())
            DialogError->setObjectName(QString::fromUtf8("DialogError"));
        DialogError->resize(230, 99);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(DialogError->sizePolicy().hasHeightForWidth());
        DialogError->setSizePolicy(sizePolicy);
        DialogError->setMinimumSize(QSize(230, 99));
        DialogError->setMaximumSize(QSize(230, 99));
        DialogError->setModal(true);
        gridLayout = new QGridLayout(DialogError);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(DialogError);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignCenter);
        label->setWordWrap(true);

        gridLayout->addWidget(label, 0, 0, 1, 3);

        horizontalSpacer = new QSpacerItem(60, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 0, 1, 1);

        pushButton = new QPushButton(DialogError);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        gridLayout->addWidget(pushButton, 1, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(59, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 1, 2, 1, 1);


        retranslateUi(DialogError);
        QObject::connect(pushButton, SIGNAL(clicked()), DialogError, SLOT(accept()));

        QMetaObject::connectSlotsByName(DialogError);
    } // setupUi

    void retranslateUi(QDialog *DialogError)
    {
        DialogError->setWindowTitle(QApplication::translate("DialogError", "Attention", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("DialogError", "Unknown error.", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("DialogError", "OK", 0, QApplication::UnicodeUTF8));
        pushButton->setShortcut(QApplication::translate("DialogError", "Return", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DialogError: public Ui_DialogError {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGERROR_H
