#include "ptz_protocol.h"
#include<QByteArray>

PtzProtocol::PtzProtocol()
{
   
}
uchar PtzProtocol::checkSum(QByteArray buffer)
{
    uchar sum =0;
    for(int i =1;i<buffer.size()-1;i++)
    {
        sum += (uchar)buffer.at(i);
    }
    return sum;
}


QByteArray PtzProtocol::SetVerticalAngle(char datah, char datal,uchar addr)
{
    //FF Addr 00 4D DataH DataL Checsum
    QByteArray array(7, 0x00);
    array[0] = 0xff;
    array[1] = addr;
    array[2] = 0x00;
    array[3] = 0x4D;
    array[4] = datah;
    array[5] = datal;
    array[6] = checkSum(array);
    return array;
}

QByteArray PtzProtocol::SetHorizontalAngle(char datah, char datal,uchar addr)
{
    //FF Addr 00 4B DataH DataL Checksum
    QByteArray array(7, 0x00);
    array[0] = 0xff;
    array[1] = addr;
    array[2] = 0x00;
    array[3] = 0x4b;
    array[4] = datah;
    array[5] = datal;
    array[6] = checkSum(array);
    return array;
}
QByteArray PtzProtocol::SetSpeed(uchar hspeed,uchar vspeed,uchar addr)
{
    //speedset	FF Addr 00 5F HSPD VSPD Checsum
    QByteArray array(7, 0x00);
    array[0] = 0xff;
    array[1] = addr;
    array[2] = 0x00;
    array[3] = 0x5f;
    array[4] = hspeed;
    array[5] = vspeed;
    array[6] = checkSum(array);
    return array;
}


QByteArray PtzProtocol::Up(uchar vspeed,uchar addr)
{
    //{0xFF,0x01,0x00,0x08,0x00,0x3F,0x48,}
    QByteArray array(7, 0x00);
    array[0] = 0xff;
    array[1] = addr;
    array[2] = 0x0;
    array[3] = 0x8;
    array[4] = 0x0;
    array[5] = vspeed;
    array[6] = checkSum(array);
    return array;
}
QByteArray PtzProtocol::Down(uchar vspeed,uchar addr)
{
    //{0xFF,0x01,0x00,0x10,0x00,0x3F,0x50,}
    QByteArray array(7, 0x00);
    array[0] = 0xff;
    array[1] = addr;
    array[2] = 0x0;
    array[3] = 0x10;
    array[4] = 0x0;
    array[5] = vspeed;
    array[6] = checkSum(array);
    return array;
}
QByteArray PtzProtocol::Left(uchar hspeed,uchar addr)
{
    //{0xFF,addr,0x00,0x04,0x3F,0x00,checksum,}
    QByteArray array(7, 0x00);
    array[0] = 0xff;
    array[1] = addr;
    array[2] = 0x0;
    array[3] = 0x04;
    array[4] = hspeed;
    array[5] = 0x0;
    array[6] = checkSum(array);
    return array;
}

QByteArray PtzProtocol::Right(uchar hspeed,uchar addr)
{
    //{0xFF,addr,0x00,0x02,0x3F,0x00,checksum,}
    QByteArray array(7, 0x00);
    array[0] = 0xff;
    array[1] = addr;
    array[2] = 0x0;
    array[3] = 0x02;
    array[4] = hspeed;
    array[5] = 0x0;
    array[6] = checkSum(array);
    return array;
}

QByteArray PtzProtocol::StopPtz(uchar addr )
{
    QByteArray array(7, 0x00);
    array[0] = 0xff;
    array[1] = addr;
    array[2] = 0x0;
    array[3] = 0x0;
    array[4] = 0x0;
    array[5] = 0x0;
    array[6] = addr;
    return array;
}

QByteArray PtzProtocol::SetZoomNum(double num, uchar addr)
{
    int numTemp = num;
    uchar hNum = numTemp >> 8;
    uchar lNum = numTemp;
    QByteArray array(7, 0x00);
    array[0] = 0xff;
    array[1] = addr;
    array[2] = 0x0;
    array[3] = 0x4c;
    array[4] = hNum;
    array[5] = lNum;
    array[6] = checkSum(array);
    return array;
}

QByteArray PtzProtocol::ZoomIn(uchar addr)
{
    QByteArray array(7, 0x00);
    array[0] = 0xff;
    array[1] = addr;
    array[2] = 0x0;
    array[3] = 0x21;
    array[4] = 0x0;
    array[5] = 0x0;
    array[6] = checkSum(array);
    return array;
}

QByteArray PtzProtocol::ZoomOut(uchar addr)
{
    QByteArray array(7, 0x00);
    array[0] = 0xff;
    array[1] = addr;
    array[2] = 0x0;
    array[3] = 0x20;
    array[4] = 0x0;
    array[5] = 0x0;
    array[6] = checkSum(array);
    return array;
}

QByteArray PtzProtocol::ZoomStop(uchar addr)
{
    QByteArray array(7, 0x00);
    array[0] = 0xff;
    array[1] = addr;
    array[2] = 0x0;
    array[3] = 0x01;
    array[4] = 0x0;
    array[5] = 0x0;
    array[6] = checkSum(array);
    return array;
}

