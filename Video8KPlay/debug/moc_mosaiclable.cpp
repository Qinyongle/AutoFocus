/****************************************************************************
** Meta object code from reading C++ file 'mosaiclable.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../mosaiclable.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mosaiclable.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MosaicLable_t {
    QByteArrayData data[17];
    char stringdata0[255];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MosaicLable_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MosaicLable_t qt_meta_stringdata_MosaicLable = {
    {
QT_MOC_LITERAL(0, 0, 11), // "MosaicLable"
QT_MOC_LITERAL(1, 12, 16), // "SendSelectedZone"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 30), // "MosaicLable::MOUSE_CHOICE_ZONE"
QT_MOC_LITERAL(4, 61, 17), // "CloseSelectedZone"
QT_MOC_LITERAL(5, 79, 13), // "SpaceKeyPress"
QT_MOC_LITERAL(6, 93, 9), // "FocusZone"
QT_MOC_LITERAL(7, 103, 11), // "ImageUpdate"
QT_MOC_LITERAL(8, 115, 11), // "slotShowYuv"
QT_MOC_LITERAL(9, 127, 14), // "unsigned char*"
QT_MOC_LITERAL(10, 142, 16), // "SelectedZoneMove"
QT_MOC_LITERAL(11, 159, 17), // "SelectedZoneWheel"
QT_MOC_LITERAL(12, 177, 15), // "GetDrawZoneSize"
QT_MOC_LITERAL(13, 193, 10), // "LableClose"
QT_MOC_LITERAL(14, 204, 14), // "AddZoneClicked"
QT_MOC_LITERAL(15, 219, 17), // "ReviseZoneClicked"
QT_MOC_LITERAL(16, 237, 17) // "DeleteZoneClicked"

    },
    "MosaicLable\0SendSelectedZone\0\0"
    "MosaicLable::MOUSE_CHOICE_ZONE\0"
    "CloseSelectedZone\0SpaceKeyPress\0"
    "FocusZone\0ImageUpdate\0slotShowYuv\0"
    "unsigned char*\0SelectedZoneMove\0"
    "SelectedZoneWheel\0GetDrawZoneSize\0"
    "LableClose\0AddZoneClicked\0ReviseZoneClicked\0"
    "DeleteZoneClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MosaicLable[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   79,    2, 0x06 /* Public */,
       4,    1,   84,    2, 0x06 /* Public */,
       5,    0,   87,    2, 0x06 /* Public */,
       6,    4,   88,    2, 0x06 /* Public */,
       7,    0,   97,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    3,   98,    2, 0x0a /* Public */,
      10,    5,  105,    2, 0x0a /* Public */,
      11,    4,  116,    2, 0x0a /* Public */,
      12,    1,  125,    2, 0x0a /* Public */,
      13,    1,  128,    2, 0x0a /* Public */,
      14,    0,  131,    2, 0x08 /* Private */,
      15,    0,  132,    2, 0x08 /* Private */,
      16,    0,  133,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, 0x80000000 | 3,    2,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,    2,    2,    2,    2,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 9, QMetaType::UInt, QMetaType::UInt,    2,    2,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::QSize, QMetaType::QPoint, QMetaType::QPoint, QMetaType::Bool,    2,    2,    2,    2,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::QSize, QMetaType::QPoint, QMetaType::Bool,    2,    2,    2,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MosaicLable::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MosaicLable *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->SendSelectedZone((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< MosaicLable::MOUSE_CHOICE_ZONE(*)>(_a[2]))); break;
        case 1: _t->CloseSelectedZone((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->SpaceKeyPress(); break;
        case 3: _t->FocusZone((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 4: _t->ImageUpdate(); break;
        case 5: _t->slotShowYuv((*reinterpret_cast< unsigned char*(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2])),(*reinterpret_cast< uint(*)>(_a[3]))); break;
        case 6: _t->SelectedZoneMove((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QSize(*)>(_a[2])),(*reinterpret_cast< QPoint(*)>(_a[3])),(*reinterpret_cast< QPoint(*)>(_a[4])),(*reinterpret_cast< bool(*)>(_a[5]))); break;
        case 7: _t->SelectedZoneWheel((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QSize(*)>(_a[2])),(*reinterpret_cast< QPoint(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 8: _t->GetDrawZoneSize((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->LableClose((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->AddZoneClicked(); break;
        case 11: _t->ReviseZoneClicked(); break;
        case 12: _t->DeleteZoneClicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MosaicLable::*)(int , MosaicLable::MOUSE_CHOICE_ZONE );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MosaicLable::SendSelectedZone)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MosaicLable::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MosaicLable::CloseSelectedZone)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MosaicLable::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MosaicLable::SpaceKeyPress)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (MosaicLable::*)(int , int , int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MosaicLable::FocusZone)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (MosaicLable::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MosaicLable::ImageUpdate)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MosaicLable::staticMetaObject = { {
    &QOpenGLWidget::staticMetaObject,
    qt_meta_stringdata_MosaicLable.data,
    qt_meta_data_MosaicLable,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MosaicLable::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MosaicLable::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MosaicLable.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "QOpenGLFunctions"))
        return static_cast< QOpenGLFunctions*>(this);
    return QOpenGLWidget::qt_metacast(_clname);
}

int MosaicLable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QOpenGLWidget::qt_metacall(_c, _id, _a);
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
void MosaicLable::SendSelectedZone(int _t1, MosaicLable::MOUSE_CHOICE_ZONE _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MosaicLable::CloseSelectedZone(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MosaicLable::SpaceKeyPress()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void MosaicLable::FocusZone(int _t1, int _t2, int _t3, int _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void MosaicLable::ImageUpdate()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
