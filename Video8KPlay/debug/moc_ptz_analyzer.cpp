/****************************************************************************
** Meta object code from reading C++ file 'ptz_analyzer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../threads/ptz/ptz_analyzer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ptz_analyzer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_PtzAnalyzer_t {
    QByteArrayData data[7];
    char stringdata0[67];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PtzAnalyzer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PtzAnalyzer_t qt_meta_stringdata_PtzAnalyzer = {
    {
QT_MOC_LITERAL(0, 0, 11), // "PtzAnalyzer"
QT_MOC_LITERAL(1, 12, 10), // "DataPacket"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 9), // "ptzHAngle"
QT_MOC_LITERAL(4, 34, 9), // "ptzVAngle"
QT_MOC_LITERAL(5, 44, 9), // "DoAnalyze"
QT_MOC_LITERAL(6, 54, 12) // "setEquipType"

    },
    "PtzAnalyzer\0DataPacket\0\0ptzHAngle\0"
    "ptzVAngle\0DoAnalyze\0setEquipType"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PtzAnalyzer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,
       3,    1,   42,    2, 0x06 /* Public */,
       4,    1,   45,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   48,    2, 0x0a /* Public */,
       6,    1,   49,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Double,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void PtzAnalyzer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<PtzAnalyzer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->DataPacket((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->ptzHAngle((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: _t->ptzVAngle((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: _t->DoAnalyze(); break;
        case 4: _t->setEquipType((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (PtzAnalyzer::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PtzAnalyzer::DataPacket)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (PtzAnalyzer::*)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PtzAnalyzer::ptzHAngle)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (PtzAnalyzer::*)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PtzAnalyzer::ptzVAngle)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject PtzAnalyzer::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_PtzAnalyzer.data,
    qt_meta_data_PtzAnalyzer,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *PtzAnalyzer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PtzAnalyzer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_PtzAnalyzer.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int PtzAnalyzer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void PtzAnalyzer::DataPacket(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void PtzAnalyzer::ptzHAngle(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void PtzAnalyzer::ptzVAngle(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
