/********************************************************************************
** Form generated from reading UI file 'widgetnews.ui'
**
** Created: Thu 21. Oct 04:13:21 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGETNEWS_H
#define UI_WIDGETNEWS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QTreeWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WidgetNews
{
public:
    QGridLayout *gridLayout;
    QTreeWidget *treeWidget;
    QLabel *label;
    QLabel *posterLabel;
    QLabel *label_2;
    QLabel *dateLabel;
    QPlainTextEdit *plainTextEdit;

    void setupUi(QWidget *WidgetNews)
    {
        if (WidgetNews->objectName().isEmpty())
            WidgetNews->setObjectName(QString::fromUtf8("WidgetNews"));
        WidgetNews->resize(594, 303);
        gridLayout = new QGridLayout(WidgetNews);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        treeWidget = new QTreeWidget(WidgetNews);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(treeWidget->sizePolicy().hasHeightForWidth());
        treeWidget->setSizePolicy(sizePolicy);
        treeWidget->header()->setVisible(false);

        gridLayout->addWidget(treeWidget, 0, 0, 3, 1);

        label = new QLabel(WidgetNews);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMaximumSize(QSize(35, 16777215));

        gridLayout->addWidget(label, 0, 1, 1, 1);

        posterLabel = new QLabel(WidgetNews);
        posterLabel->setObjectName(QString::fromUtf8("posterLabel"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        posterLabel->setFont(font);

        gridLayout->addWidget(posterLabel, 0, 2, 1, 1);

        label_2 = new QLabel(WidgetNews);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMaximumSize(QSize(35, 16777215));

        gridLayout->addWidget(label_2, 1, 1, 1, 1);

        dateLabel = new QLabel(WidgetNews);
        dateLabel->setObjectName(QString::fromUtf8("dateLabel"));

        gridLayout->addWidget(dateLabel, 1, 2, 1, 1);

        plainTextEdit = new QPlainTextEdit(WidgetNews);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Courier"));
        font1.setPointSize(9);
        plainTextEdit->setFont(font1);
        plainTextEdit->setReadOnly(true);

        gridLayout->addWidget(plainTextEdit, 2, 1, 1, 2);


        retranslateUi(WidgetNews);

        QMetaObject::connectSlotsByName(WidgetNews);
    } // setupUi

    void retranslateUi(QWidget *WidgetNews)
    {
        WidgetNews->setWindowTitle(QApplication::translate("WidgetNews", "Form", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("WidgetNews", "Poster:", 0, QApplication::UnicodeUTF8));
        posterLabel->setText(QString());
        label_2->setText(QApplication::translate("WidgetNews", "Date:", 0, QApplication::UnicodeUTF8));
        dateLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class WidgetNews: public Ui_WidgetNews {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGETNEWS_H
