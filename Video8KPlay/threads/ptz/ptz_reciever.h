#ifndef PTZRECIEVER_H
#define PTZRECIEVER_H
#include "threads/data/datareciever.h"
#include <QObject>
#include <QMutex>
extern QMutex Ptzmutex;
extern QMutex PtzQuemutex;
extern QQueue<QByteArray> PtzAnalyzingQueue;
class PtzReciever : public DataReciever
{
    Q_OBJECT
public:
    explicit PtzReciever(QObject *parent = nullptr);

signals:

public slots:
    void DoRecive(QByteArray);
};

#endif // PTZRECIEVER_H
