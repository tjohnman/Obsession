#include "dialogprivileges.h"
#include "ui_dialogprivileges.h"

#include "connectioncontroller.h"
#include "AccessTranslator.h"
#include "TextHelper.h"

DialogPrivileges::DialogPrivileges(ConnectionController * c, QString user, QString password, quint64 privs, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPrivileges)
{
    ui->setupUi(this);
    connection = c;
    pPrivileges = privs;
    ui->editLogin->setText(user);
    if(password.data()[0].toLatin1() == 0) password = "";
    ui->editPassword->setText(password);

    qDebug() << "Permissions for " << user << " " << privs;
    connect(this, SIGNAL(accepted()), this, SLOT(onAccepted()));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

DialogPrivileges::DialogPrivileges(ConnectionController * c, QString user, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPrivileges)
{
    ui->setupUi(this);
    connection = c;
    pPrivileges = 0;
    ui->editLogin->setText(user);


    connect(this, SIGNAL(accepted()), this, SLOT(onAccepted()));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

DialogPrivileges::~DialogPrivileges()
{
    delete ui;
}

void DialogPrivileges::readPrivileges()
{
    if(AccessTranslator::HavePrivileges(pPrivileges, AccessTranslator::DELETE_FILE)) ui->checkDeleteFile->setChecked(true);
    if(AccessTranslator::HavePrivileges(pPrivileges, AccessTranslator::UPLOAD_FILE)) ui->checkUploadFile->setChecked(true);
    if(AccessTranslator::HavePrivileges(pPrivileges, AccessTranslator::DOWNLOAD_FILE)) ui->checkDownloadFile->setChecked(true);
    if(AccessTranslator::HavePrivileges(pPrivileges, AccessTranslator::RENAME_FILE)) ui->checkRenameFile->setChecked(true);
    if(AccessTranslator::HavePrivileges(pPrivileges, AccessTranslator::MOVE_FILE)) ui->checkMoveFile->setChecked(true);
    if(AccessTranslator::HavePrivileges(pPrivileges, AccessTranslator::CREATE_FOLDER)) ui->checkCreateFolders->setChecked(true);
    if(AccessTranslator::HavePrivileges(pPrivileges, AccessTranslator::DELETE_FOLDER)) ui->checkDeleteFolders->setChecked(true);
    if(AccessTranslator::HavePrivileges(pPrivileges, AccessTranslator::RENAME_FOLDER)) ui->checkRenameFolders->setChecked(true);
    if(AccessTranslator::HavePrivileges(pPrivileges, AccessTranslator::MOVE_FOLDER)) ui->checkMoveFolder->setChecked(true);
    if(AccessTranslator::HavePrivileges(pPrivileges, AccessTranslator::READ_CHAT)) ui->checkReadChat->setChecked(true);
    if(AccessTranslator::HavePrivileges(pPrivileges, AccessTranslator::SEND_CHAT)) ui->checkSendChat->setChecked(true);
    if(AccessTranslator::HavePrivileges(pPrivileges, AccessTranslator::OPEN_CHAT)) ui->checkOpenChat->setChecked(true);
    if(AccessTranslator::HavePrivileges(pPrivileges, AccessTranslator::CLOSE_CHAT)) ui->checkCloseChat->setChecked(true);
    if(AccessTranslator::HavePrivileges(pPrivileges, AccessTranslator::SHOW_IN_LIST)) ui->checkShowInList->setChecked(true);
    if(AccessTranslator::HavePrivileges(pPrivileges, AccessTranslator::CREATE_USER)) ui->checkCreateUsers->setChecked(true);
    if(AccessTranslator::HavePrivileges(pPrivileges, AccessTranslator::DELETE_USER)) ui->checkDeleteUsers->setChecked(true);
    if(AccessTranslator::HavePrivileges(pPrivileges, AccessTranslator::OPEN_USER)) ui->checkOpenUsers->setChecked(true);
    if(AccessTranslator::HavePrivileges(pPrivileges, AccessTranslator::MODIFY_USER)) ui->checkModifyUsers->setChecked(true);
    if(AccessTranslator::HavePrivileges(pPrivileges, AccessTranslator::CHANGE_OWN_PASSWORD)) ui->checkChangeOwnPassword->setChecked(true);
    if(AccessTranslator::HavePrivileges(pPrivileges, AccessTranslator::SEND_PRIV_MESSAGE)) ui->checkSendPM->setChecked(true);
    if(AccessTranslator::HavePrivileges(pPrivileges, AccessTranslator::READ_NEWS)) ui->checkReadNews->setChecked(true);
    if(AccessTranslator::HavePrivileges(pPrivileges, AccessTranslator::POST_NEWS)) ui->checkPostNews->setChecked(true);
    if(AccessTranslator::HavePrivileges(pPrivileges, AccessTranslator::DISCONNECT_USER)) ui->checkKickUsers->setChecked(true);
    if(AccessTranslator::HavePrivileges(pPrivileges, AccessTranslator::CANNOT_BE_DISCONNECTED)) ui->checkCannotBeDisconnected->setChecked(true);
    if(AccessTranslator::HavePrivileges(pPrivileges, AccessTranslator::GET_CLIENT_INFO)) ui->checkGetUsersInfo->setChecked(true);
    if(AccessTranslator::HavePrivileges(pPrivileges, AccessTranslator::UPLOAD_ANYWHERE)) ui->checkUploadAnywhere->setChecked(true);
    if(AccessTranslator::HavePrivileges(pPrivileges, AccessTranslator::ANY_NAME)) ui->checkAnyName->setChecked(true);
    if(AccessTranslator::HavePrivileges(pPrivileges, AccessTranslator::NO_AGREEMENT)) ui->checkNoAgreement->setChecked(true);
    if(AccessTranslator::HavePrivileges(pPrivileges, AccessTranslator::SET_FILE_COMMENT)) ui->checkSetFileComments->setChecked(true);
    if(AccessTranslator::HavePrivileges(pPrivileges, AccessTranslator::SET_FOLDER_COMMENT)) ui->checkSetFolderComments->setChecked(true);
    if(AccessTranslator::HavePrivileges(pPrivileges, AccessTranslator::VIEW_DROPBOXES)) ui->checkViewDropboxes->setChecked(true);
    if(AccessTranslator::HavePrivileges(pPrivileges, AccessTranslator::MAKE_ALIAS)) ui->checkMakeAlias->setChecked(true);
    if(AccessTranslator::HavePrivileges(pPrivileges, AccessTranslator::BROADCAST)) ui->checkBroadcast->setChecked(true);
    if(AccessTranslator::HavePrivileges(pPrivileges, AccessTranslator::DELETE_NEWS_ARTICLE)) ui->checkDeleteNewsArticle->setChecked(true);
    if(AccessTranslator::HavePrivileges(pPrivileges, AccessTranslator::CREATE_NEWS_CATEGORY)) ui->checkCreateNewsCategory->setChecked(true);
    if(AccessTranslator::HavePrivileges(pPrivileges, AccessTranslator::DELETE_NEWS_CATEGORY)) ui->checkDeleteNewsCategory->setChecked(true);
    if(AccessTranslator::HavePrivileges(pPrivileges, AccessTranslator::CREATE_FOLDER_NEWS)) ui->checkCreateNewsFolder->setChecked(true);
    if(AccessTranslator::HavePrivileges(pPrivileges, AccessTranslator::DELETE_FOLDER_NEWS)) ui->checkDeleteNewsFolder->setChecked(true);
}

void DialogPrivileges::onAccepted()
{
    quint64 privs = 0;
    if(ui->checkDeleteFile->isChecked()) privs |= AccessTranslator::DELETE_FILE;
    if(ui->checkUploadFile->isChecked()) privs |= AccessTranslator::UPLOAD_FILE;
    if(ui->checkDownloadFile->isChecked()) privs |= AccessTranslator::DOWNLOAD_FILE;
    if(ui->checkRenameFile->isChecked()) privs |= AccessTranslator::RENAME_FILE;
    if(ui->checkMoveFile->isChecked()) privs |= AccessTranslator::MOVE_FILE;
    if(ui->checkCreateFolders->isChecked()) privs |= AccessTranslator::CREATE_FOLDER;
    if(ui->checkDeleteFolders->isChecked()) privs |= AccessTranslator::DELETE_FOLDER;
    if(ui->checkRenameFolders->isChecked()) privs |= AccessTranslator::RENAME_FOLDER;
    if(ui->checkMoveFolder->isChecked()) privs |= AccessTranslator::MOVE_FOLDER;
    if(ui->checkReadChat->isChecked()) privs |= AccessTranslator::READ_CHAT;
    if(ui->checkSendChat->isChecked()) privs |= AccessTranslator::SEND_CHAT;
    if(ui->checkOpenChat->isChecked()) privs |= AccessTranslator::OPEN_CHAT;
    if(ui->checkCloseChat->isChecked()) privs |= AccessTranslator::CLOSE_CHAT;
    if(ui->checkShowInList->isChecked()) privs |= AccessTranslator::SHOW_IN_LIST;
    if(ui->checkCreateUsers->isChecked()) privs |= AccessTranslator::CREATE_USER;
    if(ui->checkDeleteUsers->isChecked()) privs |= AccessTranslator::DELETE_USER;
    if(ui->checkOpenUsers->isChecked()) privs |= AccessTranslator::OPEN_USER;
    if(ui->checkModifyUsers->isChecked()) privs |= AccessTranslator::MODIFY_USER;
    if(ui->checkChangeOwnPassword->isChecked()) privs |= AccessTranslator::CHANGE_OWN_PASSWORD;
    if(ui->checkSendPM->isChecked()) privs |= AccessTranslator::SEND_PRIV_MESSAGE;
    if(ui->checkReadNews->isChecked()) privs |= AccessTranslator::READ_NEWS;
    if(ui->checkPostNews->isChecked()) privs |= AccessTranslator::POST_NEWS;
    if(ui->checkKickUsers->isChecked()) privs |= AccessTranslator::DISCONNECT_USER;
    if(ui->checkCannotBeDisconnected->isChecked()) privs |= AccessTranslator::CANNOT_BE_DISCONNECTED;
    if(ui->checkGetUsersInfo->isChecked()) privs |= AccessTranslator::GET_CLIENT_INFO;
    if(ui->checkUploadAnywhere->isChecked()) privs |= AccessTranslator::UPLOAD_ANYWHERE;
    if(ui->checkAnyName->isChecked()) privs |= AccessTranslator::ANY_NAME;
    if(ui->checkNoAgreement->isChecked()) privs |= AccessTranslator::NO_AGREEMENT;
    if(ui->checkSetFileComments->isChecked()) privs |= AccessTranslator::SET_FILE_COMMENT;
    if(ui->checkSetFolderComments->isChecked()) privs |= AccessTranslator::SET_FOLDER_COMMENT;
    if(ui->checkViewDropboxes->isChecked()) privs |= AccessTranslator::VIEW_DROPBOXES;
    if(ui->checkMakeAlias->isChecked()) privs |= AccessTranslator::MAKE_ALIAS;
    if(ui->checkBroadcast->isChecked()) privs |= AccessTranslator::BROADCAST;
    if(ui->checkDeleteNewsArticle->isChecked()) privs |= AccessTranslator::DELETE_NEWS_ARTICLE;
    if(ui->checkCreateNewsCategory->isChecked()) privs |= AccessTranslator::CREATE_NEWS_CATEGORY;
    if(ui->checkDeleteNewsCategory->isChecked()) privs |= AccessTranslator::DELETE_NEWS_CATEGORY;
    if(ui->checkCreateNewsFolder->isChecked()) privs |= AccessTranslator::CREATE_FOLDER_NEWS;
    if(ui->checkDeleteNewsFolder->isChecked()) privs |= AccessTranslator::DELETE_FOLDER_NEWS;

    CTransaction * trans = connection->createTransaction(353);

    QByteArray login = TextHelper::EncodeText(ui->editLogin->text());
    for(int i=0; i<login.size(); ++i)
    {
        login[i] = 255 - login[i];
    }

    QByteArray pass = TextHelper::EncodeText(ui->editPassword->text());
    if(ui->editPassword->text().isEmpty())
    {
        pass = QByteArray("\0", 1);
    }
    else
    {
        for(int i=0; i<pass.size(); ++i)
        {
            pass[i] = 255 - pass[i];
        }
    }

    trans->addParameter(105, login.size(), login.data());
    trans->addParameter(106, pass.size(), pass.data());
    trans->addParameter(102, login.size(), login.data());
    trans->addParameter(110, 8, (char*)&privs);
    connection->sendTransaction(trans);

    emit savedPrivileges(privs);
}
