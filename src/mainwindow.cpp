#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStringConverter>
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
#include "ThemeManager.h"
#include "SettingsManager.h"

MainWindow::MainWindow(QWidget *parent, bool checkForUpdates) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(removeTab(int)));

    altPressed = false;

    aboutDialog = new DialogAbout(this);

    connection = new ConnectionController();

    chatWidget = new WidgetChat(this);
    ui->tabWidget->addTab(chatWidget, QString::fromUtf8("Public Chat"));

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
    connect(ui->actionChat, SIGNAL(triggered()), this, SLOT(openChat()));
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
    connect(connection, SIGNAL(gotPM(QString,qint16)), this, SLOT(onGotPM(QString,qint16)));
    connect(connection, SIGNAL(gotUserInfo(QString,QString,quint16)), this, SLOT(onOpenUserInfo(QString,QString,quint16)));
    connect(connection, SIGNAL(gotPermissions(QString,QString,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8)), this, SLOT(gotPermissions(QString,QString,quint8,quint8,quint8,quint8,quint8,quint8,quint8,quint8)));

    connect(connection, SIGNAL(userLeft(s_user*)), this, SLOT(onUserLeft(s_user*)));
    connect(connection, SIGNAL(userChangedName(QString,QString)), this, SLOT(onUserChangedName(QString,QString)));

    connect(connection, SIGNAL(serverError(QString)), this, SLOT(onError(QString)));
    connect(connection, SIGNAL(socketError(QString)), this, SLOT(onError(QString)));

    connect(ui->actionPreferences, SIGNAL(triggered()), this, SLOT(openPreferencesDialog()));
    connect(iconViewer, SIGNAL(iconChanged()), connection, SLOT(onNameChanged()));

    connect(ui->actionIcons, SIGNAL(triggered()), this, SLOT(openIconViewer()));

    connect(connection, SIGNAL(connecting()), this, SLOT(clearUserList()));
    connect(connection, SIGNAL(connecting()), this, SLOT(onConnecting()));
    connect(connection, SIGNAL(connected()), this, SLOT(onConnected()));

    connect(connection, SIGNAL(gotFile(quint32,quint32,quint32)), downloadManager, SLOT(addDownload(quint32,quint32,quint32)));
    connect(connection, SIGNAL(gotUpload(quint32)), uploadManager, SLOT(addUpload(quint32)));

    connect(chatWidget, SIGNAL(messagingWindowSignal(quint16)), this, SLOT(onOpenMessagingWindow(quint16)));
    connect(chatWidget, SIGNAL(userInfoSignal(quint16)), this, SLOT(requestUserInfo(quint16)));
    connect(chatWidget, SIGNAL(kickUserSignal(quint16)), this, SLOT(kickUser(quint16)));
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(changedTab()));
    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(onCloseTabRequested(int)));

    QFont font = ui->statusLabel->font();
    font.setPointSize(font.pointSize() - 1);
    ui->statusLabel->setFont(font);
    setStatus(QString::fromUtf8("Not connected"));

    chatSound = new QSoundEffect(this);
    pmSound = new QSoundEffect(this);
    
    #ifdef Q_OS_WIN32
    chatSound->setSource(QUrl::fromLocalFile(QString::fromUtf8("./sounds/chat.wav")));
    pmSound->setSource(QUrl::fromLocalFile(QString::fromUtf8("./sounds/pm.wav")));
    #else
    chatSound->setSource(QUrl(QString::fromUtf8("qrc:/sounds/chat.wav")));
    pmSound->setSource(QUrl(QString::fromUtf8("qrc:/sounds/pm.wav")));
    #endif

    chatWidget->chatSound = chatSound;
    chatWidget->connection = connection;

    onPreferencesSaved();

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
    ui->statusLabel->setText(s);
}

void MainWindow::log(QString t) {
    debugConsole->addText(t+ QString::fromUtf8("\n"));
}

void MainWindow::autoConnect() {
    bool auto_connected = false;
    auto& settings = SettingsManager::instance();

    QString auto_bookmark = settings.value(QString::fromUtf8("autoBookmark"), QString::fromUtf8("")).toString();
    int bookmark_count = settings.value("bookmarkCount", 0).toInt();

    if(auto_bookmark != QString::fromUtf8("")) for(int i=0; i<bookmark_count; ++i) {
        if(settings.value(QString::fromUtf8("bookmarkaddress")+QString::number(i)) == auto_bookmark) {
            auto_connected = true;
            this->connection->connectToServer(settings.value(QString::fromUtf8("bookmarkaddress")+QString::number(i), QString::fromUtf8("localhost")).toString(),
                                          settings.value(QString::fromUtf8("bookmarklogin")+QString::number(i),  QString::fromUtf8("")).toString(),
                                          settings.value(QString::fromUtf8("bookmarkpassword")+QString::number(i),  QString::fromUtf8("")).toString());
        }
    }

    if(!auto_connected) {
        this->openNewConnectionDialog();
    }
}

void MainWindow::onConnected() {
    setStatus(QString::fromUtf8("Connected"));

    auto& settings = SettingsManager::instance();
    if(settings.value("connectionKeepAlive", true).toBool())
    {
        connect(&keepAliveTimer, SIGNAL(timeout()), connection, SLOT(requestUserList()));
        keepAliveTimer.start();
    }

    this->clearChat();
}

void MainWindow::playChatSound() {
    auto& settings = SettingsManager::instance();
    if(settings.value("soundsEnabled", true).toBool()) {
        chatSound->play();
    }
}

void MainWindow::playPMSound() {
    auto& settings = SettingsManager::instance();
    if(settings.value("soundsEnabled", true).toBool()) {
        pmSound->play();
    }
}

void MainWindow::changedTab() {
    if(ui->tabWidget->currentIndex() == 0 && ui->tabWidget->tabText(0) != QString::fromUtf8("Public Chat")) {
        ui->tabWidget->setTabText(0, QString::fromUtf8("Public Chat"));
    }
}

void MainWindow::openThreadedNews() {
    ui->tabWidget->addTab(threadedNewsWidget, QString::fromUtf8("News"));
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
    threadedNewsWidget->getNews();
}

void MainWindow::onDownloadQueueUpdated() {
    if(!downloadsDialog->open) {
        openDownloads();
    }
}

void MainWindow::removeTab(int i) {
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
        ui->tabWidget->addTab(downloadsDialog, QString::fromUtf8("Downloads"));
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
        ui->tabWidget->addTab(uploadsDialog, QString::fromUtf8("Uploads"));
        ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
    }
}

void MainWindow::onConnecting() {
    connection->closeConnection(true);
    setStatus(QString::fromUtf8("Connecting..."));
    ui->tabWidget->clear();
    threadedNewsWidget->clear();
    fileBrowserDialog->resetPath();
    this->openChat();
}

void MainWindow::openChat()
{
    ui->tabWidget->addTab(chatWidget, QString::fromUtf8("Public Chat"));
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
}

void MainWindow::openLinearNews() {
    ui->tabWidget->addTab(linearNews, QString::fromUtf8("Message Board"));
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
    linearNews->requestNews();
}

void MainWindow::openConsole() {
    ui->tabWidget->addTab(debugConsole, QString::fromUtf8("Debug console"));
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
    ui->tabWidget->addTab(fileBrowserDialog, QString::fromUtf8("Files"));
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

void MainWindow::onCloseTabRequested(int index)
{
    if (ui->tabWidget->widget(index) == downloadsDialog)
    {
        downloadsDialog->open = false;
    }
    ui->tabWidget->removeTab(index);
}

void MainWindow::slotCloseWindow() {
    // TODO: Ask for confirmation
    connection->closeConnection();
    if(openConnectionDialog) {
        delete openConnectionDialog;
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
    setWindowTitle(QString::fromUtf8("Obsession Hotline Client"));
    setStatus(QString::fromUtf8("Disconnected"));
}

void MainWindow::showServerAgreement() {
    agreementDialog->setAgreement(connection->serverAgreement());
    ui->tabWidget->addTab(agreementDialog, QString::fromUtf8("Agreement"));
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
}

void MainWindow::onGotServerName() {
    setStatus(QString::fromUtf8("Connected to ") + connection->serverName());
    setWindowTitle(connection->serverName() + QString::fromUtf8(" | Obsession Hotline Client"));
}

void MainWindow::onGotBroadcast(QString message)
{
    onError(QString::fromUtf8("Server-wide broadcast: ")+message);
}

void MainWindow::onGotChatMessage(QString message) {
    playChatSound();
    chatWidget->printChat(message);
    if(ui->tabWidget->currentWidget() != chatWidget) {
        ui->tabWidget->setTabText(0, QString::fromUtf8("*Public Chat*"));
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
    auto& settings = SettingsManager::instance();

    std::vector<s_user*> * users = connection->getUserList();
    clearUserList();
    for(quint32 i=0; i<users->size(); i++) {
        s_user * user = users->at(i);
        user->orderInList = i;

        DialogPrivateMessaging * private_messages = this->getUserPrivateChat(user);
        private_messages->user = user;

        QListWidgetItem * item = new QListWidgetItem(QString::fromUtf8("           ")+TextHelper::DecodeText(user->name, user->nameLength));
        QFont f = QFont();

        f.setBold(true);
        item->setFont(f);

        QString path = *(users->at(i)->iconPath);
        QImage image = QImage(path);
        bool use_light_color = false;

        if(!image.isNull()) {
            item->setBackground(QBrush(image));
            item->setSizeHint(QSize(232, image.size().height()));

            if(settings.value("useLightColorNames", true).toBool()) {
                qreal average = 0;
                int height = image.height();
                int width = image.width();
                for(int y=0; y<height; y+=4) {
                    QRgb * rgb = (QRgb *) image.scanLine(y);
                    for(int x=0; x<width; x+=4) {
                        average += 0.2126*qRed(rgb[x]) + 0.7152*qGreen(rgb[x]) + 0.0722*qBlue(rgb[x]);
                    }
                }
                average /= (image.width()/4) * (image.height()/4);
                use_light_color = average < 100;
            }
        }

        // Determine base color strategy: In dark theme, use light colors; in light theme, check icon brightness
        bool useLightColors = ThemeManager::isDarkThemeActive() || use_light_color;
        
        switch(user->flags%4) {
        default:
        case 0:
            if(useLightColors)
                item->setForeground(QColor(255, 255, 255));  // White
            else
                item->setForeground(QColor(0, 0, 0));  // Black
            break;
        case 1:
            if(useLightColors)
                item->setForeground(QColor(180, 180, 180));  // Light gray (darker than white for contrast)
            else
                item->setForeground(QColor(100, 100, 100));  // Dark gray
            break;
        case 2:
            if(useLightColors)
                item->setForeground(QColor(255, 100, 100));  // Light red (brighter for visibility)
            else
                item->setForeground(QColor(180, 0, 0));  // Dark red (brighter than original)
            break;
        case 3:
            if(useLightColors)
                item->setForeground(QColor(255, 150, 255));  // Light magenta (brighter for visibility)
            else
                item->setForeground(QColor(180, 0, 180));  // Dark magenta (brighter than original)
            break;
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
    auto& settings = SettingsManager::instance();
    font.setFamily(settings.value(QString::fromUtf8("fontFamily"), QString::fromUtf8("MS Shell Dlg2")).toString());
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
    chatWidget->setEncodingLabel(settings.value(QString::fromUtf8("EncodingName"), QString::fromUtf8("Mac OS Roman")).toString());

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
    this->onUserListChanged();
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
    qDebug() << "Current version: " << QString::fromUtf8("%1.%2").arg(VERSION_MAJOR).arg(VERSION_MINOR);
    qDebug() << "Latest version: " << versionArray;

    QList<QByteArray> splitArray = versionArray.split('.');
    if(splitArray[0].toInt() > VERSION_MAJOR || splitArray[1].toInt() > VERSION_MINOR)
    {
        qDebug() << "Update needed.";
        switch(QMessageBox::information(this, QString::fromUtf8("Update available"),
                                        QString::fromUtf8("There is a new version of Obsession ready for download."
                                        " Do you want to launch your web browser to download it?"), 
                                        QString::fromUtf8("Yes"), 
                                        QString::fromUtf8("No")))
        {
            case 0:
            QDesktopServices::openUrl(QUrl(QString::fromUtf8("")));
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
