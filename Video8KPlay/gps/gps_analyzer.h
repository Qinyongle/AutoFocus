#ifndef GPSANALYZER_H
#define GPSANALYZER_H

#include "data/datastruct.h"
#include <QObject>
#include <QByteArray>
#include <QMutex>
extern QMutex GpsMutex;
extern QMutex GpsQueMutex;
extern GpsData gps;
extern QQueue<QByteArray> GpsAnalyzingQueue;
class GpsAnalyzer : public QObject
{
    Q_OBJECT
public:
    explicit GpsAnalyzer(QObject *parent = nullptr);

signals:
    void GpsPotData(GpsData);
    void showGPsMsg(QByteArray);
public slots:
    void DoAnalyze();

private:
    //uchar XorCheck(QByteArray);
    bool m_bCheck;
    bool CheckXor(QByteArray);
    uchar checkxor(QByteArray);
};

#endif // GPSANALYZER_H
