#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "ConnectionController.h"
#include "dialogopenconnection.h"
#include <QtWidgets/QLabel>
#include <QTextCursor>
#include "dialogagreement.h"
#include "DialogFileBrowser.h"
#include "DialogError.h"
#include <QSound>
#include "DialogIconViewer.h"
#include "DialogLinearNews.h"
#include <QFocusEvent>
#include "DownloadManager.h"
#include "uploadmanager.h"
#include "dialogdownloadqueue.h"
#include "dialoguploadsqueue.h"
#include "dialogabout.h"
#include <QtWidgets/QListWidgetItem>
#include <QTextStream>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include "dialogbookmarks.h"

#include "WidgetChat.h"
#include "WidgetNews.h"
#include "WidgetConsole.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0, bool checkForUpdates = false);
    ~MainWindow();
    ConnectionController *connection;
    void log(QString);

public slots:
    void openNewConnectionDialog();
    void openAboutDialog();
    void slotCloseWindow();
    void disconnect();
    void onGotServerName();
    void onGotChatMessage(QString message);
    void onGotPM(QString message, qint16 uid);
    void onOpenMessagingWindow(quint32);
    void onOpenUserInfo(quint32);
    void onUserListChanged();
    void openPreferencesDialog();
    void showServerAgreement();
    void openTrackersDialog();
    void openNewWindow();
    void openFileBrowser();
    void onError(QString);
    void openIconViewer();
    void clearChat();
    void clearUserList();
    void openLinearNews();
    void openConsole();
    void onConnecting();
    void openDownloads();
    void openUploads();
    void removeTab(qint32);
    void slotCloseTab();
    void onDownloadQueueUpdated();
    void openThreadedNews();
    void changedTab();
    void playChatSound();
    void playPMSound();
    void onPreferencesSaved();
    void onConnected();
    void onVersionReady();
    void openBookmarksDialog();

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent*);

private:
    bool altPressed;

    void setStatus(QString s);

    QNetworkReply * pUpdateCheckReply;
    QNetworkAccessManager * pNetworkAccessManager;
    QNetworkRequest * pUpdateCheckRequest;

    Ui::MainWindow *ui;
    DialogOpenConnection * openConnectionDialog;
    QLabel * statusLabel;
    DialogAgreement * agreementDialog;
    DialogFileBrowser * fileBrowserDialog;
    QSound * chatSound;
    QSound * pmSound;
    DialogIconViewer * iconViewer;
    DialogLinearNews * linearNews;
    DownloadManager * downloadManager;
    UploadManager * uploadManager;
    DialogDownloadQueue * downloadsDialog;
    DialogUploadsQueue * uploadsDialog;
    DialogAbout * aboutDialog;
    DialogBookmarks * bookmarksDialog;

    WidgetChat * chatWidget;
    WidgetNews * threadedNewsWidget;
    WidgetConsole * debugConsole;
};

#endif // MAINWINDOW_H
