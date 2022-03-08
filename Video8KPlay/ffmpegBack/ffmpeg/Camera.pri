#-------------------------------------------------
#
# Project created by QtCreator 2020-06-15T18:21:59
#
#-------------------------------------------------



#DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += WITH_OPENSSL

CONFIG += c++11

INCLUDEPATH += $$PWD/Onvif/gsoap/gsoap/runtime $$PWD/Onvif/include $$PWD/Onvif/onvifgen $$PWD/Onvif/include
#$$PWD/Onvif/3rdparty/openssl-1.0.1g/include

SOURCES += \
    $$PWD/ffmpegwidgets.cpp



HEADERS += \
    $$PWD/ffmpegwidgets.h


INCLUDEPATH += $$PWD/ffmpeg/include

LIBS += $$PWD/ffmpeg/lib/avcodec.lib \
        $$PWD/ffmpeg/lib/avdevice.lib \
        $$PWD/ffmpeg/lib/avfilter.lib \
        $$PWD/ffmpeg/lib/avformat.lib \
        $$PWD/ffmpeg/lib/avutil.lib \
        $$PWD/ffmpeg/lib/postproc.lib \
        $$PWD/ffmpeg/lib/swresample.lib \
        $$PWD/ffmpeg/lib/swscale.lib
