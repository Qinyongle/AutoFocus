/****************************************************************************
** Meta object code from reading C++ file 'videopicget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../ffmpeg/videopicget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'videopicget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_VideoPicGet_t {
    QByteArrayData data[13];
    char stringdata0[172];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VideoPicGet_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VideoPicGet_t qt_meta_stringdata_VideoPicGet = {
    {
QT_MOC_LITERAL(0, 0, 11), // "VideoPicGet"
QT_MOC_LITERAL(1, 12, 17), // "FramePositionSend"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 15), // "FrameMasageSend"
QT_MOC_LITERAL(4, 47, 11), // "SetSavePath"
QT_MOC_LITERAL(5, 59, 8), // "RunStart"
QT_MOC_LITERAL(6, 68, 30), // "TestFfmpegDecoder::DECODE_TYPE"
QT_MOC_LITERAL(7, 99, 7), // "RunStop"
QT_MOC_LITERAL(8, 107, 12), // "SetFrameTime"
QT_MOC_LITERAL(9, 120, 12), // "SetPlayState"
QT_MOC_LITERAL(10, 133, 16), // "SetPlayDelayTime"
QT_MOC_LITERAL(11, 150, 11), // "SetPlaySlot"
QT_MOC_LITERAL(12, 162, 9) // "GetPicCmd"

    },
    "VideoPicGet\0FramePositionSend\0\0"
    "FrameMasageSend\0SetSavePath\0RunStart\0"
    "TestFfmpegDecoder::DECODE_TYPE\0RunStop\0"
    "SetFrameTime\0SetPlayState\0SetPlayDelayTime\0"
    "SetPlaySlot\0GetPicCmd"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VideoPicGet[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x06 /* Public */,
       3,    2,   67,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    2,   72,    2, 0x0a /* Public */,
       5,    2,   77,    2, 0x0a /* Public */,
       7,    0,   82,    2, 0x0a /* Public */,
       8,    1,   83,    2, 0x0a /* Public */,
       9,    1,   86,    2, 0x0a /* Public */,
      10,    1,   89,    2, 0x0a /* Public */,
      11,    2,   92,    2, 0x0a /* Public */,
      12,    0,   97,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::Double,    2,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    2,    2,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 6,    2,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Long,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::ULong, QMetaType::ULong,    2,    2,
    QMetaType::Void,

       0        // eod
};

void VideoPicGet::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VideoPicGet *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->FramePositionSend((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->FrameMasageSend((*reinterpret_cast< unsigned long long(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 2: _t->SetSavePath((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 3: _t->RunStart((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< TestFfmpegDecoder::DECODE_TYPE(*)>(_a[2]))); break;
        case 4: _t->RunStop(); break;
        case 5: _t->SetFrameTime((*reinterpret_cast< long(*)>(_a[1]))); break;
        case 6: _t->SetPlayState((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->SetPlayDelayTime((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->SetPlaySlot((*reinterpret_cast< ulong(*)>(_a[1])),(*reinterpret_cast< ulong(*)>(_a[2]))); break;
        case 9: _t->GetPicCmd(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (VideoPicGet::*)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoPicGet::FramePositionSend)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (VideoPicGet::*)(unsigned long long , double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoPicGet::FrameMasageSend)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject VideoPicGet::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_VideoPicGet.data,
    qt_meta_data_VideoPicGet,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *VideoPicGet::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VideoPicGet::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VideoPicGet.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int VideoPicGet::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void VideoPicGet::FramePositionSend(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void VideoPicGet::FrameMasageSend(unsigned long long _t1, double _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
