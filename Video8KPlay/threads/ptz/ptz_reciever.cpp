 #include "ptz_reciever.h"
#include <QQueue>
#include <QMutex>
#include <QDebug>
#include <QTime>

QMutex Ptzmutex;
QMutex PtzQuemutex;
QQueue<QByteArray> PtzAnalyzingQueue;

PtzReciever::PtzReciever(QObject *parent) : DataReciever(parent)
{

}
void PtzReciever::DoRecive(QByteArray bytes)
{
   QByteArray data = bytes;
   if(data.isEmpty()) return;
   m_arrangData.append(data);
   int i = 0;
   uchar head1,head2;
   QByteArray segData;
   //qDebug()<<"in-----------------------";
   //qDebug()<<m_arrangData.size()<<":"<<m_arrangData.toHex()<<QTime::currentTime();
   while(i < m_arrangData.size() -1)
   {
       //qDebug()<<"in-----------------------";
       //qDebug()<<m_arrangData.size()<<":"<<m_arrangData.toHex();
       head1 = (uchar)m_arrangData.at(i);
       head2 = (uchar)m_arrangData.at(i+1);
       if(head1 == 0xff && head2 == 0x01)
       {
           int tail = i + 6;
           if(m_arrangData.size() - 1 > tail)
           {
               segData = m_arrangData.mid(i, 7);
               PtzQuemutex.lock();
               PtzAnalyzingQueue.enqueue(segData);
               PtzQuemutex.unlock();
               //qDebug()<<"insize1:"<<PtzAnalyzingQueue.size()<<QTime::currentTime();
               emit dataReady();
               //qDebug()<<i<<":inc1++++++++++++++++++++++++++";
               i = tail + 1;
               //qDebug()<<i<<":inc2++++++++++++++++++++++++++";
               if(i == m_arrangData.size() - 1)
               {
                   segData = m_arrangData.mid(i,1);
                   m_arrangData.clear();
                   m_arrangData = segData;
                   //qDebug()<<m_arrangData;
                   //qDebug()<<i<<":out1++++++++++++++++++++++++++";
                   break;
               }
           }
           else if(m_arrangData.size() -1 == tail)
           {
               segData = m_arrangData.mid(i, 7);
               PtzQuemutex.lock();
               PtzAnalyzingQueue.enqueue(segData);
               PtzQuemutex.unlock();
               //qDebug()<<"insize2:"<<PtzAnalyzingQueue.size()<<QTime::currentTime();
               //qDebug()<<m_arrangData;
               //qDebug()<<i<<":out2++++++++++++++++++++++++++";
               emit dataReady();
               m_arrangData.clear();
               break;
           }
           else
           {
               segData = m_arrangData.mid(i,m_arrangData.size() - i);
               m_arrangData.clear();
               m_arrangData = segData;
               //qDebug()<<m_arrangData.toHex();
               //qDebug()<<i<<":out3++++++++++++++++++++++++++";
               break;
           }
       }
       else
       {
           i++;
           //qDebug()<<i<<":out4++++++++++++++++++++++++++";
           if(i==m_arrangData.size()-1)
           {
               m_arrangData.clear();
               break;
           }
       }
   }
}
