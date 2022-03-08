/****************************************************************************
** Meta object code from reading C++ file 'gpsgetthread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../gpsgetthread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gpsgetthread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_GpsGetThread_t {
    QByteArrayData data[21];
    char stringdata0[201];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GpsGetThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GpsGetThread_t qt_meta_stringdata_GpsGetThread = {
    {
QT_MOC_LITERAL(0, 0, 12), // "GpsGetThread"
QT_MOC_LITERAL(1, 13, 10), // "ConnectCmd"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 13), // "DisConnectCmd"
QT_MOC_LITERAL(4, 39, 10), // "GpsPotData"
QT_MOC_LITERAL(5, 50, 7), // "GpsData"
QT_MOC_LITERAL(6, 58, 9), // "TcpListen"
QT_MOC_LITERAL(7, 68, 11), // "SendGpsData"
QT_MOC_LITERAL(8, 80, 12), // "GpsConnected"
QT_MOC_LITERAL(9, 93, 15), // "GpsDisConnected"
QT_MOC_LITERAL(10, 109, 7), // "Connect"
QT_MOC_LITERAL(11, 117, 16), // "ClientsConnected"
QT_MOC_LITERAL(12, 134, 8), // "clientID"
QT_MOC_LITERAL(13, 143, 2), // "ip"
QT_MOC_LITERAL(14, 146, 4), // "port"
QT_MOC_LITERAL(15, 151, 19), // "ClientsDisConnected"
QT_MOC_LITERAL(16, 171, 11), // "RecvGpsData"
QT_MOC_LITERAL(17, 183, 3), // "lng"
QT_MOC_LITERAL(18, 187, 3), // "lat"
QT_MOC_LITERAL(19, 191, 3), // "nav"
QT_MOC_LITERAL(20, 195, 5) // "hight"

    },
    "GpsGetThread\0ConnectCmd\0\0DisConnectCmd\0"
    "GpsPotData\0GpsData\0TcpListen\0SendGpsData\0"
    "GpsConnected\0GpsDisConnected\0Connect\0"
    "ClientsConnected\0clientID\0ip\0port\0"
    "ClientsDisConnected\0RecvGpsData\0lng\0"
    "lat\0nav\0hight"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GpsGetThread[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   69,    2, 0x06 /* Public */,
       3,    0,   74,    2, 0x06 /* Public */,
       4,    1,   75,    2, 0x06 /* Public */,
       6,    2,   78,    2, 0x06 /* Public */,
       7,    4,   83,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    2,   92,    2, 0x0a /* Public */,
       9,    2,   97,    2, 0x0a /* Public */,
      10,    2,  102,    2, 0x0a /* Public */,
      11,    3,  107,    2, 0x0a /* Public */,
      15,    3,  114,    2, 0x0a /* Public */,
      16,    4,  121,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    2,    2,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    2,    2,
    QMetaType::Void, QMetaType::Double, QMetaType::Double, QMetaType::Float, QMetaType::Float,    2,    2,    2,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    2,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    2,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    2,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::QString, QMetaType::Int,   12,   13,   14,
    QMetaType::Void, QMetaType::Int, QMetaType::QString, QMetaType::Int,   12,   13,   14,
    QMetaType::Void, QMetaType::Double, QMetaType::Double, QMetaType::Float, QMetaType::Float,   17,   18,   19,   20,

       0        // eod
};

void GpsGetThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<GpsGetThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ConnectCmd((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->DisConnectCmd(); break;
        case 2: _t->GpsPotData((*reinterpret_cast< GpsData(*)>(_a[1]))); break;
        case 3: _t->TcpListen((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->SendGpsData((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3])),(*reinterpret_cast< float(*)>(_a[4]))); break;
        case 5: _t->GpsConnected((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: _t->GpsDisConnected((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: _t->Connect((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 8: _t->ClientsConnected((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 9: _t->ClientsDisConnected((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 10: _t->RecvGpsData((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3])),(*reinterpret_cast< float(*)>(_a[4]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< GpsData >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (GpsGetThread::*)(QString , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GpsGetThread::ConnectCmd)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (GpsGetThread::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GpsGetThread::DisConnectCmd)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (GpsGetThread::*)(GpsData );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GpsGetThread::GpsPotData)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (GpsGetThread::*)(QString , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GpsGetThread::TcpListen)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (GpsGetThread::*)(double , double , float , float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GpsGetThread::SendGpsData)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject GpsGetThread::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_GpsGetThread.data,
    qt_meta_data_GpsGetThread,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *GpsGetThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GpsGetThread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_GpsGetThread.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int GpsGetThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void GpsGetThread::ConnectCmd(QString _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void GpsGetThread::DisConnectCmd()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void GpsGetThread::GpsPotData(GpsData _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void GpsGetThread::TcpListen(QString _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void GpsGetThread::SendGpsData(double _t1, double _t2, float _t3, float _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
