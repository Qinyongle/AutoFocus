#ifndef JSONRECIEVER_H
#define JSONRECIEVER_H

#include "datareciever.h"

class JsonReciever:public DataReciever
{
    Q_OBJECT
public:
    JsonReciever(QObject *parent = nullptr);

signals:
    void sendJson(QByteArray);

public slots:
    void DoRecive(QByteArray bytes);
};

#endif // PTZJSONRECIEVER_H
