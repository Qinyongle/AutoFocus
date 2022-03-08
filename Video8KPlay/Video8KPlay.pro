QT       += core gui sql opengl multimedia
QT       += xml network xmlpatterns
QT       += serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
TARGET = Video8KPlay
TEMPLATE = app
RC_FILE += logo.rc

include (./ffmpeg/Camera.pri);

DESTDIR = .\bin\debug

LIBS += -lopengl32 -lglu32
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    audioplay.cpp \
    cameraconfigwidget.cpp \
    cpptransqt.cpp \
    data/datareciever.cpp \
    data/datastruct.cpp \
    gps/gps_analyzer.cpp \
    gps/gps_data.cpp \
    gps/gps_reciever.cpp \
    gpsgetthread.cpp \
    inifile.cpp \
    main.cpp \
    mainprogram.cpp \
    mainwindow.cpp \
    mainwindowwidget.cpp \
    mosaiclable.cpp \
    ptzconfig.cpp \
    ptzctrl.cpp \
    realplaywidget.cpp \
    selectedlable.cpp \
    selectedwidegt.cpp \
    streamconfigwidget.cpp \
    tcp/gps_tcpserver.cpp \
    tcp/tcpserver.cpp \
    tcp/tcpsocket.cpp \
    threads/data/datareciever.cpp \
    threads/data/jsonreciever.cpp \
    threads/ptz/ptz_analyzer.cpp \
    threads/ptz/ptz_controller.cpp \
    threads/ptz/ptz_opencv.cpp \
    threads/ptz/ptz_protocol.cpp \
    threads/ptz/ptz_reciever.cpp \
    threads/ptz/zoomreceiver.cpp \
    threads/ptzthreads.cpp \
    threads/tcp/tcpserver.cpp \
    threads/tcp/tcpsocket.cpp \
    webapi/webapi.cpp

HEADERS += \
    audioplay.h \
    cameraconfigwidget.h \
    cpptransqt.h \
    data/datareciever.h \
    data/datastruct.h \
    gps/gps_analyzer.h \
    gps/gps_data.h \
    gps/gps_reciever.h \
    gpsgetthread.h \
    inifile.h \
    mainprogram.h \
    mainwindow.h \
    mainwindowwidget.h \
    mosaiclable.h \
    ptzconfig.h \
    ptzctrl.h \
    realplaywidget.h \
    selectedlable.h \
    selectedwidegt.h \
    streamconfigwidget.h \
    tcp/gps_tcpserver.h \
    tcp/tcpserver.h \
    tcp/tcpsocket.h \
    threads/data/datareciever.h \
    threads/data/jsonreciever.h \
    threads/ptz/ptz_analyzer.h \
    threads/ptz/ptz_controller.h \
    threads/ptz/ptz_opencv.h \
    threads/ptz/ptz_protocol.h \
    threads/ptz/ptz_reciever.h \
    threads/ptz/zoomreceiver.h \
    threads/ptzthreads.h \
    threads/tcp/tcpserver.h \
    threads/tcp/tcpsocket.h \
    webapi/webapi.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    logo.rc

RESOURCES += \
    qss.qrc

LIBS += -LE:/OPENCV/buildopencv/install/x64/vc15/lib/ -lopencv_world455d

INCLUDEPATH += E:/OPENCV/buildopencv/install/include


DEPENDPATH += E:/OPENCV/buildopencv/install/include

