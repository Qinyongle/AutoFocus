/****************************************************************************
** Meta object code from reading C++ file 'gps_tcpserver.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../tcp/gps_tcpserver.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gps_tcpserver.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Gps_TcpServer_t {
    QByteArrayData data[15];
    char stringdata0[199];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Gps_TcpServer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Gps_TcpServer_t qt_meta_stringdata_Gps_TcpServer = {
    {
QT_MOC_LITERAL(0, 0, 13), // "Gps_TcpServer"
QT_MOC_LITERAL(1, 14, 9), // "send_DATA"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 8), // "send_CTL"
QT_MOC_LITERAL(4, 34, 11), // "SendGpsData"
QT_MOC_LITERAL(5, 46, 15), // "TcpServerListen"
QT_MOC_LITERAL(6, 62, 17), // "newConnectionSlot"
QT_MOC_LITERAL(7, 80, 13), // "readyReadSlot"
QT_MOC_LITERAL(8, 94, 8), // "SendData"
QT_MOC_LITERAL(9, 103, 4), // "data"
QT_MOC_LITERAL(10, 108, 12), // "DisConnected"
QT_MOC_LITERAL(11, 121, 13), // "delete_socket"
QT_MOC_LITERAL(12, 135, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(13, 164, 18), // "socket_State_check"
QT_MOC_LITERAL(14, 183, 15) // "Tcp_State_check"

    },
    "Gps_TcpServer\0send_DATA\0\0send_CTL\0"
    "SendGpsData\0TcpServerListen\0"
    "newConnectionSlot\0readyReadSlot\0"
    "SendData\0data\0DisConnected\0delete_socket\0"
    "QAbstractSocket::SocketError\0"
    "socket_State_check\0Tcp_State_check"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Gps_TcpServer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x06 /* Public */,
       3,    1,   70,    2, 0x06 /* Public */,
       4,    4,   73,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    2,   82,    2, 0x0a /* Public */,
       6,    0,   87,    2, 0x0a /* Public */,
       7,    0,   88,    2, 0x0a /* Public */,
       8,    1,   89,    2, 0x0a /* Public */,
      10,    0,   92,    2, 0x0a /* Public */,
      11,    1,   93,    2, 0x0a /* Public */,
      13,    0,   96,    2, 0x0a /* Public */,
      14,    0,   97,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Float,    2,
    QMetaType::Void, QMetaType::Double, QMetaType::Double, QMetaType::Float, QMetaType::Float,    2,    2,    2,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    2,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 12,    2,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Gps_TcpServer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Gps_TcpServer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->send_DATA(); break;
        case 1: _t->send_CTL((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 2: _t->SendGpsData((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3])),(*reinterpret_cast< float(*)>(_a[4]))); break;
        case 3: _t->TcpServerListen((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->newConnectionSlot(); break;
        case 5: _t->readyReadSlot(); break;
        case 6: _t->SendData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->DisConnected(); break;
        case 8: _t->delete_socket((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 9: _t->socket_State_check(); break;
        case 10: _t->Tcp_State_check(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Gps_TcpServer::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Gps_TcpServer::send_DATA)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Gps_TcpServer::*)(float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Gps_TcpServer::send_CTL)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Gps_TcpServer::*)(double , double , float , float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Gps_TcpServer::SendGpsData)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Gps_TcpServer::staticMetaObject = { {
    &QTcpServer::staticMetaObject,
    qt_meta_stringdata_Gps_TcpServer.data,
    qt_meta_data_Gps_TcpServer,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Gps_TcpServer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Gps_TcpServer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Gps_TcpServer.stringdata0))
        return static_cast<void*>(this);
    return QTcpServer::qt_metacast(_clname);
}

int Gps_TcpServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTcpServer::qt_metacall(_c, _id, _a);
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
void Gps_TcpServer::send_DATA()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void Gps_TcpServer::send_CTL(float _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Gps_TcpServer::SendGpsData(double _t1, double _t2, float _t3, float _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
