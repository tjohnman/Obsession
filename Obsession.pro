# -------------------------------------------------
# Project created by QtCreator 2010-03-23T06:24:26
# -------------------------------------------------
QT += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia
CONFIG -= exceptions rtti
CONFIG += resources_big
TARGET = Obsession
TEMPLATE = app
SOURCES += src/main.cpp \
    src/mainwindow.cpp \
    src/ctransaction.cpp \
    src/connectioncontroller.cpp \
    src/dialogopenconnection.cpp \
    src/dialogpreferences.cpp \
    src/dialogagreement.cpp \
    src/listwidgetusers.cpp \
    src/dialogprivatemessaging.cpp \
    src/dialogfilebrowser.cpp \
    src/dialogtrackers.cpp \
    src/dialogaddtracker.cpp \
    src/dialogerror.cpp \
    src/dialogiconviewer.cpp \
    src/dialoglinearnews.cpp \
    src/chatlineedit.cpp \
    src/downloadmanager.cpp \
    src/dialogdownloadqueue.cpp \
    src/dialogabout.cpp \
    src/widgetchat.cpp \
    src/widgetnews.cpp \
    src/dialogbookmarks.cpp \
    src/cdownload.cpp \
    src/widgetdownloaditem.cpp \
    src/uploadmanager.cpp \
    src/cupload.cpp \
    src/dialoguploadsqueue.cpp \
    src/widgetconsole.cpp \
    src/dialoguserinfo.cpp \
    src/threadiconloader.cpp \
    src/threaduploader.cpp \
    src/dialogprivileges.cpp \
    src/dialogrequestaccount.cpp \
    src/dialogcreateaccount.cpp \
    src/dialogbroadcast.cpp \
    src/dialogpostlinearnews.cpp
HEADERS += src/mainwindow.h \
    src/ctransaction.h \
    src/defines.h \
    src/connectioncontroller.h \
    src/dialogopenconnection.h \
    src/dialogpreferences.h \
    src/dialogagreement.h \
    src/listwidgetusers.h \
    src/dialogprivatemessaging.h \
    src/dialogfilebrowser.h \
    src/dialogtrackers.h \
    src/dialogaddtracker.h \
    src/dialogerror.h \
    src/dialogiconviewer.h \
    src/dialoglinearnews.h \
    src/chatlineedit.h \
    src/downloadmanager.h \
    src/dialogdownloadqueue.h \
    src/dialogabout.h \
    src/widgetchat.h \
    src/widgetnews.h \
    src/dialogbookmarks.h \
    src/cdownload.h \
    src/widgetdownloaditem.h \
    src/uploadmanager.h \
    src/cupload.h \
    src/dialoguploadsqueue.h \
    src/widgetconsole.h \
    src/dialoguserinfo.h \
    src/threadiconloader.h \
    src/version.h \
    src/threaduploader.h \
    src/TextHelper.h \
    src/dialogprivileges.h \
    src/dialogrequestaccount.h \
    src/dialogcreateaccount.h \
    src/dialogbroadcast.h \
    src/dialogpostlinearnews.h
FORMS += src/mainwindow.ui \
    src/dialogopenconnection.ui \
    src/dialogpreferences.ui \
    src/dialogagreement.ui \
    src/dialogprivatemessaging.ui \
    src/dialogfilebrowser.ui \
    src/dialogtrackers.ui \
    src/dialogaddtracker.ui \
    src/dialogerror.ui \
    src/dialogiconviewer.ui \
    src/dialoglinearnews.ui \
    src/dialogdownloadqueue.ui \
    src/dialogabout.ui \
    src/widgetchat.ui \
    src/widgetnews.ui \
    src/dialogbookmarks.ui \
    src/widgetdownloaditem.ui \
    src/dialoguploadsqueue.ui \
    src/widgetconsole.ui \
    src/dialoguserinfo.ui \
    src/dialogprivileges.ui \
    src/dialogrequestaccount.ui \
    src/dialogcreateaccount.ui \
    src/dialogbroadcast.ui \
    src/dialogpostlinearnews.ui
INCLUDEPATH += src/
RESOURCES += resources/resources.qrc
RC_ICONS = resources/icon.ico
ICON = resources/icon.icns
