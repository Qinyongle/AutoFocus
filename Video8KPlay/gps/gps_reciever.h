#ifndef GPSRECIEVER_H
#define GPSRECIEVER_H

#include "data/datareciever.h"

class GpsReciever:public DataReciever
{
     Q_OBJECT
public:
    explicit GpsReciever(QObject *parent = nullptr);

public slots:
    void DoRecive(QByteArray bytes);

};

#endif // GPSRECIEVER_H
