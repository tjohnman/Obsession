/********************************************************************************
** Form generated from reading UI file 'dialogfilebrowser.ui'
**
** Created: Thu 21. Oct 04:13:21 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGFILEBROWSER_H
#define UI_DIALOGFILEBROWSER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTreeWidget>

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
            DialogFileBrowser->setObjectName(QString::fromUtf8("DialogFileBrowser"));
        DialogFileBrowser->resize(552, 406);
        DialogFileBrowser->setMinimumSize(QSize(449, 354));
        DialogFileBrowser->setModal(false);
        gridLayout = new QGridLayout(DialogFileBrowser);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        buttonUp = new QPushButton(DialogFileBrowser);
        buttonUp->setObjectName(QString::fromUtf8("buttonUp"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/files/interfaceIcons/filesUp.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonUp->setIcon(icon);
        buttonUp->setIconSize(QSize(16, 16));
        buttonUp->setFlat(true);

        gridLayout->addWidget(buttonUp, 0, 0, 1, 1);

        buttonDownload = new QPushButton(DialogFileBrowser);
        buttonDownload->setObjectName(QString::fromUtf8("buttonDownload"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/files/interfaceIcons/filesDownload.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonDownload->setIcon(icon1);
        buttonDownload->setIconSize(QSize(16, 16));
        buttonDownload->setFlat(true);

        gridLayout->addWidget(buttonDownload, 0, 2, 1, 1);

        buttonUpload = new QPushButton(DialogFileBrowser);
        buttonUpload->setObjectName(QString::fromUtf8("buttonUpload"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/files/interfaceIcons/filesUpload.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonUpload->setIcon(icon2);
        buttonUpload->setIconSize(QSize(16, 16));
        buttonUpload->setFlat(true);

        gridLayout->addWidget(buttonUpload, 0, 3, 1, 1);

        line = new QFrame(DialogFileBrowser);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line, 0, 4, 1, 1);

        buttonDelete = new QPushButton(DialogFileBrowser);
        buttonDelete->setObjectName(QString::fromUtf8("buttonDelete"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/files/interfaceIcons/filesDelete.png"), QSize(), QIcon::Normal, QIcon::Off);
        buttonDelete->setIcon(icon3);
        buttonDelete->setIconSize(QSize(16, 16));
        buttonDelete->setFlat(true);

        gridLayout->addWidget(buttonDelete, 0, 5, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(100, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 0, 6, 1, 1);

        label = new QLabel(DialogFileBrowser);
        label->setObjectName(QString::fromUtf8("label"));
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
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
        treeWidget->setRootIsDecorated(true);
        treeWidget->setColumnCount(2);

        gridLayout->addWidget(treeWidget, 1, 0, 1, 8);

        line_2 = new QFrame(DialogFileBrowser);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line_2, 0, 1, 1, 1);


        retranslateUi(DialogFileBrowser);

        QMetaObject::connectSlotsByName(DialogFileBrowser);
    } // setupUi

    void retranslateUi(QDialog *DialogFileBrowser)
    {
        DialogFileBrowser->setWindowTitle(QApplication::translate("DialogFileBrowser", "File Browser", 0, QApplication::UnicodeUTF8));
        buttonUp->setText(QString());
        buttonDownload->setText(QString());
        buttonUpload->setText(QString());
        buttonDelete->setText(QString());
        label->setText(QApplication::translate("DialogFileBrowser", "loading...", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(1, QApplication::translate("DialogFileBrowser", "Size", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(0, QApplication::translate("DialogFileBrowser", "Name", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DialogFileBrowser: public Ui_DialogFileBrowser {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGFILEBROWSER_H
