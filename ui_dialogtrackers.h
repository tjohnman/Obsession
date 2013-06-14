/********************************************************************************
** Form generated from reading UI file 'dialogtrackers.ui'
**
** Created: Thu 21. Oct 04:13:21 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGTRACKERS_H
#define UI_DIALOGTRACKERS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTreeWidget>

QT_BEGIN_NAMESPACE

class Ui_DialogTrackers
{
public:
    QGridLayout *gridLayout;
    QComboBox *comboBox;
    QLabel *label;
    QTreeWidget *treeWidget;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_3;
    QPushButton *buttonRefresh;

    void setupUi(QDialog *DialogTrackers)
    {
        if (DialogTrackers->objectName().isEmpty())
            DialogTrackers->setObjectName(QString::fromUtf8("DialogTrackers"));
        DialogTrackers->setWindowModality(Qt::ApplicationModal);
        DialogTrackers->resize(691, 418);
        DialogTrackers->setMinimumSize(QSize(640, 337));
        gridLayout = new QGridLayout(DialogTrackers);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        comboBox = new QComboBox(DialogTrackers);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(comboBox->sizePolicy().hasHeightForWidth());
        comboBox->setSizePolicy(sizePolicy);

        gridLayout->addWidget(comboBox, 0, 0, 1, 3);

        label = new QLabel(DialogTrackers);
        label->setObjectName(QString::fromUtf8("label"));
        label->setTextFormat(Qt::PlainText);
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label, 0, 6, 1, 1);

        treeWidget = new QTreeWidget(DialogTrackers);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setTextAlignment(1, Qt::AlignHCenter|Qt::AlignVCenter|Qt::AlignCenter);
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
        QFont font;
        font.setPointSize(8);
        treeWidget->setFont(font);
        treeWidget->setIndentation(2);
        treeWidget->setItemsExpandable(false);
        treeWidget->setSortingEnabled(false);
        treeWidget->setAnimated(true);
        treeWidget->setExpandsOnDoubleClick(false);
        treeWidget->setColumnCount(3);

        gridLayout->addWidget(treeWidget, 1, 0, 1, 7);

        pushButton = new QPushButton(DialogTrackers);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setMinimumSize(QSize(100, 0));

        gridLayout->addWidget(pushButton, 2, 0, 1, 1);

        pushButton_2 = new QPushButton(DialogTrackers);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setEnabled(true);

        gridLayout->addWidget(pushButton_2, 2, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(465, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 2, 2, 1, 4);

        pushButton_3 = new QPushButton(DialogTrackers);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));

        gridLayout->addWidget(pushButton_3, 2, 6, 1, 1);

        buttonRefresh = new QPushButton(DialogTrackers);
        buttonRefresh->setObjectName(QString::fromUtf8("buttonRefresh"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(buttonRefresh->sizePolicy().hasHeightForWidth());
        buttonRefresh->setSizePolicy(sizePolicy1);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/general/interfaceIcons/generalRefresh.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonRefresh->setIcon(icon);
        buttonRefresh->setIconSize(QSize(16, 16));

        gridLayout->addWidget(buttonRefresh, 0, 3, 1, 1);


        retranslateUi(DialogTrackers);

        QMetaObject::connectSlotsByName(DialogTrackers);
    } // setupUi

    void retranslateUi(QDialog *DialogTrackers)
    {
        DialogTrackers->setWindowTitle(QApplication::translate("DialogTrackers", "Trackers", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("DialogTrackers", "0 servers", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(2, QApplication::translate("DialogTrackers", "Description", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(1, QApplication::translate("DialogTrackers", "Users", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(0, QApplication::translate("DialogTrackers", "Name", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("DialogTrackers", "Add tracker...", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("DialogTrackers", "Remove", 0, QApplication::UnicodeUTF8));
        pushButton_3->setText(QApplication::translate("DialogTrackers", "Connect...", 0, QApplication::UnicodeUTF8));
        buttonRefresh->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class DialogTrackers: public Ui_DialogTrackers {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGTRACKERS_H
