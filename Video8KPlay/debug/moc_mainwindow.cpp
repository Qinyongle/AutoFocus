/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[43];
    char stringdata0[660];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 18), // "SetSavePathNameCmd"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 11), // "RunStartCmd"
QT_MOC_LITERAL(4, 43, 28), // "QtFfmpegDecoder::DECODE_TYPE"
QT_MOC_LITERAL(5, 72, 10), // "RunStopCmd"
QT_MOC_LITERAL(6, 83, 12), // "PlayingState"
QT_MOC_LITERAL(7, 96, 16), // "SetVideoFrameInt"
QT_MOC_LITERAL(8, 113, 15), // "SetDrawZoneSize"
QT_MOC_LITERAL(9, 129, 19), // "SetDecodeSpeedDelay"
QT_MOC_LITERAL(10, 149, 10), // "AudioStart"
QT_MOC_LITERAL(11, 160, 9), // "AudioQuit"
QT_MOC_LITERAL(12, 170, 9), // "SetVolume"
QT_MOC_LITERAL(13, 180, 12), // "SendPicState"
QT_MOC_LITERAL(14, 193, 11), // "SendAuxRect"
QT_MOC_LITERAL(15, 205, 8), // "StartCmd"
QT_MOC_LITERAL(16, 214, 10), // "SendSwitch"
QT_MOC_LITERAL(17, 225, 3), // "url"
QT_MOC_LITERAL(18, 229, 8), // "OpenFile"
QT_MOC_LITERAL(19, 238, 10), // "OpenStream"
QT_MOC_LITERAL(20, 249, 9), // "ClosePlay"
QT_MOC_LITERAL(21, 259, 17), // "PlayButtonClicked"
QT_MOC_LITERAL(22, 277, 18), // "SliderValueChanged"
QT_MOC_LITERAL(23, 296, 14), // "SliderReleased"
QT_MOC_LITERAL(24, 311, 13), // "SliderPressed"
QT_MOC_LITERAL(25, 325, 17), // "VolumeSliderMoved"
QT_MOC_LITERAL(26, 343, 25), // "VolumeSliderVaalueChanged"
QT_MOC_LITERAL(27, 369, 19), // "VolumeButtonClicked"
QT_MOC_LITERAL(28, 389, 19), // "DecodeMenuTriggered"
QT_MOC_LITERAL(29, 409, 8), // "QAction*"
QT_MOC_LITERAL(30, 418, 20), // "ZoneNumMenuTriggered"
QT_MOC_LITERAL(31, 439, 15), // "AddSpeedClicked"
QT_MOC_LITERAL(32, 455, 18), // "ReduceSpeedClicked"
QT_MOC_LITERAL(33, 474, 17), // "ResetSpeedClicked"
QT_MOC_LITERAL(34, 492, 27), // "SpeedNumSpinBoxValueChanged"
QT_MOC_LITERAL(35, 520, 18), // "CameraConfigConfig"
QT_MOC_LITERAL(36, 539, 20), // "AboutActionTriggered"
QT_MOC_LITERAL(37, 560, 22), // "PictureActionTriggered"
QT_MOC_LITERAL(38, 583, 12), // "GetFrameData"
QT_MOC_LITERAL(39, 596, 18), // "GetCurrentFramTime"
QT_MOC_LITERAL(40, 615, 15), // "LableKeyPressed"
QT_MOC_LITERAL(41, 631, 12), // "GetStreamUrl"
QT_MOC_LITERAL(42, 644, 15) // "ConfigFocusZone"

    },
    "MainWindow\0SetSavePathNameCmd\0\0"
    "RunStartCmd\0QtFfmpegDecoder::DECODE_TYPE\0"
    "RunStopCmd\0PlayingState\0SetVideoFrameInt\0"
    "SetDrawZoneSize\0SetDecodeSpeedDelay\0"
    "AudioStart\0AudioQuit\0SetVolume\0"
    "SendPicState\0SendAuxRect\0StartCmd\0"
    "SendSwitch\0url\0OpenFile\0OpenStream\0"
    "ClosePlay\0PlayButtonClicked\0"
    "SliderValueChanged\0SliderReleased\0"
    "SliderPressed\0VolumeSliderMoved\0"
    "VolumeSliderVaalueChanged\0VolumeButtonClicked\0"
    "DecodeMenuTriggered\0QAction*\0"
    "ZoneNumMenuTriggered\0AddSpeedClicked\0"
    "ReduceSpeedClicked\0ResetSpeedClicked\0"
    "SpeedNumSpinBoxValueChanged\0"
    "CameraConfigConfig\0AboutActionTriggered\0"
    "PictureActionTriggered\0GetFrameData\0"
    "GetCurrentFramTime\0LableKeyPressed\0"
    "GetStreamUrl\0ConfigFocusZone"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      38,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      14,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,  204,    2, 0x06 /* Public */,
       3,    2,  209,    2, 0x06 /* Public */,
       5,    0,  214,    2, 0x06 /* Public */,
       6,    1,  215,    2, 0x06 /* Public */,
       7,    1,  218,    2, 0x06 /* Public */,
       8,    1,  221,    2, 0x06 /* Public */,
       9,    1,  224,    2, 0x06 /* Public */,
      10,    3,  227,    2, 0x06 /* Public */,
      11,    1,  234,    2, 0x06 /* Public */,
      12,    1,  237,    2, 0x06 /* Public */,
      13,    0,  240,    2, 0x06 /* Public */,
      14,    1,  241,    2, 0x06 /* Public */,
      15,    1,  244,    2, 0x06 /* Public */,
      16,    1,  247,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      18,    0,  250,    2, 0x08 /* Private */,
      19,    0,  251,    2, 0x08 /* Private */,
      20,    0,  252,    2, 0x08 /* Private */,
      21,    0,  253,    2, 0x08 /* Private */,
      22,    1,  254,    2, 0x08 /* Private */,
      23,    0,  257,    2, 0x08 /* Private */,
      24,    0,  258,    2, 0x08 /* Private */,
      25,    1,  259,    2, 0x08 /* Private */,
      26,    1,  262,    2, 0x08 /* Private */,
      27,    0,  265,    2, 0x08 /* Private */,
      28,    1,  266,    2, 0x08 /* Private */,
      30,    1,  269,    2, 0x08 /* Private */,
      31,    0,  272,    2, 0x08 /* Private */,
      32,    0,  273,    2, 0x08 /* Private */,
      33,    0,  274,    2, 0x08 /* Private */,
      34,    1,  275,    2, 0x08 /* Private */,
      35,    0,  278,    2, 0x08 /* Private */,
      36,    0,  279,    2, 0x08 /* Private */,
      37,    0,  280,    2, 0x08 /* Private */,
      38,    2,  281,    2, 0x0a /* Public */,
      39,    1,  286,    2, 0x0a /* Public */,
      40,    0,  289,    2, 0x0a /* Public */,
      41,    1,  290,    2, 0x0a /* Public */,
      42,    4,  293,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    2,    2,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 4,    2,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Long,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    2,    2,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QRect,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,   17,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 29,    2,
    QMetaType::Void, 0x80000000 | 29,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::Double,    2,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,    2,    2,    2,    2,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->SetSavePathNameCmd((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->RunStartCmd((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QtFfmpegDecoder::DECODE_TYPE(*)>(_a[2]))); break;
        case 2: _t->RunStopCmd(); break;
        case 3: _t->PlayingState((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->SetVideoFrameInt((*reinterpret_cast< long(*)>(_a[1]))); break;
        case 5: _t->SetDrawZoneSize((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->SetDecodeSpeedDelay((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->AudioStart((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 8: _t->AudioQuit((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->SetVolume((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 10: _t->SendPicState(); break;
        case 11: _t->SendAuxRect((*reinterpret_cast< QRect(*)>(_a[1]))); break;
        case 12: _t->StartCmd((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 13: _t->SendSwitch((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 14: _t->OpenFile(); break;
        case 15: _t->OpenStream(); break;
        case 16: _t->ClosePlay(); break;
        case 17: _t->PlayButtonClicked(); break;
        case 18: _t->SliderValueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: _t->SliderReleased(); break;
        case 20: _t->SliderPressed(); break;
        case 21: _t->VolumeSliderMoved((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 22: _t->VolumeSliderVaalueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 23: _t->VolumeButtonClicked(); break;
        case 24: _t->DecodeMenuTriggered((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 25: _t->ZoneNumMenuTriggered((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 26: _t->AddSpeedClicked(); break;
        case 27: _t->ReduceSpeedClicked(); break;
        case 28: _t->ResetSpeedClicked(); break;
        case 29: _t->SpeedNumSpinBoxValueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 30: _t->CameraConfigConfig(); break;
        case 31: _t->AboutActionTriggered(); break;
        case 32: _t->PictureActionTriggered(); break;
        case 33: _t->GetFrameData((*reinterpret_cast< unsigned long long(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 34: _t->GetCurrentFramTime((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 35: _t->LableKeyPressed(); break;
        case 36: _t->GetStreamUrl((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 37: _t->ConfigFocusZone((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 24:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAction* >(); break;
            }
            break;
        case 25:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAction* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MainWindow::*)(QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::SetSavePathNameCmd)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(QString , QtFfmpegDecoder::DECODE_TYPE );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::RunStartCmd)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::RunStopCmd)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::PlayingState)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(long );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::SetVideoFrameInt)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::SetDrawZoneSize)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::SetDecodeSpeedDelay)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(int , int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::AudioStart)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::AudioQuit)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::SetVolume)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::SendPicState)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(QRect );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::SendAuxRect)) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::StartCmd)) {
                *result = 12;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::SendSwitch)) {
                *result = 13;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 38)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 38;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 38)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 38;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::SetSavePathNameCmd(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MainWindow::RunStartCmd(QString _t1, QtFfmpegDecoder::DECODE_TYPE _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MainWindow::RunStopCmd()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void MainWindow::PlayingState(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void MainWindow::SetVideoFrameInt(long _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void MainWindow::SetDrawZoneSize(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void MainWindow::SetDecodeSpeedDelay(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void MainWindow::AudioStart(int _t1, int _t2, int _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void MainWindow::AudioQuit(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void MainWindow::SetVolume(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void MainWindow::SendPicState()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}

// SIGNAL 11
void MainWindow::SendAuxRect(QRect _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void MainWindow::StartCmd(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void MainWindow::SendSwitch(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
