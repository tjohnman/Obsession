/********************************************************************************
** Form generated from reading UI file 'dialogfilebrowser.ui'
**
** Created by: Qt User Interface Compiler version 5.0.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGFILEBROWSER_H
#define UI_DIALOGFILEBROWSER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTreeWidget>

QT_BEGIN_NAMESPACE

class Ui_DialogFileBrowser
{
public:
    QGridLayout *gridLayout;
    QPushButton *buttonUp;
    QPushButton *buttonDownload;
    QPushButton *buttonUpload;
    QFrame *line;
    QPushButton *buttonDelete;
    QSpacerItem *horizontalSpacer_3;
    QLabel *label;
    QTreeWidget *treeWidget;
    QFrame *line_2;

    void setupUi(QDialog *DialogFileBrowser)
    {
        if (DialogFileBrowser->objectName().isEmpty())
            DialogFileBrowser->setObjectName(QStringLiteral("DialogFileBrowser"));
        DialogFileBrowser->resize(552, 406);
        DialogFileBrowser->setMinimumSize(QSize(449, 354));
        DialogFileBrowser->setModal(false);
        gridLayout = new QGridLayout(DialogFileBrowser);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        buttonUp = new QPushButton(DialogFileBrowser);
        buttonUp->setObjectName(QStringLiteral("buttonUp"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/files/interfaceIcons/filesUp.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonUp->setIcon(icon);
        buttonUp->setIconSize(QSize(16, 16));
        buttonUp->setFlat(true);

        gridLayout->addWidget(buttonUp, 0, 0, 1, 1);

        buttonDownload = new QPushButton(DialogFileBrowser);
        buttonDownload->setObjectName(QStringLiteral("buttonDownload"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/files/interfaceIcons/filesDownload.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonDownload->setIcon(icon1);
        buttonDownload->setIconSize(QSize(16, 16));
        buttonDownload->setFlat(true);

        gridLayout->addWidget(buttonDownload, 0, 2, 1, 1);

        buttonUpload = new QPushButton(DialogFileBrowser);
        buttonUpload->setObjectName(QStringLiteral("buttonUpload"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/files/interfaceIcons/filesUpload.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonUpload->setIcon(icon2);
        buttonUpload->setIconSize(QSize(16, 16));
        buttonUpload->setFlat(true);

        gridLayout->addWidget(buttonUpload, 0, 3, 1, 1);

        line = new QFrame(DialogFileBrowser);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line, 0, 4, 1, 1);

        buttonDelete = new QPushButton(DialogFileBrowser);
        buttonDelete->setObjectName(QStringLiteral("buttonDelete"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/files/interfaceIcons/filesDelete.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonDelete->setIcon(icon3);
        buttonDelete->setIconSize(QSize(16, 16));
        buttonDelete->setFlat(true);

        gridLayout->addWidget(buttonDelete, 0, 5, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(100, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 0, 6, 1, 1);

        label = new QLabel(DialogFileBrowser);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setMinimumSize(QSize(100, 0));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label, 0, 7, 1, 1);

        treeWidget = new QTreeWidget(DialogFileBrowser);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setTextAlignment(1, Qt::AlignRight|Qt::AlignVCenter);
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QStringLiteral("treeWidget"));
        treeWidget->setRootIsDecorated(true);
        treeWidget->setColumnCount(2);

        gridLayout->addWidget(treeWidget, 1, 0, 1, 8);

        line_2 = new QFrame(DialogFileBrowser);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line_2, 0, 1, 1, 1);


        retranslateUi(DialogFileBrowser);

        QMetaObject::connectSlotsByName(DialogFileBrowser);
    } // setupUi

    void retranslateUi(QDialog *DialogFileBrowser)
    {
        DialogFileBrowser->setWindowTitle(QApplication::translate("DialogFileBrowser", "File Browser", 0));
        buttonUp->setText(QString());
        buttonDownload->setText(QString());
        buttonUpload->setText(QString());
        buttonDelete->setText(QString());
        label->setText(QApplication::translate("DialogFileBrowser", "loading...", 0));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(1, QApplication::translate("DialogFileBrowser", "Size", 0));
        ___qtreewidgetitem->setText(0, QApplication::translate("DialogFileBrowser", "Name", 0));
    } // retranslateUi

};

namespace Ui {
    class DialogFileBrowser: public Ui_DialogFileBrowser {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGFILEBROWSER_H
