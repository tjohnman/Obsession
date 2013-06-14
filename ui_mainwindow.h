/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.0.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen_Connection;
    QAction *actionQuit;
    QAction *actionServer_Info;
    QAction *actionDisconnect;
    QAction *actionAbout_Contra;
    QAction *actionPreferences;
    QAction *actionServer_Agreement;
    QAction *actionTrackers;
    QAction *actionNew_Window;
    QAction *actionFiles;
    QAction *actionIcons;
    QAction *actionLinear_News;
    QAction *actionDownloads;
    QAction *actionClose_Tab;
    QAction *actionThreaded_News;
    QAction *actionDebug_console;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_2;
    QTabWidget *tabWidget;
    QMenuBar *menuBar;
    QMenu *menuConnection;
    QMenu *menuHelp;
    QMenu *menuOptions;
    QMenu *menuServer;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(900, 600);
        MainWindow->setMinimumSize(QSize(900, 600));
        actionOpen_Connection = new QAction(MainWindow);
        actionOpen_Connection->setObjectName(QStringLiteral("actionOpen_Connection"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/main/interfaceIcons/mainConnect.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen_Connection->setIcon(icon);
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QStringLiteral("actionQuit"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/main/interfaceIcons/mainClose.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionQuit->setIcon(icon1);
        actionServer_Info = new QAction(MainWindow);
        actionServer_Info->setObjectName(QStringLiteral("actionServer_Info"));
        actionDisconnect = new QAction(MainWindow);
        actionDisconnect->setObjectName(QStringLiteral("actionDisconnect"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/main/interfaceIcons/mainDisconnect.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDisconnect->setIcon(icon2);
        actionAbout_Contra = new QAction(MainWindow);
        actionAbout_Contra->setObjectName(QStringLiteral("actionAbout_Contra"));
        actionPreferences = new QAction(MainWindow);
        actionPreferences->setObjectName(QStringLiteral("actionPreferences"));
        actionServer_Agreement = new QAction(MainWindow);
        actionServer_Agreement->setObjectName(QStringLiteral("actionServer_Agreement"));
        actionTrackers = new QAction(MainWindow);
        actionTrackers->setObjectName(QStringLiteral("actionTrackers"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/main/interfaceIcons/mainTrackers.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionTrackers->setIcon(icon3);
        actionNew_Window = new QAction(MainWindow);
        actionNew_Window->setObjectName(QStringLiteral("actionNew_Window"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/main/interfaceIcons/mainNew.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNew_Window->setIcon(icon4);
        actionFiles = new QAction(MainWindow);
        actionFiles->setObjectName(QStringLiteral("actionFiles"));
        actionIcons = new QAction(MainWindow);
        actionIcons->setObjectName(QStringLiteral("actionIcons"));
        actionLinear_News = new QAction(MainWindow);
        actionLinear_News->setObjectName(QStringLiteral("actionLinear_News"));
        actionDownloads = new QAction(MainWindow);
        actionDownloads->setObjectName(QStringLiteral("actionDownloads"));
        actionClose_Tab = new QAction(MainWindow);
        actionClose_Tab->setObjectName(QStringLiteral("actionClose_Tab"));
        actionClose_Tab->setEnabled(true);
        actionClose_Tab->setVisible(true);
        actionThreaded_News = new QAction(MainWindow);
        actionThreaded_News->setObjectName(QStringLiteral("actionThreaded_News"));
        actionDebug_console = new QAction(MainWindow);
        actionDebug_console->setObjectName(QStringLiteral("actionDebug_console"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        centralWidget->setAutoFillBackground(false);
        horizontalLayout_2 = new QHBoxLayout(centralWidget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setTabShape(QTabWidget::Rounded);
        tabWidget->setTabsClosable(true);
        tabWidget->setMovable(false);

        horizontalLayout_2->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 900, 21));
        menuConnection = new QMenu(menuBar);
        menuConnection->setObjectName(QStringLiteral("menuConnection"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        menuOptions = new QMenu(menuBar);
        menuOptions->setObjectName(QStringLiteral("menuOptions"));
        menuServer = new QMenu(menuBar);
        menuServer->setObjectName(QStringLiteral("menuServer"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        mainToolBar->setMovable(false);
        mainToolBar->setFloatable(false);
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuConnection->menuAction());
        menuBar->addAction(menuServer->menuAction());
        menuBar->addAction(menuOptions->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuConnection->addAction(actionNew_Window);
        menuConnection->addAction(actionOpen_Connection);
        menuConnection->addAction(actionDisconnect);
        menuConnection->addSeparator();
        menuConnection->addAction(actionTrackers);
        menuConnection->addSeparator();
        menuConnection->addAction(actionClose_Tab);
        menuConnection->addAction(actionQuit);
        menuHelp->addAction(actionAbout_Contra);
        menuOptions->addAction(actionPreferences);
        menuOptions->addAction(actionIcons);
        menuServer->addAction(actionServer_Agreement);
        menuServer->addAction(actionFiles);
        menuServer->addAction(actionLinear_News);
        menuServer->addAction(actionThreaded_News);
        menuServer->addSeparator();
        menuServer->addAction(actionDownloads);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Obsession Hotline Client Alpha", 0));
#ifndef QT_NO_STATUSTIP
        MainWindow->setStatusTip(QString());
#endif // QT_NO_STATUSTIP
        actionOpen_Connection->setText(QApplication::translate("MainWindow", "Open Connection...", 0));
#ifndef QT_NO_TOOLTIP
        actionOpen_Connection->setToolTip(QApplication::translate("MainWindow", "Open a new connection", 0));
#endif // QT_NO_TOOLTIP
        actionOpen_Connection->setShortcut(QApplication::translate("MainWindow", "Ctrl+K", 0));
        actionQuit->setText(QApplication::translate("MainWindow", "Quit", 0));
        actionQuit->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", 0));
        actionServer_Info->setText(QApplication::translate("MainWindow", "Server Info", 0));
        actionDisconnect->setText(QApplication::translate("MainWindow", "Disconnect...", 0));
        actionAbout_Contra->setText(QApplication::translate("MainWindow", "About Obsession...", 0));
        actionPreferences->setText(QApplication::translate("MainWindow", "Preferences...", 0));
        actionServer_Agreement->setText(QApplication::translate("MainWindow", "Agreement", 0));
        actionTrackers->setText(QApplication::translate("MainWindow", "Trackers...", 0));
        actionTrackers->setShortcut(QApplication::translate("MainWindow", "Ctrl+T", 0));
        actionNew_Window->setText(QApplication::translate("MainWindow", "New Window...", 0));
        actionNew_Window->setShortcut(QApplication::translate("MainWindow", "Ctrl+N", 0));
        actionFiles->setText(QApplication::translate("MainWindow", "Files", 0));
        actionFiles->setShortcut(QApplication::translate("MainWindow", "Ctrl+F", 0));
        actionIcons->setText(QApplication::translate("MainWindow", "Icons...", 0));
        actionLinear_News->setText(QApplication::translate("MainWindow", "Message Board", 0));
        actionDownloads->setText(QApplication::translate("MainWindow", "Downloads", 0));
        actionClose_Tab->setText(QApplication::translate("MainWindow", "Close Tab", 0));
        actionClose_Tab->setShortcut(QApplication::translate("MainWindow", "Ctrl+W", 0));
        actionThreaded_News->setText(QApplication::translate("MainWindow", "News", 0));
        actionThreaded_News->setShortcut(QApplication::translate("MainWindow", "Ctrl+E", 0));
        actionDebug_console->setText(QApplication::translate("MainWindow", "Debug console", 0));
        menuConnection->setTitle(QApplication::translate("MainWindow", "Connection", 0));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", 0));
        menuOptions->setTitle(QApplication::translate("MainWindow", "Options", 0));
        menuServer->setTitle(QApplication::translate("MainWindow", "Server", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
