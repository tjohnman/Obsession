/****************************************************************************
** Meta object code from reading C++ file 'dialogopenconnection.h'
**
** Created: Wed Mar 24 22:22:48 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "dialogopenconnection.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dialogopenconnection.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DialogOpenConnection[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      22,   21,   21,   21, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DialogOpenConnection[] = {
    "DialogOpenConnection\0\0openConnection()\0"
};

const QMetaObject DialogOpenConnection::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_DialogOpenConnection,
      qt_meta_data_DialogOpenConnection, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DialogOpenConnection::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DialogOpenConnection::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DialogOpenConnection::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DialogOpenConnection))
        return static_cast<void*>(const_cast< DialogOpenConnection*>(this));
    return QDialog::qt_metacast(_clname);
}

int DialogOpenConnection::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: openConnection(); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
