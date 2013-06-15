#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextCodec>
#include "DialogPreferences.h"
#include "DialogPrivateMessaging.h"
#include "DialogUserInfo.h"
#include "DialogTrackers.h"
#include "version.h"
#include <QDesktopServices>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent, bool checkForUpdates) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(removeTab(int)));

    altPressed = false;

    aboutDialog = new DialogAbout(this);

    QSettings settings("mir", "Contra");
    if(settings.value("JapaneseMode", false).toBool()) {
        //QTextCodec::setCodecForCStrings(QTextCodec::codecForName("Shift-JIS"));
    } else {
        //QTextCodec::setCodecForCStrings(QTextCodec::codecForName("Apple Roman"));
    }

    connection = new ConnectionController();

    chatWidget = new WidgetChat(this);
    ui->tabWidget->addTab(chatWidget, "Public Chat");

    threadedNewsWidget = new WidgetNews(connection);
    downloadManager = new DownloadManager(connection);
    uploadManager = new UploadManager(connection);
    debugConsole = new WidgetConsole();

    openConnectionDialog = new DialogOpenConnection(this, connection);
    agreementDialog = new DialogAgreement(this);
    fileBrowserDialog = new DialogFileBrowser(connection, this);
    iconViewer = new DialogIconViewer(this);
    linearNews = new DialogLinearNews(connection, this);
    downloadsDialog = new DialogDownloadQueue(downloadManager, this);
    uploadsDialog = new DialogUploadsQueue(uploadManager);

    connect(fileBrowserDialog, SIGNAL(requestedFile(QString, int, QString)), downloadManager, SLOT(onRequestedFile(QString, int, QString)));
    connect(fileBrowserDialog, SIGNAL(requestedFile(QString, int, QString)), this, SLOT(openDownloads()));

    connect(fileBrowserDialog, SIGNAL(requestedUpload(QString, int)), uploadManager, SLOT(onRequestedFile(QString, int)));
    connect(fileBrowserDialog, SIGNAL(requestedUpload(QString, int)), this, SLOT(openUploads()));

    connect(ui->actionOpen_Connection, SIGNAL(triggered()), this, SLOT(openNewConnectionDialog()));
    connect(ui->actionNew_Window, SIGNAL(triggered()), this, SLOT(openNewWindow()));
    connect(ui->actionAbout_Contra, SIGNAL(triggered()), this, SLOT(openAboutDialog()));
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionClose_Tab, SIGNAL(triggered()), this, SLOT(slotCloseTab()));
    connect(ui->actionDisconnect, SIGNAL(triggered()), this, SLOT(disconnect()));
    connect(ui->actionTrackers, SIGNAL(triggered()), this, SLOT(openTrackersDialog()));
    connect(ui->actionServer_Agreement, SIGNAL(triggered()), SLOT(showServerAgreement()));
    connect(ui->actionFiles, SIGNAL(triggered()), this, SLOT(openFileBrowser()));
    connect(ui->actionLinear_News, SIGNAL(triggered()), this, SLOT(openLinearNews()));
    connect(ui->actionDownloads, SIGNAL(triggered()), this, SLOT(openDownloads()));
    connect(ui->actionThreaded_News, SIGNAL(triggered()), this, SLOT(openThreadedNews()));
    //connect(ui->actionDebug_console, SIGNAL(triggered()), this, SLOT(openConsole()));

    connect(connection, SIGNAL(gotServerName()), this, SLOT(onGotServerName()));
    connect(connection, SIGNAL(gotChatMessage(char *, short)), this, SLOT(onGotChatMessage(char *, short)));
    connect(connection, SIGNAL(userListChanged()), this, SLOT(onUserListChanged()));
    connect(connection, SIGNAL(gotPM(short, QString)), this, SLOT(onGotPM(short,QString)));

    connect(connection, SIGNAL(serverError(QString)), this, SLOT(onError(QString)));
    connect(connection, SIGNAL(socketError(QString)), this, SLOT(onError(QString)));

    connect(ui->actionPreferences, SIGNAL(triggered()), this, SLOT(openPreferencesDialog()));
    connect(iconViewer, SIGNAL(iconChanged()), connection, SLOT(onNameChanged()));

    connect(ui->actionIcons, SIGNAL(triggered()), this, SLOT(openIconViewer()));

    connect(connection, SIGNAL(connecting()), this, SLOT(clearChat()));
    connect(connection, SIGNAL(connecting()), this, SLOT(clearUserList()));
    connect(connection, SIGNAL(connecting()), this, SLOT(onConnecting()));
    connect(connection, SIGNAL(connected()), this, SLOT(onConnected()));

    connect(connection, SIGNAL(gotFile(unsigned int, unsigned int, unsigned int)), downloadManager, SLOT(addDownload(unsigned int, unsigned int, unsigned int)));
    connect(connection, SIGNAL(gotUpload(unsigned int)), uploadManager, SLOT(addUpload(unsigned int)));

    connect(chatWidget, SIGNAL(messagingWindowSignal(uint)), this, SLOT(onOpenMessagingWindow(uint)));
    connect(chatWidget, SIGNAL(userInfoSignal(uint)), this, SLOT(onOpenUserInfo(uint)));
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(changedTab()));

    statusLabel = new QLabel(ui->statusBar);
    ui->statusBar->addWidget(statusLabel);

    setStatus(QString("Not connected"));

    chatSound = new QSound("sounds/chat.wav");
    pmSound = new QSound("sounds/pm.wav");

    chatWidget->chatSound = chatSound;
    chatWidget->connection = connection;

    onPreferencesSaved();

    // Check for updates
    pUpdateCheckReply = NULL;
    pNetworkAccessManager = new QNetworkAccessManager(this);
    pUpdateCheckRequest = NULL;
    if(checkForUpdates)
    {
        pUpdateCheckRequest = new QNetworkRequest(QUrl("http://www.sumamimasen.com/obsession/version"));
        pUpdateCheckReply = pNetworkAccessManager->get(*pUpdateCheckRequest);
        connect(pUpdateCheckReply, SIGNAL(finished()), this, SLOT(onVersionReady()));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    delete connection;
    delete chatSound;
    delete pmSound;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::setStatus(QString s) {
    statusLabel->setText(s);
}

void MainWindow::log(QString t) {
    debugConsole->addText(t+"\n");
}

// SLOTS

void MainWindow::onConnected() {
    setStatus("Connected");
}

void MainWindow::playChatSound() {
    QSettings settings("mir", "Contra");
    if(settings.value("soundsEnabled", true).toBool()) {
        chatSound->play();
    }
}

void MainWindow::playPMSound() {
    QSettings settings("mir", "Contra");
    if(settings.value("soundsEnabled", true).toBool()) {
        pmSound->play();
    }
}

void MainWindow::changedTab() {
    if(ui->tabWidget->currentIndex() == 0 && ui->tabWidget->tabText(0) != "Public Chat") {
        ui->tabWidget->setTabText(0, "Public Chat");
    }
}

void MainWindow::openThreadedNews() {
    qDebug() << "Adding threaded news widget...";
    ui->tabWidget->addTab(threadedNewsWidget, "News");
    qDebug() << "Setting focus...";
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
    threadedNewsWidget->getNews();
}

void MainWindow::onDownloadQueueUpdated() {
    if(!downloadsDialog->open) {
        openDownloads();
    }
}

void MainWindow::removeTab(int i) {
    qDebug() << "Clicked on tab close " << i;
    if(i != 0) {
        ui->tabWidget->removeTab(i);
    }
}

void MainWindow::openDownloads() {
    bool isThere = false;
    for(int i=0; i<ui->tabWidget->count(); i++) {
        if(ui->tabWidget->widget(i) == downloadsDialog) {
            isThere = true;
        }
    }
    if(!isThere) {
        ui->tabWidget->addTab(downloadsDialog, "Downloads");
        ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
    }
}

void MainWindow::openUploads() {
    bool isThere = false;
    for(int i=0; i<ui->tabWidget->count(); i++) {
        if(ui->tabWidget->widget(i) == uploadsDialog) {
            isThere = true;
        }
    }
    if(!isThere) {
        ui->tabWidget->addTab(uploadsDialog, "Uploads");
        ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
    }
}

void MainWindow::onConnecting() {
    connection->closeConnection();
    setStatus("Connecting...");
    ui->tabWidget->clear();
    threadedNewsWidget->clear();
    fileBrowserDialog->resetPath();
    ui->tabWidget->addTab(chatWidget, "Public Chat");
}

void MainWindow::openLinearNews() {
    ui->tabWidget->addTab(linearNews, "Message Board");
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
    linearNews->requestNews();
}

void MainWindow::openConsole() {
    ui->tabWidget->addTab(debugConsole, "Debug console");
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
}

void MainWindow::clearChat() {
    chatWidget->clearChat();
}

void MainWindow::clearUserList() {
    chatWidget->clearUserList();
}
void MainWindow::openIconViewer() {
    iconViewer->show();
}

void MainWindow::onError(QString errorString) {
    DialogError * dialog = new DialogError(errorString);
    dialog->show();
    setStatus(errorString);
}

void MainWindow::openFileBrowser() {
    qDebug() << "Opening file browser...";
    ui->tabWidget->addTab(fileBrowserDialog, "Files");
    //fileBrowserDialog->show();
    fileBrowserDialog->load();
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
}

void MainWindow::openTrackersDialog() {
    DialogTrackers * dialog = new DialogTrackers(connection, this);
    dialog->show();
}

void MainWindow::openNewConnectionDialog() {
    openConnectionDialog->clear();
    openConnectionDialog->open();
}

void MainWindow::openNewWindow() {
    MainWindow * newWindow = new MainWindow();
    newWindow->show();
    newWindow->openNewConnectionDialog();
}

void MainWindow::openAboutDialog() {
    aboutDialog->show();
}

void MainWindow::closeEvent(QCloseEvent*) {
    slotCloseWindow();
}

void MainWindow::slotCloseTab() {
    if(ui->tabWidget->count() > 1) {
        if(ui->tabWidget->widget(ui->tabWidget->count()-1) == downloadsDialog) {
            downloadsDialog->open = false;
        }
        ui->tabWidget->removeTab(ui->tabWidget->count()-1);
    }
}

void MainWindow::slotCloseWindow() {
    // TODO: Ask for confirmation
    connection->closeConnection();
    if(openConnectionDialog) {
        delete openConnectionDialog;
    }
    if(statusLabel) {
        delete statusLabel;
    }
    if(agreementDialog) {
        delete agreementDialog;
    }
    connection->disconnect();
    this->close();
}

void MainWindow::disconnect() {
    connection->closeConnection();
    clearChat();
    clearUserList();
    setStatus("Disconnected");
}

void MainWindow::showServerAgreement() {
    agreementDialog->setAgreement(connection->serverAgreement());
    ui->tabWidget->addTab(agreementDialog, "Agreement");
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
}

void MainWindow::onGotServerName() {
    setStatus(QString("Connected to ") + connection->serverName());
}

void MainWindow::onGotChatMessage(char * t, short l) {
    char * temp = (char *) malloc(sizeof(char)*l+1);
    playChatSound();
    memcpy(temp, t, l);
    temp[l] = '\0';
    QString msg = QString(temp);
    chatWidget->printChat(msg);
    delete temp;
    if(ui->tabWidget->currentWidget() != chatWidget) {
        ui->tabWidget->setTabText(0, "*Public Chat*");
    }
}

void MainWindow::onUserListChanged() {
    qDebug() << "Updating QListWidget...";
    std::vector<s_user*> * users = connection->getUserList();
    clearUserList();
    for(unsigned int i=0; i<users->size(); i++) {
        users->at(i)->orderInList = i;
        QString name = QString(users->at(i)->name);
        QListWidgetItem * item = new QListWidgetItem("           "+name);
        QFont f = QFont();
        f.setBold(true);
        item->setFont(f);
        switch(users->at(i)->flags%4) {
        default:
        case 0:
            item->setTextColor(QColor(0, 0, 0));
            break;
        case 1:
            item->setTextColor(QColor(100, 100, 100));
            break;
        case 2:
            item->setTextColor(QColor(127, 0, 0));
            break;
        case 3:
            item->setTextColor(QColor(127, 0, 127));
            break;
        }

        QString path = *(users->at(i)->iconPath);
        qDebug() << path << " for " << name;
        QImage image = QImage(path);
        if(!image.isNull()) {
            item->setBackground(QBrush(image));
            item->setSizeHint(QSize(232, image.size().height()));
        }
        chatWidget->addUser(item);
    }
}

void MainWindow::onPreferencesSaved() {
    QFont font;
    QSettings settings("mir", "Contra");
    font.setFamily(settings.value("fontFamily", "MS Shell Dlg2").toString());
    int style = settings.value("fontStyle", 0).toInt();
    switch(style) {
    case 0:
        font.setStyle(QFont::StyleNormal);
        break;
    case 1:
        font.setStyle(QFont::StyleItalic);
        break;
    case 2:
        font.setStyle(QFont::StyleOblique);
        break;
    }

    font.setPointSize(settings.value("fontSize", 8).toInt());
    font.setBold(settings.value("fontBold", false).toBool());
    chatWidget->setChatFont(font);
}

void MainWindow::openPreferencesDialog() {
    DialogPreferences * dialog = new DialogPreferences(this);
    connect(dialog, SIGNAL(saved()), connection, SLOT(onNameChanged()));
    connect(dialog, SIGNAL(saved()), this, SLOT(onPreferencesSaved()));
    dialog->serverIdent = connection->serverIdent;
    dialog->open();
}

void MainWindow::onGotPM(short uid, QString s) {
    s_user * user = connection->getUserByUid(uid);
    if(user) {
        if(user->messagingWindow == NULL || !user->messagingWindow) {
            user->messagingWindow = new DialogPrivateMessaging(uid, connection, this);
            connect(user->messagingWindow, SIGNAL(sentPM()), this, SLOT(playChatSound()));
        }
        if(user->messagingWindow->isVisible()) {
            playChatSound();
        } else {
            playPMSound();
        }
        user->messagingWindow->show();
        user->messagingWindow->gotMessage(s);
    }
}

void MainWindow::onOpenMessagingWindow(unsigned int order) {
    std::vector<s_user *> * users = connection->getUserList();
    unsigned short uid = 0;
    for(unsigned int i=0; i<users->size(); i++) {
        if(users->at(i)->orderInList == order) {
            uid = users->at(i)->id;
        }
    }
    if(uid == 0) {
        qDebug() << "Warning: User not found";
        return;
    }

    s_user * user = connection->getUserByUid(uid);
    if(user) {
        if(user->messagingWindow == NULL) {
            user->messagingWindow = new DialogPrivateMessaging(uid, connection, this);
            connect(user->messagingWindow, SIGNAL(sentPM()), this, SLOT(playChatSound()));
        }
        user->messagingWindow->show();
    }
}

void MainWindow::onOpenUserInfo(unsigned int order) {
    std::vector<s_user *> * users = connection->getUserList();
    unsigned short uid = 0;
    for(unsigned int i=0; i<users->size(); i++) {
        if(users->at(i)->orderInList == order) {
            uid = users->at(i)->id;
        }
    }
    if(uid == 0) {
        qDebug() << "Warning: User not found";
        return;
    }

    s_user * user = connection->getUserByUid(uid);
    if(user) {
        if(user->infoWindow == NULL) {
            user->infoWindow = new DialogUserInfo(uid, connection, this);
        }
        user->infoWindow->show();
    }
}

void MainWindow::onVersionReady()
{
    if(pUpdateCheckReply->error() != QNetworkReply::NoError)
    {
        qDebug() << "Error retrieving last version number";
        pUpdateCheckReply->deleteLater();
        return;
    }

    QByteArray versionArray = pUpdateCheckReply->readAll();
    qDebug() << "Current version: " << QString("%1.%2").arg(VERSION_MAJOR).arg(VERSION_MINOR);
    qDebug() << "Latest version: " << versionArray;

    QList<QByteArray> splitArray = versionArray.split('.');
    if(splitArray[0].toInt() > VERSION_MAJOR || splitArray[1].toInt() > VERSION_MINOR)
    {
        qDebug() << "Update needed.";
        switch(QMessageBox::information(this, "Update available",
                                        "There is a new version of Obsession ready for download."
                                        " Do you want to launch your web browser to download it?", "Yes", "No"))
        {
            case 0:
            QDesktopServices::openUrl(QUrl("https://github.com/tjohnman/Obsession/blob/master/README.md"));
            break;
        }
    }

    pUpdateCheckReply->deleteLater();
}
