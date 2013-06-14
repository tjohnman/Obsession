/********************************************************************************
** Form generated from reading UI file 'dialogerror.ui'
**
** Created by: Qt User Interface Compiler version 5.0.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGERROR_H
#define UI_DIALOGERROR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

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
            DialogError->setObjectName(QStringLiteral("DialogError"));
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
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label = new QLabel(DialogError);
        label->setObjectName(QStringLiteral("label"));
        label->setAlignment(Qt::AlignCenter);
        label->setWordWrap(true);

        gridLayout->addWidget(label, 0, 0, 1, 3);

        horizontalSpacer = new QSpacerItem(60, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 0, 1, 1);

        pushButton = new QPushButton(DialogError);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        gridLayout->addWidget(pushButton, 1, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(59, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 1, 2, 1, 1);


        retranslateUi(DialogError);
        QObject::connect(pushButton, SIGNAL(clicked()), DialogError, SLOT(accept()));

        QMetaObject::connectSlotsByName(DialogError);
    } // setupUi

    void retranslateUi(QDialog *DialogError)
    {
        DialogError->setWindowTitle(QApplication::translate("DialogError", "Attention", 0));
        label->setText(QApplication::translate("DialogError", "Unknown error.", 0));
        pushButton->setText(QApplication::translate("DialogError", "OK", 0));
        pushButton->setShortcut(QApplication::translate("DialogError", "Return", 0));
    } // retranslateUi

};

namespace Ui {
    class DialogError: public Ui_DialogError {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGERROR_H
