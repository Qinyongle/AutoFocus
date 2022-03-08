#ifndef PTZPROTOCOL_H
#define PTZPROTOCOL_H

#include<QByteArray>
#include<QString>

class PtzProtocol
{
public:
    PtzProtocol();
    static uchar checkSum(QByteArray);
    static QByteArray SetVerticalAngle(char data1, char data2,uchar addr=0x01);
    static QByteArray SetHorizontalAngle(char data1, char data2,uchar addr=0x01);
    static QByteArray SetSpeed(uchar hspeed=32,uchar vspeed=32,uchar addr=0x01);
    static QByteArray Up(uchar vspeed=32,uchar addr=0x01);
    static QByteArray Left(uchar hspeed=32,uchar addr=0x01);
    static QByteArray Right(uchar hspeed=32,uchar addr=0x01);
    static QByteArray Down(uchar vspeed=32,uchar addr=0x01);

    static QByteArray StopPtz(uchar addr=0x01);

    static QByteArray SetZoomNum(double num, uchar addr=0x01);
    static QByteArray ZoomIn(uchar addr=0x01);
    static QByteArray ZoomOut(uchar addr=0x01);
    static QByteArray ZoomStop(uchar addr=0x01);

};

#endif // PTZPROTOCOL_H
