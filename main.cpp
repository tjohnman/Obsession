#define _CRT_SECURE_NO_WARNINGS

#include <QtWidgets/QApplication>
#include "mainwindow.h"
#include <QThreadPool>
#include <iostream>
#include <QTextCodec>
#include <QSettings>

void kill_threads( void );

qint32 main(qint32 argc, char *argv[])
{
    FILE * debugLog = fopen("log.txt", "a");
    if(debugLog)
    {
        fwrite("\n", 1, 1, debugLog);
        fclose(debugLog);
    }

    QSettings settings("mir", "contra");
    if(settings.value("JapaneseMode", true).toBool()) {
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("Shift-JIS"));
        settings.setValue("JapaneseMode", true);
    } else {
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("Apple Roman"));
        settings.setValue("JapaneseMode", false);
    }

    QApplication a(argc, argv);
    MainWindow w(0, true);

    w.show();
    w.openNewConnectionDialog();
    qint32 r = a.exec();
    QThreadPool::globalInstance()->waitForDone(); // Wait for all threads to close
    return r;
}
