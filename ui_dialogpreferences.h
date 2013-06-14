/********************************************************************************
** Form generated from reading UI file 'dialogpreferences.ui'
**
** Created: Thu 21. Oct 04:52:01 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGPREFERENCES_H
#define UI_DIALOGPREFERENCES_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_DialogPreferences
{
public:
    QLabel *label;
    QLineEdit *lineEdit;
    QDialogButtonBox *buttonBox;
    QCheckBox *checkBox;
    QLabel *label_2;
    QPushButton *pushButton;
    QLabel *label_3;
    QLineEdit *dlQueueEdit;
    QLabel *label_4;
    QLineEdit *ulQueueEdit;
    QLabel *label_5;
    QLineEdit *maxDlEdit;
    QLineEdit *maxUlEdit;
    QLabel *label_6;
    QPushButton *fontButton;
    QLineEdit *sampleEdit;
    QCheckBox *soundCheckBox;
    QPushButton *pushButton_2;
    QCheckBox *debugCheckBox;

    void setupUi(QDialog *DialogPreferences)
    {
        if (DialogPreferences->objectName().isEmpty())
            DialogPreferences->setObjectName(QString::fromUtf8("DialogPreferences"));
        DialogPreferences->setWindowModality(Qt::ApplicationModal);
        DialogPreferences->resize(300, 444);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(DialogPreferences->sizePolicy().hasHeightForWidth());
        DialogPreferences->setSizePolicy(sizePolicy);
        DialogPreferences->setMinimumSize(QSize(0, 0));
        DialogPreferences->setMaximumSize(QSize(546584, 654686));
        DialogPreferences->setModal(true);
        label = new QLabel(DialogPreferences);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(9, 28, 49, 16));
        lineEdit = new QLineEdit(DialogPreferences);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(64, 28, 221, 20));
        buttonBox = new QDialogButtonBox(DialogPreferences);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(10, 410, 156, 23));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        checkBox = new QCheckBox(DialogPreferences);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setGeometry(QRect(10, 70, 195, 17));
        label_2 = new QLabel(DialogPreferences);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 90, 281, 31));
        QFont font;
        font.setPointSize(7);
        label_2->setFont(font);
        label_2->setWordWrap(true);
        pushButton = new QPushButton(DialogPreferences);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(10, 220, 121, 23));
        label_3 = new QLabel(DialogPreferences);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 290, 91, 16));
        dlQueueEdit = new QLineEdit(DialogPreferences);
        dlQueueEdit->setObjectName(QString::fromUtf8("dlQueueEdit"));
        dlQueueEdit->setGeometry(QRect(10, 310, 71, 20));
        label_4 = new QLabel(DialogPreferences);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(140, 290, 91, 16));
        ulQueueEdit = new QLineEdit(DialogPreferences);
        ulQueueEdit->setObjectName(QString::fromUtf8("ulQueueEdit"));
        ulQueueEdit->setEnabled(false);
        ulQueueEdit->setGeometry(QRect(140, 310, 71, 20));
        label_5 = new QLabel(DialogPreferences);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(10, 340, 91, 16));
        maxDlEdit = new QLineEdit(DialogPreferences);
        maxDlEdit->setObjectName(QString::fromUtf8("maxDlEdit"));
        maxDlEdit->setEnabled(false);
        maxDlEdit->setGeometry(QRect(10, 360, 71, 20));
        maxUlEdit = new QLineEdit(DialogPreferences);
        maxUlEdit->setObjectName(QString::fromUtf8("maxUlEdit"));
        maxUlEdit->setEnabled(false);
        maxUlEdit->setGeometry(QRect(140, 360, 71, 20));
        label_6 = new QLabel(DialogPreferences);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(140, 340, 91, 16));
        fontButton = new QPushButton(DialogPreferences);
        fontButton->setObjectName(QString::fromUtf8("fontButton"));
        fontButton->setGeometry(QRect(10, 250, 121, 23));
        sampleEdit = new QLineEdit(DialogPreferences);
        sampleEdit->setObjectName(QString::fromUtf8("sampleEdit"));
        sampleEdit->setGeometry(QRect(150, 250, 131, 20));
        sampleEdit->setAlignment(Qt::AlignCenter);
        soundCheckBox = new QCheckBox(DialogPreferences);
        soundCheckBox->setObjectName(QString::fromUtf8("soundCheckBox"));
        soundCheckBox->setGeometry(QRect(10, 130, 101, 17));
        pushButton_2 = new QPushButton(DialogPreferences);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setEnabled(false);
        pushButton_2->setGeometry(QRect(30, 150, 75, 23));
        debugCheckBox = new QCheckBox(DialogPreferences);
        debugCheckBox->setObjectName(QString::fromUtf8("debugCheckBox"));
        debugCheckBox->setGeometry(QRect(10, 190, 131, 17));

        retranslateUi(DialogPreferences);
        QObject::connect(buttonBox, SIGNAL(accepted()), DialogPreferences, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DialogPreferences, SLOT(reject()));

        QMetaObject::connectSlotsByName(DialogPreferences);
    } // setupUi

    void retranslateUi(QDialog *DialogPreferences)
    {
        DialogPreferences->setWindowTitle(QApplication::translate("DialogPreferences", "Preferences", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("DialogPreferences", "Nickname:", 0, QApplication::UnicodeUTF8));
        checkBox->setText(QApplication::translate("DialogPreferences", "Enable Japanese support (Shift-JIS)", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("DialogPreferences", "This may cause accents and other diacritical marks to be rendered incorrectly in other languages such as French or Spanish.", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("DialogPreferences", "Edit bookmarks...", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("DialogPreferences", "Download queue:", 0, QApplication::UnicodeUTF8));
        dlQueueEdit->setInputMask(QApplication::translate("DialogPreferences", "0000; ", 0, QApplication::UnicodeUTF8));
        dlQueueEdit->setText(QApplication::translate("DialogPreferences", "0", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("DialogPreferences", "Upload queue:", 0, QApplication::UnicodeUTF8));
        ulQueueEdit->setInputMask(QApplication::translate("DialogPreferences", "0000; ", 0, QApplication::UnicodeUTF8));
        ulQueueEdit->setText(QApplication::translate("DialogPreferences", "0", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("DialogPreferences", "Max dl speed:", 0, QApplication::UnicodeUTF8));
        maxDlEdit->setInputMask(QApplication::translate("DialogPreferences", "0000; ", 0, QApplication::UnicodeUTF8));
        maxDlEdit->setText(QApplication::translate("DialogPreferences", "0", 0, QApplication::UnicodeUTF8));
        maxUlEdit->setInputMask(QApplication::translate("DialogPreferences", "0000; ", 0, QApplication::UnicodeUTF8));
        maxUlEdit->setText(QApplication::translate("DialogPreferences", "0", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("DialogPreferences", "Max ul speed:", 0, QApplication::UnicodeUTF8));
        fontButton->setText(QApplication::translate("DialogPreferences", "Change chat font...", 0, QApplication::UnicodeUTF8));
        sampleEdit->setText(QApplication::translate("DialogPreferences", "Sample text", 0, QApplication::UnicodeUTF8));
        soundCheckBox->setText(QApplication::translate("DialogPreferences", "Enable sounds", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("DialogPreferences", "Edit...", 0, QApplication::UnicodeUTF8));
        debugCheckBox->setText(QApplication::translate("DialogPreferences", "Enable debug logging", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DialogPreferences: public Ui_DialogPreferences {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGPREFERENCES_H
