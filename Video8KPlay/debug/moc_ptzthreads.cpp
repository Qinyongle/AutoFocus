/****************************************************************************
** Meta object code from reading C++ file 'ptzthreads.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../threads/ptzthreads.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ptzthreads.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_PtzThreads_t {
    QByteArrayData data[27];
    char stringdata0[255];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PtzThreads_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PtzThreads_t qt_meta_stringdata_PtzThreads = {
    {
QT_MOC_LITERAL(0, 0, 10), // "PtzThreads"
QT_MOC_LITERAL(1, 11, 9), // "HVConnect"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 12), // "HVDisconnect"
QT_MOC_LITERAL(4, 35, 10), // "HVSendData"
QT_MOC_LITERAL(5, 46, 14), // "HVConnectState"
QT_MOC_LITERAL(6, 61, 17), // "ZoomeConnectState"
QT_MOC_LITERAL(7, 79, 6), // "HAngle"
QT_MOC_LITERAL(8, 86, 6), // "VAngle"
QT_MOC_LITERAL(9, 93, 9), // "ZoomAngle"
QT_MOC_LITERAL(10, 103, 11), // "HVConnected"
QT_MOC_LITERAL(11, 115, 14), // "HVDisconnected"
QT_MOC_LITERAL(12, 130, 14), // "ZoomeConnected"
QT_MOC_LITERAL(13, 145, 17), // "ZoomeDisconnected"
QT_MOC_LITERAL(14, 163, 3), // "Run"
QT_MOC_LITERAL(15, 167, 4), // "Quit"
QT_MOC_LITERAL(16, 172, 7), // "RRotate"
QT_MOC_LITERAL(17, 180, 7), // "LRotate"
QT_MOC_LITERAL(18, 188, 9), // "HAngleSet"
QT_MOC_LITERAL(19, 198, 7), // "URotate"
QT_MOC_LITERAL(20, 206, 7), // "DRotate"
QT_MOC_LITERAL(21, 214, 9), // "VAngleSet"
QT_MOC_LITERAL(22, 224, 10), // "RotateStop"
QT_MOC_LITERAL(23, 235, 3), // "ZIn"
QT_MOC_LITERAL(24, 239, 4), // "ZOut"
QT_MOC_LITERAL(25, 244, 4), // "ZSet"
QT_MOC_LITERAL(26, 249, 5) // "ZStop"

    },
    "PtzThreads\0HVConnect\0\0HVDisconnect\0"
    "HVSendData\0HVConnectState\0ZoomeConnectState\0"
    "HAngle\0VAngle\0ZoomAngle\0HVConnected\0"
    "HVDisconnected\0ZoomeConnected\0"
    "ZoomeDisconnected\0Run\0Quit\0RRotate\0"
    "LRotate\0HAngleSet\0URotate\0DRotate\0"
    "VAngleSet\0RotateStop\0ZIn\0ZOut\0ZSet\0"
    "ZStop"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PtzThreads[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      25,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,  139,    2, 0x06 /* Public */,
       3,    0,  144,    2, 0x06 /* Public */,
       4,    1,  145,    2, 0x06 /* Public */,
       5,    1,  148,    2, 0x06 /* Public */,
       6,    1,  151,    2, 0x06 /* Public */,
       7,    1,  154,    2, 0x06 /* Public */,
       8,    1,  157,    2, 0x06 /* Public */,
       9,    1,  160,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    2,  163,    2, 0x0a /* Public */,
      11,    2,  168,    2, 0x0a /* Public */,
      12,    2,  173,    2, 0x0a /* Public */,
      13,    2,  178,    2, 0x0a /* Public */,
      14,    0,  183,    2, 0x0a /* Public */,
      15,    0,  184,    2, 0x0a /* Public */,
      16,    0,  185,    2, 0x0a /* Public */,
      17,    0,  186,    2, 0x0a /* Public */,
      18,    1,  187,    2, 0x0a /* Public */,
      19,    0,  190,    2, 0x0a /* Public */,
      20,    0,  191,    2, 0x0a /* Public */,
      21,    1,  192,    2, 0x0a /* Public */,
      22,    0,  195,    2, 0x0a /* Public */,
      23,    0,  196,    2, 0x0a /* Public */,
      24,    0,  197,    2, 0x0a /* Public */,
      25,    1,  198,    2, 0x0a /* Public */,
      26,    0,  201,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    2,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Double,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    2,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    2,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    2,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    2,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void,

       0        // eod
};

void PtzThreads::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<PtzThreads *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->HVConnect((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->HVDisconnect(); break;
        case 2: _t->HVSendData((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 3: _t->HVConnectState((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->ZoomeConnectState((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->HAngle((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 6: _t->VAngle((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 7: _t->ZoomAngle((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 8: _t->HVConnected((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 9: _t->HVDisconnected((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 10: _t->ZoomeConnected((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 11: _t->ZoomeDisconnected((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 12: _t->Run(); break;
        case 13: _t->Quit(); break;
        case 14: _t->RRotate(); break;
        case 15: _t->LRotate(); break;
        case 16: _t->HAngleSet((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 17: _t->URotate(); break;
        case 18: _t->DRotate(); break;
        case 19: _t->VAngleSet((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 20: _t->RotateStop(); break;
        case 21: _t->ZIn(); break;
        case 22: _t->ZOut(); break;
        case 23: _t->ZSet((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 24: _t->ZStop(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (PtzThreads::*)(QString , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PtzThreads::HVConnect)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (PtzThreads::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PtzThreads::HVDisconnect)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (PtzThreads::*)(QByteArray );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PtzThreads::HVSendData)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (PtzThreads::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PtzThreads::HVConnectState)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (PtzThreads::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PtzThreads::ZoomeConnectState)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (PtzThreads::*)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PtzThreads::HAngle)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (PtzThreads::*)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PtzThreads::VAngle)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (PtzThreads::*)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PtzThreads::ZoomAngle)) {
                *result = 7;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject PtzThreads::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_PtzThreads.data,
    qt_meta_data_PtzThreads,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *PtzThreads::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PtzThreads::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_PtzThreads.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int PtzThreads::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 25)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 25;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 25)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 25;
    }
    return _id;
}

// SIGNAL 0
void PtzThreads::HVConnect(QString _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void PtzThreads::HVDisconnect()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void PtzThreads::HVSendData(QByteArray _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void PtzThreads::HVConnectState(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void PtzThreads::ZoomeConnectState(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void PtzThreads::HAngle(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void PtzThreads::VAngle(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void PtzThreads::ZoomAngle(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
