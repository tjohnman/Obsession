#define _CRT_SECURE_NO_WARNINGS
#define QT_NO_CAST_FROM_ASCII

#include <QtWidgets/QApplication>
#include "mainwindow.h"
#include <QThreadPool>
#include <iostream>
#include <QSettings>
#include "connectioncontroller.h"

void kill_threads( void );
void * __gxx_personality_v0=0;

qint32 main(qint32 argc, char *argv[])
{
    FILE * debugLog = fopen("log.txt", "a");
    if(debugLog)
    {
        fwrite("\n", 1, 1, debugLog);
        fclose(debugLog);
    }

    QApplication a(argc, argv);
    MainWindow w(0, true);

    w.show();

    QSettings settings(QString::fromUtf8("mir"), QString::fromUtf8("Contra"));
    qint32 autoBookmark = settings.value(QString::fromUtf8("autoBookmark"), -1).toInt();
    qint32 bookmarkCount = settings.value(QString::fromUtf8("bookmarkCount"), 0).toInt();
    if(autoBookmark > -1 && autoBookmark < bookmarkCount)
    {
        w.connection->connectToServer(settings.value(QString::fromUtf8("bookmarkaddress")+QString::number(autoBookmark), QString::fromUtf8("localhost")).toString(),
                                      settings.value(QString::fromUtf8("bookmarklogin")+QString::number(autoBookmark), QString::fromUtf8("")).toString(),
                                      settings.value(QString::fromUtf8("bookmarkpassword")+QString::number(autoBookmark), QString::fromUtf8("")).toString());
    }
    else
    {
        w.openNewConnectionDialog();
    }
    qint32 r = a.exec();
    QThreadPool::globalInstance()->waitForDone(); // Wait for all threads to close
    return r;
}
