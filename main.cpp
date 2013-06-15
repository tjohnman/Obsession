#define _CRT_SECURE_NO_WARNINGS

#include <QtWidgets/QApplication>
#include "mainwindow.h"
#include <QThreadPool>
#include <iostream>

void kill_threads( void );

int main(int argc, char *argv[])
{
    FILE *debugLog;
    debugLog = fopen("log.txt", "a");
    fwrite("\n", 1, 1, debugLog);
    fclose(debugLog);

    QApplication a(argc, argv);
    MainWindow w;

    w.show();
    w.openNewConnectionDialog();
    int r = a.exec();
    QThreadPool::globalInstance()->waitForDone(); // Wait for all threads to close
    return r;
}
