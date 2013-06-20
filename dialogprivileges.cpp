#include "dialogprivileges.h"
#include "ui_dialogprivileges.h"

#include "connectioncontroller.h"
#include "TextHelper.h"

DialogPrivileges::DialogPrivileges(ConnectionController * c, QString user, QString password,
                                   quint8 p1, quint8 p2, quint8 p3, quint8 p4, quint8 p5, quint8 p6, quint8 p7, quint8 p8,
                                   QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPrivileges)
{
    ui->setupUi(this);
    connection = c;
    pPrivileges1 = p1;
    pPrivileges2 = p2;
    pPrivileges3 = p3;
    pPrivileges4 = p4;
    pPrivileges5 = p5;
    pPrivileges6 = p6;
    pPrivileges7 = p7;
    pPrivileges8 = p8;
    ui->editLogin->setText(user);
    if(password.data()[0].toLatin1() == 0) password = "";
    ui->editPassword->setText(password);

    qDebug() << "Permissions for " << user;
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
    pPrivileges1 = 0;
    pPrivileges2 = 0;
    pPrivileges3 = 0;
    pPrivileges4 = 0;
    pPrivileges5 = 0;
    pPrivileges6 = 0;
    pPrivileges7 = 0;
    pPrivileges8 = 0;
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
    if(pPrivileges1 & 128) ui->checkDeleteFile->setChecked(true);
    if(pPrivileges1 & 64) ui->checkUploadFile->setChecked(true);
    if(pPrivileges1 & 32) ui->checkDownloadFile->setChecked(true);
    if(pPrivileges1 & 16) ui->checkRenameFile->setChecked(true);
    if(pPrivileges1 & 8) ui->checkMoveFile->setChecked(true);
    if(pPrivileges1 & 4) ui->checkCreateFolders->setChecked(true);
    if(pPrivileges1 & 2) ui->checkDeleteFolders->setChecked(true);
    if(pPrivileges1 & 1) ui->checkRenameFolders->setChecked(true);
    if(pPrivileges2 & 128) ui->checkMoveFolder->setChecked(true);
    if(pPrivileges2 & 64) ui->checkReadChat->setChecked(true);
    if(pPrivileges2 & 32) ui->checkSendChat->setChecked(true);
    if(pPrivileges2 & 16) ui->checkOpenChat->setChecked(true);
    if(pPrivileges2 & 8) ui->checkCloseChat->setChecked(true);
    if(pPrivileges2 & 4) ui->checkShowInList->setChecked(true);
    if(pPrivileges2 & 2) ui->checkCreateUsers->setChecked(true);
    if(pPrivileges2 & 1) ui->checkDeleteUsers->setChecked(true);
    if(pPrivileges3 & 128) ui->checkOpenUsers->setChecked(true);
    if(pPrivileges3 & 64) ui->checkModifyUsers->setChecked(true);
    if(pPrivileges3 & 32) ui->checkChangeOwnPassword->setChecked(true);
    if(pPrivileges3 & 16) ui->checkSendPM->setChecked(true);
    if(pPrivileges3 & 8) ui->checkReadNews->setChecked(true);
    if(pPrivileges3 & 4) ui->checkPostNews->setChecked(true);
    if(pPrivileges3 & 2) ui->checkKickUsers->setChecked(true);
    if(pPrivileges3 & 1) ui->checkCannotBeDisconnected->setChecked(true);
    if(pPrivileges4 & 128) ui->checkGetUsersInfo->setChecked(true);
    if(pPrivileges4 & 64) ui->checkUploadAnywhere->setChecked(true);
    if(pPrivileges4 & 32) ui->checkAnyName->setChecked(true);
    if(pPrivileges4 & 16) ui->checkNoAgreement->setChecked(true);
    if(pPrivileges4 & 8) ui->checkSetFileComments->setChecked(true);
    if(pPrivileges4 & 4) ui->checkSetFolderComments->setChecked(true);
    if(pPrivileges4 & 2) ui->checkViewDropboxes->setChecked(true);
    if(pPrivileges4 & 1) ui->checkMakeAlias->setChecked(true);
    if(pPrivileges5 & 128) ui->checkBroadcast->setChecked(true);
    if(pPrivileges5 & 64) ui->checkDeleteNewsArticle->setChecked(true);
    if(pPrivileges5 & 32) ui->checkCreateNewsCategory->setChecked(true);
    if(pPrivileges5 & 16) ui->checkDeleteNewsCategory->setChecked(true);
    if(pPrivileges5 & 8) ui->checkCreateNewsFolder->setChecked(true);
    if(pPrivileges5 & 4) ui->checkDeleteNewsFolder->setChecked(true);
}

void DialogPrivileges::onAccepted()
{
    quint8 p1=0, p2=0, p3=0, p4=0, p5=0, p6=0, p7=0, p8=0;
    if(ui->checkDeleteFile->isChecked())            p1 |= 128;
    if(ui->checkUploadFile->isChecked())            p1 |= 64;
    if(ui->checkDownloadFile->isChecked())          p1 |= 32;
    if(ui->checkRenameFile->isChecked())            p1 |= 16;
    if(ui->checkMoveFile->isChecked())              p1 |= 8;
    if(ui->checkCreateFolders->isChecked())         p1 |= 4;
    if(ui->checkDeleteFolders->isChecked())         p1 |= 2;
    if(ui->checkRenameFolders->isChecked())         p1 |= 1;
    if(ui->checkMoveFolder->isChecked())            p2 |= 128;
    if(ui->checkReadChat->isChecked())              p2 |= 64;
    if(ui->checkSendChat->isChecked())              p2 |= 32;
    if(ui->checkOpenChat->isChecked())              p2 |= 16;
    if(ui->checkCloseChat->isChecked())             p2 |= 8;
    if(ui->checkShowInList->isChecked())            p2 |= 4;
    if(ui->checkCreateUsers->isChecked())           p2 |= 2;
    if(ui->checkDeleteUsers->isChecked())           p2 |= 1;
    if(ui->checkOpenUsers->isChecked())             p3 |= 128;
    if(ui->checkModifyUsers->isChecked())           p3 |= 64;
    if(ui->checkChangeOwnPassword->isChecked())     p3 |= 32;
    if(ui->checkSendPM->isChecked())                p3 |= 16;
    if(ui->checkReadNews->isChecked())              p3 |= 8;
    if(ui->checkPostNews->isChecked())              p3 |= 4;
    if(ui->checkKickUsers->isChecked())             p3 |= 2;
    if(ui->checkCannotBeDisconnected->isChecked())  p3 |= 1;
    if(ui->checkGetUsersInfo->isChecked())          p4 |= 128;
    if(ui->checkUploadAnywhere->isChecked())        p4 |= 64;
    if(ui->checkAnyName->isChecked())               p4 |= 32;
    if(ui->checkNoAgreement->isChecked())           p4 |= 16;
    if(ui->checkSetFileComments->isChecked())       p4 |= 8;
    if(ui->checkSetFolderComments->isChecked())     p4 |= 4;
    if(ui->checkViewDropboxes->isChecked())         p4 |= 2;
    if(ui->checkMakeAlias->isChecked())             p4 |= 1;
    if(ui->checkBroadcast->isChecked())             p5 |= 128;
    if(ui->checkDeleteNewsArticle->isChecked())     p5 |= 64;
    if(ui->checkCreateNewsCategory->isChecked())    p5 |= 32;
    if(ui->checkDeleteNewsCategory->isChecked())    p5 |= 16;
    if(ui->checkCreateNewsFolder->isChecked())      p5 |= 8;
    if(ui->checkDeleteNewsFolder->isChecked())      p5 |= 4;

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

    quint8 privs[8];
    memset(privs, 0, 8);
    memcpy(&privs[0], (char*)&p1, 1);
    memcpy(&privs[1], (char*)&p2, 1);
    memcpy(&privs[2], (char*)&p3, 1);
    memcpy(&privs[3], (char*)&p4, 1);
    memcpy(&privs[4], (char*)&p5, 1);
    memcpy(&privs[5], (char*)&p6, 1);
    memcpy(&privs[6], (char*)&p7, 1);
    memcpy(&privs[7], (char*)&p8, 1);
    trans->addParameter(105, login.size(), login.data());
    trans->addParameter(106, pass.size(), pass.data());
    trans->addParameter(102, login.size(), login.data());
    trans->addParameter(110, 8, (char*)&privs);
    connection->sendTransaction(trans);
}
