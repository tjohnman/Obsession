/****************************************************************************
** Meta object code from reading C++ file 'connectioncontroller.h'
**
** Created: Wed Mar 24 22:22:47 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "connectioncontroller.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'connectioncontroller.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ConnectionController[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      22,   21,   21,   21, 0x05,
      42,   38,   21,   21, 0x05,
      70,   21,   21,   21, 0x05,

 // slots: signature, parameters, type, tag, flags
      88,   21,   21,   21, 0x0a,
     108,   21,   21,   21, 0x0a,
     152,   21,   21,   21, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ConnectionController[] = {
    "ConnectionController\0\0gotServerName()\0"
    "t,l\0gotChatMessage(char*,short)\0"
    "userListChanged()\0onSocketConnected()\0"
    "onSocketError(QAbstractSocket::SocketError)\0"
    "onSocketData()\0"
};

const QMetaObject ConnectionController::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ConnectionController,
      qt_meta_data_ConnectionController, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ConnectionController::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ConnectionController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ConnectionController::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ConnectionController))
        return static_cast<void*>(const_cast< ConnectionController*>(this));
    return QObject::qt_metacast(_clname);
}

int ConnectionController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: gotServerName(); break;
        case 1: gotChatMessage((*reinterpret_cast< char*(*)>(_a[1])),(*reinterpret_cast< short(*)>(_a[2]))); break;
        case 2: userListChanged(); break;
        case 3: onSocketConnected(); break;
        case 4: onSocketError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 5: onSocketData(); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void ConnectionController::gotServerName()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void ConnectionController::gotChatMessage(char * _t1, short _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ConnectionController::userListChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
QT_END_MOC_NAMESPACE
