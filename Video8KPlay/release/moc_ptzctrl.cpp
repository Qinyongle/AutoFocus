/****************************************************************************
** Meta object code from reading C++ file 'ptzctrl.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../ptzctrl.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ptzctrl.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_PtzCtrl_t {
    QByteArrayData data[15];
    char stringdata0[107];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PtzCtrl_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PtzCtrl_t qt_meta_stringdata_PtzCtrl = {
    {
QT_MOC_LITERAL(0, 0, 7), // "PtzCtrl"
QT_MOC_LITERAL(1, 8, 6), // "TcpRun"
QT_MOC_LITERAL(2, 15, 0), // ""
QT_MOC_LITERAL(3, 16, 7), // "TcpQuit"
QT_MOC_LITERAL(4, 24, 7), // "RRotate"
QT_MOC_LITERAL(5, 32, 7), // "LRotate"
QT_MOC_LITERAL(6, 40, 9), // "HAngleSet"
QT_MOC_LITERAL(7, 50, 7), // "URotate"
QT_MOC_LITERAL(8, 58, 7), // "DRotate"
QT_MOC_LITERAL(9, 66, 9), // "VAngleSet"
QT_MOC_LITERAL(10, 76, 10), // "RotateStop"
QT_MOC_LITERAL(11, 87, 3), // "ZIn"
QT_MOC_LITERAL(12, 91, 4), // "ZOut"
QT_MOC_LITERAL(13, 96, 4), // "ZSet"
QT_MOC_LITERAL(14, 101, 5) // "ZStop"

    },
    "PtzCtrl\0TcpRun\0\0TcpQuit\0RRotate\0LRotate\0"
    "HAngleSet\0URotate\0DRotate\0VAngleSet\0"
    "RotateStop\0ZIn\0ZOut\0ZSet\0ZStop"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PtzCtrl[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      13,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x06 /* Public */,
       3,    0,   80,    2, 0x06 /* Public */,
       4,    0,   81,    2, 0x06 /* Public */,
       5,    0,   82,    2, 0x06 /* Public */,
       6,    1,   83,    2, 0x06 /* Public */,
       7,    0,   86,    2, 0x06 /* Public */,
       8,    0,   87,    2, 0x06 /* Public */,
       9,    1,   88,    2, 0x06 /* Public */,
      10,    0,   91,    2, 0x06 /* Public */,
      11,    0,   92,    2, 0x06 /* Public */,
      12,    0,   93,    2, 0x06 /* Public */,
      13,    1,   94,    2, 0x06 /* Public */,
      14,    0,   97,    2, 0x06 /* Public */,

 // signals: parameters
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

void PtzCtrl::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<PtzCtrl *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->TcpRun(); break;
        case 1: _t->TcpQuit(); break;
        case 2: _t->RRotate(); break;
        case 3: _t->LRotate(); break;
        case 4: _t->HAngleSet((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 5: _t->URotate(); break;
        case 6: _t->DRotate(); break;
        case 7: _t->VAngleSet((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 8: _t->RotateStop(); break;
        case 9: _t->ZIn(); break;
        case 10: _t->ZOut(); break;
        case 11: _t->ZSet((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 12: _t->ZStop(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (PtzCtrl::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PtzCtrl::TcpRun)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (PtzCtrl::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PtzCtrl::TcpQuit)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (PtzCtrl::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PtzCtrl::RRotate)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (PtzCtrl::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PtzCtrl::LRotate)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (PtzCtrl::*)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PtzCtrl::HAngleSet)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (PtzCtrl::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PtzCtrl::URotate)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (PtzCtrl::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PtzCtrl::DRotate)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (PtzCtrl::*)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PtzCtrl::VAngleSet)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (PtzCtrl::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PtzCtrl::RotateStop)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (PtzCtrl::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PtzCtrl::ZIn)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (PtzCtrl::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PtzCtrl::ZOut)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (PtzCtrl::*)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PtzCtrl::ZSet)) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (PtzCtrl::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PtzCtrl::ZStop)) {
                *result = 12;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject PtzCtrl::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_PtzCtrl.data,
    qt_meta_data_PtzCtrl,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *PtzCtrl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PtzCtrl::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_PtzCtrl.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int PtzCtrl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void PtzCtrl::TcpRun()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void PtzCtrl::TcpQuit()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void PtzCtrl::RRotate()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void PtzCtrl::LRotate()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void PtzCtrl::HAngleSet(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void PtzCtrl::URotate()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void PtzCtrl::DRotate()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void PtzCtrl::VAngleSet(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void PtzCtrl::RotateStop()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void PtzCtrl::ZIn()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void PtzCtrl::ZOut()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}

// SIGNAL 11
void PtzCtrl::ZSet(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void PtzCtrl::ZStop()
{
    QMetaObject::activate(this, &staticMetaObject, 12, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
