/********************************************************************************
** Form generated from reading UI file 'dialogopenconnection.ui'
**
** Created by: Qt User Interface Compiler version 5.0.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGOPENCONNECTION_H
#define UI_DIALOGOPENCONNECTION_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_DialogOpenConnection
{
public:
    QLabel *label;
    QLineEdit *lineEdit;
    QLabel *label_2;
    QLineEdit *lineEdit_2;
    QLabel *label_3;
    QLineEdit *lineEdit_3;
    QCheckBox *checkBox;
    QDialogButtonBox *buttonBox_2;
    QPushButton *pushButton;
    QLabel *label_4;
    QComboBox *comboBox;

    void setupUi(QDialog *DialogOpenConnection)
    {
        if (DialogOpenConnection->objectName().isEmpty())
            DialogOpenConnection->setObjectName(QStringLiteral("DialogOpenConnection"));
        DialogOpenConnection->resize(290, 199);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(DialogOpenConnection->sizePolicy().hasHeightForWidth());
        DialogOpenConnection->setSizePolicy(sizePolicy);
        DialogOpenConnection->setMinimumSize(QSize(290, 199));
        DialogOpenConnection->setMaximumSize(QSize(290, 199));
        DialogOpenConnection->setModal(true);
        label = new QLabel(DialogOpenConnection);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(9, 9, 43, 16));
        lineEdit = new QLineEdit(DialogOpenConnection);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(65, 9, 211, 20));
        label_2 = new QLabel(DialogOpenConnection);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(9, 35, 29, 16));
        lineEdit_2 = new QLineEdit(DialogOpenConnection);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(65, 35, 211, 20));
        label_3 = new QLabel(DialogOpenConnection);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(9, 61, 50, 16));
        lineEdit_3 = new QLineEdit(DialogOpenConnection);
        lineEdit_3->setObjectName(QStringLiteral("lineEdit_3"));
        lineEdit_3->setGeometry(QRect(65, 61, 211, 20));
        lineEdit_3->setEchoMode(QLineEdit::Password);
        checkBox = new QCheckBox(DialogOpenConnection);
        checkBox->setObjectName(QStringLiteral("checkBox"));
        checkBox->setGeometry(QRect(9, 87, 186, 17));
        buttonBox_2 = new QDialogButtonBox(DialogOpenConnection);
        buttonBox_2->setObjectName(QStringLiteral("buttonBox_2"));
        buttonBox_2->setGeometry(QRect(120, 160, 156, 23));
        buttonBox_2->setOrientation(Qt::Horizontal);
        buttonBox_2->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        pushButton = new QPushButton(DialogOpenConnection);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(10, 160, 81, 23));
        label_4 = new QLabel(DialogOpenConnection);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(10, 120, 50, 20));
        label_4->setMinimumSize(QSize(50, 0));
        label_4->setMaximumSize(QSize(50, 16777215));
        comboBox = new QComboBox(DialogOpenConnection);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setGeometry(QRect(66, 120, 211, 20));

        retranslateUi(DialogOpenConnection);
        QObject::connect(buttonBox_2, SIGNAL(rejected()), DialogOpenConnection, SLOT(reject()));

        QMetaObject::connectSlotsByName(DialogOpenConnection);
    } // setupUi

    void retranslateUi(QDialog *DialogOpenConnection)
    {
        DialogOpenConnection->setWindowTitle(QApplication::translate("DialogOpenConnection", "Open Connection", 0));
        label->setText(QApplication::translate("DialogOpenConnection", "Address:", 0));
        label_2->setText(QApplication::translate("DialogOpenConnection", "Login:", 0));
        label_3->setText(QApplication::translate("DialogOpenConnection", "Password:", 0));
#ifndef QT_NO_TOOLTIP
        checkBox->setToolTip(QApplication::translate("DialogOpenConnection", "Will open in a new window instead of the current one", 0));
#endif // QT_NO_TOOLTIP
        checkBox->setText(QApplication::translate("DialogOpenConnection", "Open connection in a new window", 0));
        pushButton->setText(QApplication::translate("DialogOpenConnection", "Bookmark it", 0));
        label_4->setText(QApplication::translate("DialogOpenConnection", "Bookmarks:", 0));
    } // retranslateUi

};

namespace Ui {
    class DialogOpenConnection: public Ui_DialogOpenConnection {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGOPENCONNECTION_H
