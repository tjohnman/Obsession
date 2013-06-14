#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QThreadPool>
#include <iostream>

void kill_threads( void );

void messageOutput(QtMsgType type, const char *msg) {
    QSettings settings("mir", "Contra");
    if(settings.value("debugging", false).toBool()) {
        FILE *debugLog;
        debugLog = fopen("log.txt", "a");
        fwrite(QTime::currentTime().toString("hh:mm:ss:zzz' - '").toAscii(), 15, 1, debugLog);
        fwrite(msg, 1, strlen(msg), debugLog);
        fwrite("\n", 1, 1, debugLog);
        fclose(debugLog);
    } else {
        std::cout << msg << "\n";
    }
}

int main(int argc, char *argv[])
{
    FILE *debugLog;
    debugLog = fopen("log.txt", "a");
    fwrite("\n", 1, 1, debugLog);
    fclose(debugLog);

    QApplication a(argc, argv);
    MainWindow w;
    //qInstallMsgHandler(&messageOutput);
    w.show();
    w.openNewConnectionDialog();
    int r = a.exec();
    QThreadPool::globalInstance()->waitForDone(); // Wait for all threads to close
    return r;
}
