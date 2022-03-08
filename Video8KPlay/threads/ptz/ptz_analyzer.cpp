#include "ptz_analyzer.h"
#include "ptz_reciever.h"
#include <QQueue>
#include <QMutex>
#include <QDebug>
#include <QTime>
#include <QDateTime>
#include <QStringList>

PtzAnalyzer::PtzAnalyzer(int ptzId,QObject *parent) : QObject(parent)
{
    m_nPtzId = ptzId;
    m_EqpType = 0;

    m_EqpType = BIG;
    m_EqpType = SMALL;
}

void PtzAnalyzer::setEquipType(int type)
{
    if(type == 1)
    {
        m_EqpType = BIG;
    }
    else
    {
        m_EqpType = SMALL;
    }
}

void PtzAnalyzer::DoAnalyze()
{
    while(!PtzAnalyzingQueue.isEmpty())
    {
        PtzQuemutex.lock();
        QByteArray data = PtzAnalyzingQueue.dequeue();
        PtzQuemutex.unlock();
        QString str;
        for(int i = 0;i< data.size();i++)
        {
            char b = data.at(i);
            QByteArray byte;
            byte.append(b);
            str.append(byte.toHex().toUpper());
            str.append(" ");
        }
        emit DataPacket(str);
        if(!SumCheck(data))
        {

            //qDebug()<<"ptz data check error!";
            continue;
        }
        uchar oneByte = (uchar)data.at(3);

        if(oneByte ==0x59 && m_EqpType == SMALL)
        {
            ushort u_horizontalAngle = 0;
            uchar tempchar = (uchar)data.at(4);
            u_horizontalAngle = tempchar;
            u_horizontalAngle <<= 8;
            tempchar = (uchar)data.at(5);
            u_horizontalAngle += tempchar;
            double hAngle = u_horizontalAngle/100.;
            if(hAngle>=0&&hAngle<360)
            {
                emit ptzHAngle(hAngle);
            }
        }

    }
}
bool PtzAnalyzer::SumCheck(QByteArray buffer)
{
    if(buffer.size() != 7) return false;
    uchar head = buffer.at(0);
    uchar hardid = buffer.at(1);
    uchar tail = buffer[buffer.size()-1];
    if(head != 0xff ||hardid != 0x01) return false;

    uchar sum = 0;
    for(int i =1;i<buffer.size()-1;i++)
    {
        sum += (uchar)buffer.at(i);
    }
    return sum == tail;
}
