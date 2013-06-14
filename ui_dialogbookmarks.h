/********************************************************************************
** Form generated from reading UI file 'dialogbookmarks.ui'
**
** Created: Thu 21. Oct 04:13:21 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGBOOKMARKS_H
#define UI_DIALOGBOOKMARKS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_DialogBookmarks
{
public:
    QGridLayout *gridLayout;
    QListWidget *listWidget;
    QLabel *label;
    QLineEdit *lineEdit;
    QLabel *label_2;
    QLineEdit *lineEdit_2;
    QLabel *label_3;
    QLineEdit *lineEdit_3;
    QLabel *label_4;
    QLineEdit *lineEdit_4;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QSpacerItem *horizontalSpacer;
    QDialogButtonBox *buttonBox;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *DialogBookmarks)
    {
        if (DialogBookmarks->objectName().isEmpty())
            DialogBookmarks->setObjectName(QString::fromUtf8("DialogBookmarks"));
        DialogBookmarks->setWindowModality(Qt::ApplicationModal);
        DialogBookmarks->resize(355, 328);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(DialogBookmarks->sizePolicy().hasHeightForWidth());
        DialogBookmarks->setSizePolicy(sizePolicy);
        DialogBookmarks->setModal(true);
        gridLayout = new QGridLayout(DialogBookmarks);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        listWidget = new QListWidget(DialogBookmarks);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setMinimumSize(QSize(170, 0));
        listWidget->setMaximumSize(QSize(170, 16777215));

        gridLayout->addWidget(listWidget, 0, 0, 9, 3);

        label = new QLabel(DialogBookmarks);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 3, 1, 1);

        lineEdit = new QLineEdit(DialogBookmarks);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        gridLayout->addWidget(lineEdit, 1, 3, 1, 1);

        label_2 = new QLabel(DialogBookmarks);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 2, 3, 1, 1);

        lineEdit_2 = new QLineEdit(DialogBookmarks);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));

        gridLayout->addWidget(lineEdit_2, 3, 3, 1, 1);

        label_3 = new QLabel(DialogBookmarks);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 4, 3, 1, 1);

        lineEdit_3 = new QLineEdit(DialogBookmarks);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));

        gridLayout->addWidget(lineEdit_3, 5, 3, 1, 1);

        label_4 = new QLabel(DialogBookmarks);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 6, 3, 1, 1);

        lineEdit_4 = new QLineEdit(DialogBookmarks);
        lineEdit_4->setObjectName(QString::fromUtf8("lineEdit_4"));
        lineEdit_4->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(lineEdit_4, 7, 3, 1, 1);

        pushButton = new QPushButton(DialogBookmarks);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(24);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy1);
        pushButton->setMinimumSize(QSize(24, 24));
        pushButton->setMaximumSize(QSize(24, 24));

        gridLayout->addWidget(pushButton, 9, 0, 1, 1);

        pushButton_2 = new QPushButton(DialogBookmarks);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        sizePolicy.setHeightForWidth(pushButton_2->sizePolicy().hasHeightForWidth());
        pushButton_2->setSizePolicy(sizePolicy);
        pushButton_2->setMinimumSize(QSize(24, 24));
        pushButton_2->setMaximumSize(QSize(24, 24));

        gridLayout->addWidget(pushButton_2, 9, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(110, 21, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 9, 2, 1, 1);

        buttonBox = new QDialogButtonBox(DialogBookmarks);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 9, 3, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 8, 3, 1, 1);


        retranslateUi(DialogBookmarks);
        QObject::connect(buttonBox, SIGNAL(accepted()), DialogBookmarks, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DialogBookmarks, SLOT(reject()));

        QMetaObject::connectSlotsByName(DialogBookmarks);
    } // setupUi

    void retranslateUi(QDialog *DialogBookmarks)
    {
        DialogBookmarks->setWindowTitle(QApplication::translate("DialogBookmarks", "Bookmarks", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("DialogBookmarks", "Name:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("DialogBookmarks", "Address:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("DialogBookmarks", "Login:", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("DialogBookmarks", "Password:", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("DialogBookmarks", "+", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("DialogBookmarks", "-", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DialogBookmarks: public Ui_DialogBookmarks {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGBOOKMARKS_H
