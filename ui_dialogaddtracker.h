/********************************************************************************
** Form generated from reading UI file 'dialogaddtracker.ui'
**
** Created by: Qt User Interface Compiler version 5.0.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGADDTRACKER_H
#define UI_DIALOGADDTRACKER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_DialogAddTracker
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QLabel *label_2;
    QLineEdit *lineEdit_2;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *DialogAddTracker)
    {
        if (DialogAddTracker->objectName().isEmpty())
            DialogAddTracker->setObjectName(QStringLiteral("DialogAddTracker"));
        DialogAddTracker->resize(354, 110);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(DialogAddTracker->sizePolicy().hasHeightForWidth());
        DialogAddTracker->setSizePolicy(sizePolicy);
        DialogAddTracker->setMinimumSize(QSize(354, 110));
        DialogAddTracker->setMaximumSize(QSize(354, 110));
        gridLayout = new QGridLayout(DialogAddTracker);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label = new QLabel(DialogAddTracker);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        lineEdit = new QLineEdit(DialogAddTracker);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        gridLayout->addWidget(lineEdit, 0, 1, 1, 1);

        label_2 = new QLabel(DialogAddTracker);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        lineEdit_2 = new QLineEdit(DialogAddTracker);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));

        gridLayout->addWidget(lineEdit_2, 1, 1, 1, 1);

        buttonBox = new QDialogButtonBox(DialogAddTracker);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 2, 1, 1, 1);


        retranslateUi(DialogAddTracker);
        QObject::connect(buttonBox, SIGNAL(accepted()), DialogAddTracker, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DialogAddTracker, SLOT(reject()));

        QMetaObject::connectSlotsByName(DialogAddTracker);
    } // setupUi

    void retranslateUi(QDialog *DialogAddTracker)
    {
        DialogAddTracker->setWindowTitle(QApplication::translate("DialogAddTracker", "Dialog", 0));
        label->setText(QApplication::translate("DialogAddTracker", "Tracker name:", 0));
        label_2->setText(QApplication::translate("DialogAddTracker", "Tracker address:", 0));
    } // retranslateUi

};

namespace Ui {
    class DialogAddTracker: public Ui_DialogAddTracker {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGADDTRACKER_H
