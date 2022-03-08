/****************************************************************************
** Meta object code from reading C++ file 'mainprogram.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../mainprogram.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainprogram.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainProgram_t {
    QByteArrayData data[20];
    char stringdata0[254];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainProgram_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainProgram_t qt_meta_stringdata_MainProgram = {
    {
QT_MOC_LITERAL(0, 0, 11), // "MainProgram"
QT_MOC_LITERAL(1, 12, 10), // "RunStopCmd"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 9), // "GetPicCmd"
QT_MOC_LITERAL(4, 34, 18), // "SetSavePathNameCmd"
QT_MOC_LITERAL(5, 53, 10), // "StartTimer"
QT_MOC_LITERAL(6, 64, 9), // "StopTimer"
QT_MOC_LITERAL(7, 74, 10), // "ConnectGps"
QT_MOC_LITERAL(8, 85, 10), // "GetPicPath"
QT_MOC_LITERAL(9, 96, 12), // "SendPicTimer"
QT_MOC_LITERAL(10, 109, 10), // "LoadThread"
QT_MOC_LITERAL(11, 120, 19), // "SetSendTimeInterval"
QT_MOC_LITERAL(12, 140, 13), // "SetDecodeType"
QT_MOC_LITERAL(13, 154, 13), // "SetServiceUrl"
QT_MOC_LITERAL(14, 168, 11), // "SetFilePath"
QT_MOC_LITERAL(15, 180, 9), // "SetGpsTcp"
QT_MOC_LITERAL(16, 190, 15), // "SetSavePathName"
QT_MOC_LITERAL(17, 206, 13), // "replyFinished"
QT_MOC_LITERAL(18, 220, 14), // "QNetworkReply*"
QT_MOC_LITERAL(19, 235, 18) // "httpUploadFinished"

    },
    "MainProgram\0RunStopCmd\0\0GetPicCmd\0"
    "SetSavePathNameCmd\0StartTimer\0StopTimer\0"
    "ConnectGps\0GetPicPath\0SendPicTimer\0"
    "LoadThread\0SetSendTimeInterval\0"
    "SetDecodeType\0SetServiceUrl\0SetFilePath\0"
    "SetGpsTcp\0SetSavePathName\0replyFinished\0"
    "QNetworkReply*\0httpUploadFinished"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainProgram[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   99,    2, 0x06 /* Public */,
       3,    0,  100,    2, 0x06 /* Public */,
       4,    2,  101,    2, 0x06 /* Public */,
       5,    1,  106,    2, 0x06 /* Public */,
       6,    0,  109,    2, 0x06 /* Public */,
       7,    2,  110,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    1,  115,    2, 0x0a /* Public */,
       9,    0,  118,    2, 0x0a /* Public */,
      10,    0,  119,    2, 0x0a /* Public */,
      11,    1,  120,    2, 0x0a /* Public */,
      12,    1,  123,    2, 0x0a /* Public */,
      13,    1,  126,    2, 0x0a /* Public */,
      14,    1,  129,    2, 0x0a /* Public */,
      15,    2,  132,    2, 0x0a /* Public */,
      16,    2,  137,    2, 0x0a /* Public */,
      17,    1,  142,    2, 0x0a /* Public */,
      19,    0,  145,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    2,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    2,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    2,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    2,    2,
    QMetaType::Void, 0x80000000 | 18,    2,
    QMetaType::Void,

       0        // eod
};

void MainProgram::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainProgram *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->RunStopCmd(); break;
        case 1: _t->GetPicCmd(); break;
        case 2: _t->SetSavePathNameCmd((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 3: _t->StartTimer((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->StopTimer(); break;
        case 5: _t->ConnectGps((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: _t->GetPicPath((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->SendPicTimer(); break;
        case 8: _t->LoadThread(); break;
        case 9: _t->SetSendTimeInterval((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->SetDecodeType((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 11: _t->SetServiceUrl((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 12: _t->SetFilePath((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 13: _t->SetGpsTcp((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 14: _t->SetSavePathName((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 15: _t->replyFinished((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        case 16: _t->httpUploadFinished(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 15:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QNetworkReply* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MainProgram::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainProgram::RunStopCmd)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MainProgram::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainProgram::GetPicCmd)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MainProgram::*)(QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainProgram::SetSavePathNameCmd)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (MainProgram::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainProgram::StartTimer)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (MainProgram::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainProgram::StopTimer)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (MainProgram::*)(QString , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainProgram::ConnectGps)) {
                *result = 5;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainProgram::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_MainProgram.data,
    qt_meta_data_MainProgram,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainProgram::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainProgram::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainProgram.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int MainProgram::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void MainProgram::RunStopCmd()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void MainProgram::GetPicCmd()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void MainProgram::SetSavePathNameCmd(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MainProgram::StartTimer(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void MainProgram::StopTimer()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void MainProgram::ConnectGps(QString _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
