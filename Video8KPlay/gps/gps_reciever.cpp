#include "gps_reciever.h"
#include <QQueue>
#include <QMutex>
#include <QDebug>

extern QMutex GpsQueMutex;
extern QQueue<QByteArray> GpsAnalyzingQueue;

GpsReciever::GpsReciever(QObject *parent): DataReciever(parent)
{
    head = "$";
    tail = "\r\n";
}
void GpsReciever::DoRecive(QByteArray bytes)
{
    if(bytes.isEmpty()) return;
    if(bytes.contains("#")) {
        bytes.replace("#", "$");
    }

    m_arrangData.append(bytes);

    int Spos = 0;
    int Epos1 = 0;
    int Epos2 = 0;
    QByteArray segData;

    while(true)
    {
        Spos = m_arrangData.indexOf(head,Spos);
        if(Spos < 0)
        {
            m_arrangData.clear();
            break;
        }
        Epos1 = m_arrangData.indexOf(tail,Spos+head.size());
        Epos2 = m_arrangData.indexOf(head,Spos+head.size());

        if((Epos1 >0&&Epos1 <Epos2)||(Epos1 >0&&Epos2 <0))
        {
            segData = m_arrangData.mid(Spos,Epos1 - Spos + tail.size());
            Spos = Epos1 + tail.size();
            GpsQueMutex.lock();
            GpsAnalyzingQueue.enqueue(segData);
            GpsQueMutex.unlock();
            emit dataReady();
            qDebug() << "gps DoRecive = " << segData;
        }
        else if( (Epos1 < 0 && Epos2 >0)||(Epos2 >0&&Epos2 <Epos1) )
        {
           Spos = Epos2;
        }
        else
        {
             segData = m_arrangData.mid(Spos,m_arrangData.size()- Spos);
             m_arrangData.clear();
             m_arrangData = segData;
             break;
        }
    }
}
