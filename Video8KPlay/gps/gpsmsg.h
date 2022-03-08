#ifndef GPSMSG_H
#define GPSMSG_H
#include <QMetaType>
extern QTime gpsTime;
extern QString gpsTimeStr;

struct GpsMsg
{
public:
    GpsMsg();
    ~GpsMsg();
    GpsMsg(const GpsMsg&);
};

#endif // GPSMSG_H
