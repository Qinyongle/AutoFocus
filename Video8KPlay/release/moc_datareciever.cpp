/****************************************************************************
** Meta object code from reading C++ file 'datareciever.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../threads/data/datareciever.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'datareciever.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DataReciever_t {
    QByteArrayData data[7];
    char stringdata0[63];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DataReciever_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DataReciever_t qt_meta_stringdata_DataReciever = {
    {
QT_MOC_LITERAL(0, 0, 12), // "DataReciever"
QT_MOC_LITERAL(1, 13, 11), // "OrignalData"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 11), // "resultReady"
QT_MOC_LITERAL(4, 38, 9), // "dataReady"
QT_MOC_LITERAL(5, 48, 8), // "DoRecive"
QT_MOC_LITERAL(6, 57, 5) // "bytes"

    },
    "DataReciever\0OrignalData\0\0resultReady\0"
    "dataReady\0DoRecive\0bytes"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DataReciever[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,
       1,    1,   42,    2, 0x06 /* Public */,
       3,    1,   45,    2, 0x06 /* Public */,
       4,    0,   48,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   49,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QByteArray,    2,
    QMetaType::Void, QMetaType::QByteArray,    2,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QByteArray,    6,

       0        // eod
};

void DataReciever::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DataReciever *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->OrignalData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->OrignalData((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 2: _t->resultReady((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 3: _t->dataReady(); break;
        case 4: _t->DoRecive((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (DataReciever::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DataReciever::OrignalData)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (DataReciever::*)(QByteArray );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DataReciever::OrignalData)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (DataReciever::*)(QByteArray );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DataReciever::resultReady)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (DataReciever::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DataReciever::dataReady)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DataReciever::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_DataReciever.data,
    qt_meta_data_DataReciever,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *DataReciever::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DataReciever::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DataReciever.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int DataReciever::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void DataReciever::OrignalData(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DataReciever::OrignalData(QByteArray _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DataReciever::resultReady(QByteArray _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DataReciever::dataReady()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
