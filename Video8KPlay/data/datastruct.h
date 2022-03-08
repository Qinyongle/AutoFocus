#ifndef DATASTRUCT_H
#define DATASTRUCT_H
#include <QString>
#include <QStringList>
#include <QVariant>
#include <QMetaType>
#include "gps/gps_data.h"

struct potLLA
{
    double Lng;
    double Lat;
    double Alt;
};
Q_DECLARE_METATYPE(potLLA)

struct ptzPose   //angle
{
    double Horiz;
    double Vert;
    double hOffset;
    double vOffset;
};
struct Modif     //angle
{
    double toHoriz;
    double toVert;
};

struct ptzParam
{
    GpsData local;
    potLLA peer;
    ptzPose ptz;
    Modif calcu;
    double dis;//km

    void LocalLng(double lng)  { local.lng  = lng;}
    void LocalLat(double lat)  { local.lat  = lat;}
    void LocalAlt(double alt)  { local.alt  = alt;}
    void Heading(double h){ local.heading = h;}
    void Pitch(double p)  { local.pitch = p;}

    void PeerLng(double lng) { peer.Lng   = lng;}
    void PeerLat(double lat) { peer.Lat   = lat;}
    void PeerAlt(double alt)    { peer.Alt   = alt;}

    void ToPtzHAngle(double h) { calcu.toHoriz = h;}
    void ToPtzVAngle(double v) { calcu.toVert  = v;}
    void Distance(double d)      { dis = d;}
    void PtzHAngle(double val) { ptz.Horiz = val;}
    void PtzVAngle(double val) { ptz.Vert = val;}
    void PtzHOffset(double offset) { ptz.hOffset = offset;}
    void PtzVOffset(double offset) { ptz.vOffset = offset;}

    double LocalLng() { return local.lng;}
    double LocalLat() { return local.lat; }
    double LocalAlt() { return local.alt; }
    double Heading() { return local.heading;}
    double Pitch() { return local.pitch;}
    double PeerAlt() {return peer.Alt;}
    double PeerLng() {return peer.Lng;}
    double PeerLat() {return peer.Lat;}
    double ToPtzHAngle() { return calcu.toHoriz;}
    double ToPtzVAngle() { return calcu.toVert;}

    double PtzHAngle() {return ptz.Horiz;}
    double PtzVAngle()   {return ptz.Vert;}
    double PtzHOffset() {return ptz.hOffset;}
    double PtzVOffset() {return ptz.vOffset;}
    double Distance()   {return dis;}
};
Q_DECLARE_METATYPE(ptzParam)
/*
struct PtzStatus
{
    uchar ptzId;
    QString ptzrunEnv;//云台的运行环境
    QString ptzLink;//云台连接状态
    QString ptzCtrl;//云台控制状态
    QString peerLng;
    QString peerLat;
    QString peerAlt;
};
Q_DECLARE_METATYPE(PtzStatus)
*/
#endif // DATASTRUCT_H
