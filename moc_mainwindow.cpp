/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Wed Mar 24 22:22:46 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      38,   11,   11,   11, 0x0a,
      56,   11,   11,   11, 0x0a,
      78,   11,   11,   11, 0x0a,
      91,   11,   11,   11, 0x0a,
     114,   11,   11,   11, 0x0a,
     129,   11,   11,   11, 0x0a,
     149,  147,   11,   11, 0x0a,
     179,   11,   11,   11, 0x0a,
     199,   11,   11,   11, 0x0a,
     223,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0openNewConnectionDialog()\0"
    "openAboutDialog()\0slotQuitApplication()\0"
    "disconnect()\0openServerInfoDialog()\0"
    "slotSendChat()\0onGotServerName()\0,\0"
    "onGotChatMessage(char*,short)\0"
    "onUserListChanged()\0openPreferencesDialog()\0"
    "showServerAgreement()\0"
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: openNewConnectionDialog(); break;
        case 1: openAboutDialog(); break;
        case 2: slotQuitApplication(); break;
        case 3: disconnect(); break;
        case 4: openServerInfoDialog(); break;
        case 5: slotSendChat(); break;
        case 6: onGotServerName(); break;
        case 7: onGotChatMessage((*reinterpret_cast< char*(*)>(_a[1])),(*reinterpret_cast< short(*)>(_a[2]))); break;
        case 8: onUserListChanged(); break;
        case 9: openPreferencesDialog(); break;
        case 10: showServerAgreement(); break;
        default: ;
        }
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
