#define _CRT_SECURE_NO_WARNINGS
#define QT_NO_CAST_FROM_ASCII

#include <QApplication>
#include "mainwindow.h"
#include <QThreadPool>
#include <iostream>
#include <QSettings>
#include "connectioncontroller.h"
#include "ThemeManager.h"

void kill_threads( void );
void * __gxx_personality_v0=0;

qint32 main(qint32 argc, char *argv[])
{
    QApplication a(argc, argv);
    
    // Load and apply saved theme preference
    AppTheme savedTheme = ThemeManager::loadThemePreference();
    ThemeManager::applyTheme(savedTheme);
    
    MainWindow w(0, true);

    w.show();
    w.autoConnect();

    qint32 r = a.exec();
    QThreadPool::globalInstance()->waitForDone(); // Wait for all threads to close
    return r;
}
