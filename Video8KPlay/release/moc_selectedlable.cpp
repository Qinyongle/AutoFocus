/****************************************************************************
** Meta object code from reading C++ file 'selectedlable.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../selectedlable.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'selectedlable.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SelectedLable_t {
    QByteArrayData data[16];
    char stringdata0[240];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SelectedLable_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SelectedLable_t qt_meta_stringdata_SelectedLable = {
    {
QT_MOC_LITERAL(0, 0, 13), // "SelectedLable"
QT_MOC_LITERAL(1, 14, 16), // "SelectedZoneMove"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 17), // "SelectedZoneWheel"
QT_MOC_LITERAL(4, 50, 13), // "SpaceKeyPress"
QT_MOC_LITERAL(5, 64, 17), // "SelectedLableHide"
QT_MOC_LITERAL(6, 82, 18), // "GetMouseChoiceZone"
QT_MOC_LITERAL(7, 101, 30), // "MosaicLable::MOUSE_CHOICE_ZONE"
QT_MOC_LITERAL(8, 132, 24), // "SelectedWidgetSizeChange"
QT_MOC_LITERAL(9, 157, 13), // "QResizeEvent*"
QT_MOC_LITERAL(10, 171, 13), // "CloseThisZone"
QT_MOC_LITERAL(11, 185, 11), // "slotShowYuv"
QT_MOC_LITERAL(12, 197, 14), // "unsigned char*"
QT_MOC_LITERAL(13, 212, 11), // "WidgetClose"
QT_MOC_LITERAL(14, 224, 10), // "SetAuxrect"
QT_MOC_LITERAL(15, 235, 4) // "rect"

    },
    "SelectedLable\0SelectedZoneMove\0\0"
    "SelectedZoneWheel\0SpaceKeyPress\0"
    "SelectedLableHide\0GetMouseChoiceZone\0"
    "MosaicLable::MOUSE_CHOICE_ZONE\0"
    "SelectedWidgetSizeChange\0QResizeEvent*\0"
    "CloseThisZone\0slotShowYuv\0unsigned char*\0"
    "WidgetClose\0SetAuxrect\0rect"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SelectedLable[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    5,   64,    2, 0x06 /* Public */,
       3,    4,   75,    2, 0x06 /* Public */,
       4,    0,   84,    2, 0x06 /* Public */,
       5,    1,   85,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    2,   88,    2, 0x0a /* Public */,
       8,    1,   93,    2, 0x0a /* Public */,
      10,    1,   96,    2, 0x0a /* Public */,
      11,    3,   99,    2, 0x0a /* Public */,
      13,    0,  106,    2, 0x0a /* Public */,
      14,    1,  107,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QSize, QMetaType::QPoint, QMetaType::QPoint, QMetaType::Bool,    2,    2,    2,    2,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::QSize, QMetaType::QPoint, QMetaType::Bool,    2,    2,    2,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, 0x80000000 | 7,    2,    2,
    QMetaType::Void, 0x80000000 | 9,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, 0x80000000 | 12, QMetaType::UInt, QMetaType::UInt,    2,    2,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QRect,   15,

       0        // eod
};

void SelectedLable::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SelectedLable *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->SelectedZoneMove((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QSize(*)>(_a[2])),(*reinterpret_cast< QPoint(*)>(_a[3])),(*reinterpret_cast< QPoint(*)>(_a[4])),(*reinterpret_cast< bool(*)>(_a[5]))); break;
        case 1: _t->SelectedZoneWheel((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QSize(*)>(_a[2])),(*reinterpret_cast< QPoint(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 2: _t->SpaceKeyPress(); break;
        case 3: _t->SelectedLableHide((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->GetMouseChoiceZone((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< MosaicLable::MOUSE_CHOICE_ZONE(*)>(_a[2]))); break;
        case 5: _t->SelectedWidgetSizeChange((*reinterpret_cast< QResizeEvent*(*)>(_a[1]))); break;
        case 6: _t->CloseThisZone((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->slotShowYuv((*reinterpret_cast< unsigned char*(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2])),(*reinterpret_cast< uint(*)>(_a[3]))); break;
        case 8: _t->WidgetClose(); break;
        case 9: _t->SetAuxrect((*reinterpret_cast< QRect(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SelectedLable::*)(int , QSize , QPoint , QPoint , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SelectedLable::SelectedZoneMove)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SelectedLable::*)(int , QSize , QPoint , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SelectedLable::SelectedZoneWheel)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (SelectedLable::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SelectedLable::SpaceKeyPress)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (SelectedLable::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SelectedLable::SelectedLableHide)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SelectedLable::staticMetaObject = { {
    &QOpenGLWidget::staticMetaObject,
    qt_meta_stringdata_SelectedLable.data,
    qt_meta_data_SelectedLable,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SelectedLable::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SelectedLable::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SelectedLable.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "QOpenGLFunctions"))
        return static_cast< QOpenGLFunctions*>(this);
    return QOpenGLWidget::qt_metacast(_clname);
}

int SelectedLable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QOpenGLWidget::qt_metacall(_c, _id, _a);
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
void SelectedLable::SelectedZoneMove(int _t1, QSize _t2, QPoint _t3, QPoint _t4, bool _t5)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t5))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SelectedLable::SelectedZoneWheel(int _t1, QSize _t2, QPoint _t3, bool _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SelectedLable::SpaceKeyPress()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void SelectedLable::SelectedLableHide(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
