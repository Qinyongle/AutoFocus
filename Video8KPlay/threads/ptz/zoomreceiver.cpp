#include "zoomreceiver.h"
#include <QDebug>
ZoomReceiver::ZoomReceiver(QObject *parent) : QObject(parent)
{

}



void ZoomReceiver::DoRecive(QByteArray data)
{

    if(data.isEmpty()) {
        return;
    }
    m_arrangData.append(data);
    int i = 0;
    uchar head1,head2;
    QByteArray segData;
    while(i < m_arrangData.size() - 1) {
        head1 = (uchar)m_arrangData.at(i);
        head2 = (uchar)m_arrangData.at(i+1);
        if(head1 == 0xff && head2 == 0x01) {
            int tail = i + 6;
            if(m_arrangData.size() - 1 > tail)
            {
                segData = m_arrangData.mid(i, 7);
                DoAnalyze(segData);
                i = tail + 1;
                if(i == m_arrangData.size() - 1) {
                    segData = m_arrangData.mid(i,1);
                    m_arrangData.clear();
                    m_arrangData = segData;
                    break;
                }
            } else if(m_arrangData.size() -1 == tail) {
                segData = m_arrangData.mid(i, 7);
                DoAnalyze(segData);
                m_arrangData.clear();
                break;
            } else {
                segData = m_arrangData.mid(i,m_arrangData.size() - i);
                m_arrangData.clear();
                m_arrangData = segData;
                break;
            }
        } else {
            i++;
            if(i == m_arrangData.size() - 1) {
                m_arrangData.clear();
                break;
            }
        }
    }
}



void ZoomReceiver::DoAnalyze(QByteArray data)
{
    if(!SumCheck(data)) {
        return;
    }
    uchar cmd2 = (uchar)data.at(3);
    if(cmd2 != 0x6d) {
        return;
    }
    uchar hNum = (uchar)data.at(4);
    uchar lNum = (uchar)data.at(5);
    int num = (hNum << 8) + lNum;
    emit SendZoomNumber(num);
}

bool ZoomReceiver::SumCheck(QByteArray buffer)
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
