#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ConnectionController.h"
#include "dialogopenconnection.h"
#include <QLabel>
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
#include <QListWidgetItem>
#include <QTextStream>

#include "WidgetChat.h"
#include "WidgetNews.h"
#include "WidgetConsole.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    ConnectionController *connection;
    void log(QString);

public slots:
    void openNewConnectionDialog();
    void openAboutDialog();
    void slotCloseWindow();
    void disconnect();
    void onGotServerName();
    void onGotChatMessage(char *, short);
    void onGotPM(short, QString);
    void onOpenMessagingWindow(unsigned int);
    void onOpenUserInfo(unsigned int);
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
    void removeTab(int);
    void slotCloseTab();
    void onDownloadQueueUpdated();
    void openThreadedNews();
    void changedTab();
    void playChatSound();
    void playPMSound();
    void onPreferencesSaved();
    void onConnected();

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent*);

private:
    bool altPressed;

    void setStatus(QString s);

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

    WidgetChat * chatWidget;
    WidgetNews * threadedNewsWidget;
    WidgetConsole * debugConsole;
};

#endif // MAINWINDOW_H
