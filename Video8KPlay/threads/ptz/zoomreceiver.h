#ifndef ZOOMRECEIVER_H
#define ZOOMRECEIVER_H

#include <QObject>

class ZoomReceiver : public QObject
{
    Q_OBJECT
public: 
    explicit ZoomReceiver(QObject *parent = nullptr);

signals:
    void SendZoomNumber(double);

public slots:
    void DoRecive(QByteArray);

private:
    void DoAnalyze(QByteArray data);
    bool SumCheck(QByteArray buffer);
private:
    QByteArray m_arrangData;

};

#endif // ZOOMRECEIVER_H
