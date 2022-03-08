#ifndef PTZANALYZER_H
#define PTZANALYZER_H
#include <QObject>

class PtzAnalyzer : public QObject
{
    Q_OBJECT
public:
    explicit PtzAnalyzer(int ptzId =0,QObject *parent = nullptr);
    const static int  SMALL = 0;
    const static int  BIG = 1;
signals:
    void DataPacket(QString);
    void ptzHAngle(double);
    void ptzVAngle(double);   

public slots:
    void DoAnalyze();
    void setEquipType(int);
private:
    bool SumCheck(QByteArray buffer);
    int m_EqpType;
    int m_nPtzId;
};

#endif // PTZANALYZER_H
