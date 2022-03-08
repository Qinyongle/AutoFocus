#include "gps_data.h"
#include <QStringList>
#include <QTime>
#include <cmath>

QTime gpsTime;
QString gpsTimeStr;
GpsData::GpsData()
{
    heading = 0;
    pitch   = 0;
    lat     = 0;
    lng     = 0;
    alt     = 0;
    status  = 0;
}

GpsData::~GpsData()
{

}

GpsData::GpsData(const GpsData& data)
{
    copyRight(data);
}

const GpsData& GpsData::operator=(const GpsData& data)
{
    if(this!=&data)
    {
        copyRight(data);
    }
    return *this;
}

void GpsData::copyRight(const GpsData& data)
{
    heading = data.heading;
    pitch   = data.pitch;
    lat     = data.lat;
    lng     = data.lng;
    alt     = data.alt;
    status  = data.status;
}
bool GpsData::Parser(QStringList SrcData)
{
    if(SrcData.size() < 1) return false;
    if(SrcData.size() == 16 && SrcData[0].contains(QRegExp("\\$..HPD")))
    {
        if(SrcData[3] !="")
            heading = SrcData[3].toDouble();
        if(SrcData[4] !="")
            pitch = SrcData[4].toDouble();
        if(SrcData[6] !="")
            lat = SrcData[6].toDouble();
        if(SrcData[7] !="")
            lng = SrcData[7].toDouble();
        if(SrcData[8] !="")
            alt = SrcData[8].toDouble();
        if(SrcData[15] !="")
            status = SrcData[15].mid(0,1).toInt();
        return true;
    }
    else if(SrcData.size()==22&& SrcData[0].contains(QRegExp("\\$..HPD")))
    {
        if(SrcData[3] !="")
            heading = SrcData[3].toDouble();
        if(SrcData[4] !="")
            pitch = SrcData[4].toDouble();
        if(SrcData[6] !="")
            lat = SrcData[6].toDouble();
        if(SrcData[7] !="")
            lng = SrcData[7].toDouble();
        if(SrcData[8] !="")
            alt = SrcData[8].toDouble();
        if(SrcData[21] !="")
            status = SrcData[21].mid(0,1).toInt();
        return true;
    }
    else if(SrcData.size()==15&& SrcData[0].contains(QRegExp("\\$..GGA")))
    {
        int d;double m;
        if(SrcData[1] !="")
        {
            gpsTime = QTime::fromString(SrcData[1],"hhmmss.sss");
            gpsTimeStr = SrcData[1];
        }
        if(SrcData[2] !="")
        {
            d = (int)SrcData[2].toDouble()*0.01;
            m = (SrcData[2].toDouble() - d*100)/60;
            lat = d + m;
        }
        if(SrcData[3] == "S")
            lat = -1*lat;
        if(SrcData[4] !="")
        {
            d = (int)SrcData[4].toDouble()*0.01;
            m = (SrcData[4].toDouble() - d*100)/60;
            lng = d + m;
        }
        if(SrcData[5] == "W")
            lng = -1*lng;
        if(SrcData[6] !="")
            status = SrcData[6].toInt();

        if(SrcData[9] !="")
            alt = SrcData[9].toDouble();

        if(status == 0) {
            return false;
        }

        return true;
    }
    else if(SrcData.size()==13&& SrcData[0].contains(QRegExp("\\$..RMC")))
    {
        int d;double m;
        if(SrcData[1] !="")
        {
            gpsTime = QTime::fromString(SrcData[0],"hhmmss.sss");
        }
        if(SrcData[3] !="")
        {
            d = (int)SrcData[3].toDouble()*0.01;
            m = (SrcData[3].toDouble() - d*100)/60;
            lat = d + m;
        }
        if(SrcData[4] == "S")
            lat = -1*lat;
        if(SrcData[5] !="")
        {
            d = (int)SrcData[5].toDouble()*0.01;
            m = (SrcData[5].toDouble() - d*100)/60;
            lng = d + m;
        }
        if(SrcData[6] == "W")
            lng = -1*lng;


//        if(SrcData[11] !="")
//            alt = SrcData[8].toDouble();

        if(SrcData[2] != "A") {
            return false;
        }
        return true;
    }
    else if(SrcData.size()==10 && SrcData[0].contains(QRegExp("\\$..VTG")))
    {
        heading = SrcData[1].toDouble();
        return true;
    }
    else if(SrcData.size()==9 && SrcData[0].contains(QRegExp("\\$..VHW")))
    {
        heading = SrcData[1].toDouble();
        return true;
    }
    else if(SrcData.size()==18 && SrcData[0].contains(QRegExp("\\$HEADINGA")))
    {
        QString sol_stat = SrcData[1];
        QString pos_type = SrcData[2];
        // 解算状态和定位类型判断
        if((!sol_stat.contains("SOL_COMPUTED")) || (!pos_type.contains("NARROW_INT"))) {
            return false;
        }
        heading = SrcData[4].toDouble();
        return true;
    }
    else if(SrcData.size()==7 && SrcData[0].contains(QRegExp("\\$..CNG")))
    {
        bool gpsState = true;//SrcData[3] == "1" ? true : false;
        if(gpsState)
            heading = SrcData[4].toDouble();
        return true;
    }
    return false;
}
bool GpsData::Valid()
{
	if(fabs(heading -180)<0.00001)   return false;//厂家错误处理不当做的补救。送180表示数据不准确。
    if((heading > 360)||(heading<0)) return false;
    if((pitch > 90)||(pitch<-90))    return false;
    if((lat > 90)||(lat <-90))       return false;
    if((lng > 180)||(lng<-180))      return false;
    return true;
}
