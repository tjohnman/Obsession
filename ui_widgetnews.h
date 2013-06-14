/********************************************************************************
** Form generated from reading UI file 'widgetnews.ui'
**
** Created by: Qt User Interface Compiler version 5.0.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGETNEWS_H
#define UI_WIDGETNEWS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>

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
            WidgetNews->setObjectName(QStringLiteral("WidgetNews"));
        WidgetNews->resize(594, 303);
        gridLayout = new QGridLayout(WidgetNews);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        treeWidget = new QTreeWidget(WidgetNews);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("1"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QStringLiteral("treeWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(treeWidget->sizePolicy().hasHeightForWidth());
        treeWidget->setSizePolicy(sizePolicy);
        treeWidget->header()->setVisible(false);

        gridLayout->addWidget(treeWidget, 0, 0, 3, 1);

        label = new QLabel(WidgetNews);
        label->setObjectName(QStringLiteral("label"));
        label->setMaximumSize(QSize(35, 16777215));

        gridLayout->addWidget(label, 0, 1, 1, 1);

        posterLabel = new QLabel(WidgetNews);
        posterLabel->setObjectName(QStringLiteral("posterLabel"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        posterLabel->setFont(font);

        gridLayout->addWidget(posterLabel, 0, 2, 1, 1);

        label_2 = new QLabel(WidgetNews);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setMaximumSize(QSize(35, 16777215));

        gridLayout->addWidget(label_2, 1, 1, 1, 1);

        dateLabel = new QLabel(WidgetNews);
        dateLabel->setObjectName(QStringLiteral("dateLabel"));

        gridLayout->addWidget(dateLabel, 1, 2, 1, 1);

        plainTextEdit = new QPlainTextEdit(WidgetNews);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));
        QFont font1;
        font1.setFamily(QStringLiteral("Courier"));
        font1.setPointSize(9);
        plainTextEdit->setFont(font1);
        plainTextEdit->setReadOnly(true);

        gridLayout->addWidget(plainTextEdit, 2, 1, 1, 2);


        retranslateUi(WidgetNews);

        QMetaObject::connectSlotsByName(WidgetNews);
    } // setupUi

    void retranslateUi(QWidget *WidgetNews)
    {
        WidgetNews->setWindowTitle(QApplication::translate("WidgetNews", "Form", 0));
        label->setText(QApplication::translate("WidgetNews", "Poster:", 0));
        posterLabel->setText(QString());
        label_2->setText(QApplication::translate("WidgetNews", "Date:", 0));
        dateLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class WidgetNews: public Ui_WidgetNews {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGETNEWS_H
