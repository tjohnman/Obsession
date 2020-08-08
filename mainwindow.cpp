#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextCodec>
#include "dialogpreferences.h"
#include "dialogprivatemessaging.h"
#include "dialoguserinfo.h"
#include "dialogtrackers.h"
#include "version.h"
#include <QDesktopServices>
#include <QMessageBox>
#include "dialogprivileges.h"
#include "dialogrequestaccount.h"
#include "dialogcreateaccount.h"
#include "dialogbroadcast.h"
#include "TextHelper.h"

MainWindow::MainWindow(QWidget *parent, bool checkForUpdates) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->tabWidget, SIGNAL(tabCloseRequested(qint32)), this, SLOT(removeTab(qint32)));

    altPressed = false;

    aboutDialog = new DialogAbout(this);

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
    bookmarksDialog = new DialogBookmarks(this);

    connect(fileBrowserDialog, SIGNAL(requestedFile(QString, qint32, QString)), downloadManager, SLOT(onRequestedFile(QString, qint32, QString)));
    connect(fileBrowserDialog, SIGNAL(requestedFile(QString, qint32, QString)), this, SLOT(openDownloads()));

    connect(fileBrowserDialog, SIGNAL(requestedUpload(QString, qint32)), uploadManager, SLOT(onRequestedFile(QString, qint32)));
    connect(fileBrowserDialog, SIGNAL(requestedUpload(QString, qint32)), this, SLOT(openUploads()));

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
    connect(ui->actionBookmarks, SIGNAL(triggered()), this, SLOT(openBookmarksDialog()));
    connect(ui->actionView_account, SIGNAL(triggered()), this, SLOT(openRequestPermissions()));
    connect(ui->actionNew_account, SIGNAL(triggered()), this, SLOT(onClickCreateAccount()));
    connect(ui->actionBroadcast, SIGNAL(triggered()), this, SLOT(onClickBroadcast()));

    connect(connection, SIGNAL(gotServerName()), this, SLOT(onGotServerName()));
    connect(connection, SIGNAL(gotChatMessage(QString)), this, SLOT(onGotChatMessage(QString)));
    connect(connection, SIGNAL(gotBroadcast(QString)), this, SLOT(onGotBroadcast(QString)));
    connect(connection, SIGNAL(userListChanged()), this, SLOT(onUserListChanged()));
    connect(connection, SIGNAL(gotPM(QString, qint16)), this, SLOT(onGotPM(QString, qint16)));
    connect(connection, SIGNAL(gotUserInfo(QString,QString,quint16)), this, SLOT(onOpenUserInfo(QString,QString,quint16)));
    connect(connection, SIGNAL(gotPermissions(QString, QString, quint8, quint8, quint8, quint8, quint8, quint8, quint8, quint8)), this, SLOT(gotPermissions(QString, QString, quint8, quint8, quint8, quint8, quint8, quint8, quint8, quint8)));

    connect(connection, SIGNAL(userLeft(s_user *)), this, SLOT(onUserLeft(s_user *)));
    connect(connection, SIGNAL(userChangedName(QString, QString)), this, SLOT(onUserChangedName(QString, QString)));

    connect(connection, SIGNAL(serverError(QString)), this, SLOT(onError(QString)));
    connect(connection, SIGNAL(socketError(QString)), this, SLOT(onError(QString)));

    connect(ui->actionPreferences, SIGNAL(triggered()), this, SLOT(openPreferencesDialog()));
    connect(iconViewer, SIGNAL(iconChanged()), connection, SLOT(onNameChanged()));

    connect(ui->actionIcons, SIGNAL(triggered()), this, SLOT(openIconViewer()));

    connect(connection, SIGNAL(connecting()), this, SLOT(clearUserList()));
    connect(connection, SIGNAL(connecting()), this, SLOT(onConnecting()));
    connect(connection, SIGNAL(connected()), this, SLOT(onConnected()));

    connect(connection, SIGNAL(gotFile(quint32, quint32, quint32)), downloadManager, SLOT(addDownload(quint32, quint32, quint32)));
    connect(connection, SIGNAL(gotUpload(quint32)), uploadManager, SLOT(addUpload(quint32)));

    connect(chatWidget, SIGNAL(messagingWindowSignal(quint16)), this, SLOT(onOpenMessagingWindow(quint16)));
    connect(chatWidget, SIGNAL(userInfoSignal(quint16)), this, SLOT(requestUserInfo(quint16)));
    connect(chatWidget, SIGNAL(kickUserSignal(quint16)), this, SLOT(kickUser(quint16)));
    connect(ui->tabWidget, SIGNAL(currentChanged(qint32)), this, SLOT(changedTab()));

    statusLabel = new QLabel(ui->statusBar);
    ui->statusBar->addWidget(statusLabel);

    setStatus(QString("Not connected"));

    chatSound = new QSound(":/sounds/chat.wav");
    pmSound = new QSound(":/sounds/pm.wav");

    chatWidget->chatSound = chatSound;
    chatWidget->connection = connection;

    onPreferencesSaved();

    // Check for updates
    /*pUpdateCheckReply = NULL;
    pNetworkAccessManager = new QNetworkAccessManager(this);
    pUpdateCheckRequest = NULL;
    if(checkForUpdates)
    {
        pUpdateCheckRequest = new QNetworkRequest(QUrl(""));
        pUpdateCheckReply = pNetworkAccessManager->get(*pUpdateCheckRequest);
        connect(pUpdateCheckReply, SIGNAL(finished()), this, SLOT(onVersionReady()));
    }*/

    keepAliveTimer.setInterval(120000);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete connection;
    delete chatSound;
    delete pmSound;
    delete bookmarksDialog;
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

void MainWindow::autoConnect() {
    bool auto_connected = false;
    QSettings settings("mir", "Contra");

    QString auto_bookmark = settings.value("autoBookmark", "").toString();
    int bookmark_count = settings.value("bookmarkCount", 0).toInt();

    if(auto_bookmark != "") for(int i=0; i<bookmark_count; ++i) {
        if(settings.value("bookmarkaddress"+QString::number(i)) == auto_bookmark) {
            auto_connected = true;
            this->connection->connectToServer(settings.value("bookmarkaddress"+QString::number(i), "localhost").toString(),
                                          settings.value("bookmarklogin"+QString::number(i), "").toString(),
                                          settings.value("bookmarkpassword"+QString::number(i), "").toString());
        }
    }

    if(!auto_connected) {
        this->openNewConnectionDialog();
    }
}

void MainWindow::onConnected() {
    setStatus("Connected");

    QSettings settings("mir", "Contra");
    if(settings.value("connectionKeepAlive", true).toBool())
    {
        connect(&keepAliveTimer, SIGNAL(timeout()), connection, SLOT(requestUserList()));
        keepAliveTimer.start();
    }
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
    ui->tabWidget->addTab(threadedNewsWidget, "News");
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
    threadedNewsWidget->getNews();
}

void MainWindow::onDownloadQueueUpdated() {
    if(!downloadsDialog->open) {
        openDownloads();
    }
}

void MainWindow::removeTab(qint32 i) {
    if(i != 0) {
        ui->tabWidget->removeTab(i);
    }
}

void MainWindow::openDownloads() {
    bool isThere = false;
    for(qint32 i=0; i<ui->tabWidget->count(); i++) {
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
    for(qint32 i=0; i<ui->tabWidget->count(); i++) {
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
    connection->closeConnection(true);
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
    connect(dialog, SIGNAL(finished(int)), dialog, SLOT(deleteLater()));
    setStatus(errorString);
}

void MainWindow::openFileBrowser() {
    ui->tabWidget->addTab(fileBrowserDialog, "Files");
    fileBrowserDialog->load();
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
}

void MainWindow::openTrackersDialog() {
    DialogTrackers * dialog = new DialogTrackers(connection, this);
    dialog->show();
}

void MainWindow::openNewConnectionDialog() {
    openConnectionDialog->clear();
    openConnectionDialog->updateBookmarkList();
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

void MainWindow::onGotBroadcast(QString message)
{
    onError("Server-wide broadcast: "+message);
}

void MainWindow::onGotChatMessage(QString message) {
    playChatSound();
    chatWidget->printChat(message);
    if(ui->tabWidget->currentWidget() != chatWidget) {
        ui->tabWidget->setTabText(0, "*Public Chat*");
    }
}

void MainWindow::openRequestPermissions()
{
    DialogRequestAccount * dialog = new DialogRequestAccount(connection, this);
    dialog->show();
    connect(dialog, SIGNAL(finished(int)), dialog, SLOT(deleteLater()));
}

void MainWindow::onClickBroadcast()
{
    DialogBroadcast * dialog = new DialogBroadcast(connection, this);
    dialog->show();
    connect(dialog, SIGNAL(finished(int)), dialog, SLOT(deleteLater()));
}

void MainWindow::gotPermissions(QString login, QString password, quint8 p1, quint8 p2, quint8 p3, quint8 p4, quint8 p5, quint8 p6, quint8 p7, quint8 p8)
{
    DialogPrivileges * dialog = new DialogPrivileges(connection, login, password, p1, p2, p3, p4, p5, p6, p7, p8, this);
    dialog->readPrivileges();
    dialog->show();
    connect(dialog, SIGNAL(finished(int)), dialog, SLOT(deleteLater()));
}

void MainWindow::onClickCreateAccount()
{
    DialogCreateAccount * dialog = new DialogCreateAccount(connection, this);
    dialog->show();
    connect(dialog, SIGNAL(finished(int)), dialog, SLOT(deleteLater()));
}

void MainWindow::onUserListChanged() {
    std::vector<s_user*> * users = connection->getUserList();
    clearUserList();
    for(quint32 i=0; i<users->size(); i++) {
        s_user * user = users->at(i);
        user->orderInList = i;

        DialogPrivateMessaging * private_messages = this->getUserPrivateChat(user);
        private_messages->user = user;

        QListWidgetItem * item = new QListWidgetItem("           "+TextHelper::DecodeTextAutoUTF8(user->name, user->nameLength));
        QFont f = QFont();

        f.setBold(true);
        item->setFont(f);
        switch(user->flags%4) {
        default:
        case 0:
            item->setForeground(QColor(0, 0, 0));
            break;
        case 1:
            item->setForeground(QColor(100, 100, 100));
            break;
        case 2:
            item->setForeground(QColor(127, 0, 0));
            break;
        case 3:
            item->setForeground(QColor(127, 0, 127));
            break;
        }

        QString path = *(users->at(i)->iconPath);

        QImage image = QImage(path);
        if(!image.isNull()) {
            item->setBackground(QBrush(image));
            item->setSizeHint(QSize(232, image.size().height()));
        }
        chatWidget->addUser(item, user->id);
    }
}

void MainWindow::openBookmarksDialog()
{
    bookmarksDialog->loadBookmarks();
    bookmarksDialog->show();
}

void MainWindow::onPreferencesSaved() {
    QFont font;
    QSettings settings("mir", "Contra");
    font.setFamily(settings.value("fontFamily", "MS Shell Dlg2").toString());
    qint32 style = settings.value("fontStyle", 0).toInt();
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
    chatWidget->setEncodingLabel(settings.value("EncodingName", "Mac OS Roman").toString());

    if(settings.value("connectionKeepAlive", true).toBool())
    {
        connect(&keepAliveTimer, SIGNAL(timeout()), connection, SLOT(requestUserList()));
        keepAliveTimer.start();
    }
    else
    {
        keepAliveTimer.disconnect(&keepAliveTimer, SIGNAL(timeout()), connection, SLOT(requestUserList()));
        keepAliveTimer.stop();
    }
}

void MainWindow::onUserChangedName(QString old_name, QString new_name) {
    this->pPrivateChats[new_name.toStdString()] = this->pPrivateChats[old_name.toStdString()];
    this->pPrivateChats.erase(old_name.toStdString());
}

void MainWindow::onUserLeft(s_user * user) {
    std::string user_hash = connection->getUserHash(user);
    std::map<std::string, DialogPrivateMessaging *>::iterator it = this->pPrivateChats.find(user_hash);
    if(it != this->pPrivateChats.end()) {
        delete this->pPrivateChats[user_hash];
        this->pPrivateChats.erase(user_hash);
    }
}

void MainWindow::openPreferencesDialog() {
    DialogPreferences * dialog = new DialogPreferences(this);
    connect(dialog, SIGNAL(saved()), connection, SLOT(onNameChanged()));
    connect(dialog, SIGNAL(saved()), this, SLOT(onPreferencesSaved()));
    dialog->serverIdent = connection->serverIdent;
    dialog->open();
}

void MainWindow::onGotPM(QString message, qint16 uid) {
    s_user * user = connection->getUserByUid(uid);

    if(user) {
        DialogPrivateMessaging * private_chat = this->getUserPrivateChat(user);

        if(private_chat->isVisible()) {
            playChatSound();
        } else {
            playPMSound();
            private_chat->show();
        }

        private_chat->gotMessage(message);
    }
}

void MainWindow::onOpenMessagingWindow(quint16 uid) {
    if(uid == 0) {
        qDebug() << "Warning: User not found";
        return;
    }

    s_user * user = connection->getUserByUid(uid);
    if(user) {
        this->getUserPrivateChat(user)->show();
    }
}

void MainWindow::kickUser(quint16 uid)
{
    CTransaction * transaction = connection->createTransaction(110);
    transaction->addParameter(103, uid);
    connection->sendTransaction(transaction);
}

void MainWindow::requestUserInfo(quint16 uid)
{
    connection->requestUserInfo(uid);
}

void MainWindow::onOpenUserInfo(QString username, QString info, quint16 uid) {
    if(uid == 0) {
        qDebug() << "Warning: User not found";
        return;
    }

    s_user * user = connection->getUserByUid(uid);
    if(user) {
        if(user->infoWindow == NULL) {
            user->infoWindow = new DialogUserInfo(uid, connection, this);
        }
        user->infoWindow->gotUserInfo(username, info);
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
            QDesktopServices::openUrl(QUrl(""));
            break;
        }
    }

    pUpdateCheckReply->deleteLater();
}


DialogPrivateMessaging * MainWindow::getUserPrivateChat(s_user * user) {
    std::string user_hash = connection->getUserHash(user);
    std::map<std::string, DialogPrivateMessaging *>::iterator it = this->pPrivateChats.find(user_hash);

    DialogPrivateMessaging * privateChat;

    if(it != this->pPrivateChats.end() && it->second) {
        return it->second;
    } else {
        privateChat = new DialogPrivateMessaging(user->id, connection, this);
        this->pPrivateChats[user_hash] = privateChat;
        return privateChat;
    }
}
