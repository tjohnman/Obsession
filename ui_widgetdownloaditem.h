/********************************************************************************
** Form generated from reading UI file 'widgetdownloaditem.ui'
**
** Created: Thu 21. Oct 04:13:21 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGETDOWNLOADITEM_H
#define UI_WIDGETDOWNLOADITEM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WidgetDownloadItem
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QProgressBar *progressBar;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *goButton;
    QPushButton *queueButton;
    QPushButton *stopButton;
    QFrame *line;

    void setupUi(QWidget *WidgetDownloadItem)
    {
        if (WidgetDownloadItem->objectName().isEmpty())
            WidgetDownloadItem->setObjectName(QString::fromUtf8("WidgetDownloadItem"));
        WidgetDownloadItem->resize(509, 94);
        gridLayout = new QGridLayout(WidgetDownloadItem);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(WidgetDownloadItem);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        QFont font;
        font.setPointSize(10);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        progressBar = new QProgressBar(WidgetDownloadItem);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(15);
        sizePolicy1.setHeightForWidth(progressBar->sizePolicy().hasHeightForWidth());
        progressBar->setSizePolicy(sizePolicy1);
        progressBar->setMinimumSize(QSize(0, 15));
        progressBar->setMaximumSize(QSize(16777215, 15));
        progressBar->setMaximum(100);
        progressBar->setValue(0);
        progressBar->setTextVisible(false);

        gridLayout->addWidget(progressBar, 1, 0, 1, 5);

        label_2 = new QLabel(WidgetDownloadItem);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(250, 13, QSizePolicy::Maximum, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 2, 1, 1, 1);

        goButton = new QPushButton(WidgetDownloadItem);
        goButton->setObjectName(QString::fromUtf8("goButton"));
        goButton->setEnabled(false);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/general/interfaceIcons/generalGo.png"), QSize(), QIcon::Normal, QIcon::Off);
        goButton->setIcon(icon);
        goButton->setFlat(true);

        gridLayout->addWidget(goButton, 2, 2, 1, 1);

        queueButton = new QPushButton(WidgetDownloadItem);
        queueButton->setObjectName(QString::fromUtf8("queueButton"));
        queueButton->setEnabled(false);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/general/interfaceIcons/generalQueue.png"), QSize(), QIcon::Normal, QIcon::Off);
        queueButton->setIcon(icon1);
        queueButton->setFlat(true);

        gridLayout->addWidget(queueButton, 2, 3, 1, 1);

        stopButton = new QPushButton(WidgetDownloadItem);
        stopButton->setObjectName(QString::fromUtf8("stopButton"));
        stopButton->setEnabled(false);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/general/interfaceIcons/generalStop.png"), QSize(), QIcon::Normal, QIcon::Off);
        stopButton->setIcon(icon2);
        stopButton->setFlat(true);

        gridLayout->addWidget(stopButton, 2, 4, 1, 1);

        line = new QFrame(WidgetDownloadItem);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line, 3, 0, 1, 5);


        retranslateUi(WidgetDownloadItem);

        QMetaObject::connectSlotsByName(WidgetDownloadItem);
    } // setupUi

    void retranslateUi(QWidget *WidgetDownloadItem)
    {
        WidgetDownloadItem->setWindowTitle(QApplication::translate("WidgetDownloadItem", "Form", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("WidgetDownloadItem", "Waiting for name...", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("WidgetDownloadItem", "0 minutes remaining - 0 of 0 bytes (0 bytes/s)", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        goButton->setToolTip(QApplication::translate("WidgetDownloadItem", "Start", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        goButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        queueButton->setToolTip(QApplication::translate("WidgetDownloadItem", "Queue", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        queueButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        stopButton->setToolTip(QApplication::translate("WidgetDownloadItem", "Stop", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        stopButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class WidgetDownloadItem: public Ui_WidgetDownloadItem {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGETDOWNLOADITEM_H
