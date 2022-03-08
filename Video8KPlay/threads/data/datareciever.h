#ifndef DATARECIEVER_H
#define DATARECIEVER_H
#include <QObject>

class DataReciever : public QObject
{
    Q_OBJECT
public:
    explicit DataReciever(QObject *parent = nullptr);

signals:
    void OrignalData(QString);
    void OrignalData(QByteArray);
    void resultReady(QByteArray);
    void dataReady();

public slots:
    virtual void DoRecive(QByteArray bytes);

protected:
     QByteArray m_arrangData;
     void setHead(QByteArray);
     void setTail(QByteArray);
     QByteArray head;
     QByteArray tail;
};

#endif // DATARECIEVER_H
