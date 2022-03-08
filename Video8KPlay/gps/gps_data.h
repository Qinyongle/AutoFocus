#ifndef GPSDATA_H
#define GPSDATA_H
#include <QMetaType>
extern QTime gpsTime;
extern QString gpsTimeStr;

struct GpsData
{
public:
    GpsData();
    ~GpsData();
    GpsData(const GpsData&);
    const GpsData& operator=(const GpsData&);
    void copyRight(const GpsData&);

    double  heading; //（0~359.99）hhh.hh 147.79
    double  pitch;   //-90~90）+/-pp.ppp  0.578
    double  lat;     //(-90～90) 39.778353
    double  lng;     //(-180～180) 116.5560331
    double  alt;     //高度，单位（米）45.51
    int     status;  //系统状态：0：寻星初始化 1：定位不定向  2：定位定向
    bool Valid();
    bool Parser(QStringList SrcData);
};
Q_DECLARE_METATYPE(GpsData)
#endif // GPSDATA_H
