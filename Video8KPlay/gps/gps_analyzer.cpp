#include "gps_analyzer.h"
#include <QQueue>
#include <QDebug>
#include <QThread>
#include <QMutex>
#include <QTime>
#include <QMutex>
#include <QTime>
#include <QSettings>
#include <QDir>
#include <QCoreApplication>

QMutex GpsMutex;
QMutex GpsQueMutex;
GpsData gps;
QQueue<QByteArray> GpsAnalyzingQueue;

GpsAnalyzer::GpsAnalyzer(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<GpsData>("GpsData");
    m_bCheck = true;
}
void GpsAnalyzer::DoAnalyze()
{

    while(!GpsAnalyzingQueue.isEmpty())
    {
        GpsQueMutex.lock();
        QByteArray data = GpsAnalyzingQueue.dequeue();
        GpsQueMutex.unlock();   

        if(!data.contains("HEADINGA")) {
            if(m_bCheck&&!CheckXor(data))
            {
                qDebug() << "[ERROR] gps data";
                continue;
            }
        }

//        emit showGPsMsg(data);
        qDebug()<<"gps_analyzer_gps_data:"<<data;

        GpsData gpsParser;
        //save the last data
        GpsMutex.lock();
        gpsParser = gps;
        double hight = gps.alt;
        QString tmpMsgData;
        if(data.contains(";") && data.contains("$HEADINGA")) {
            QString str = data;
            QStringList data_temp_list;
            data_temp_list = str.split(";");
            QString data_temp = "$HEADINGA,";
            if(data_temp_list.size() > 1) {
                data_temp += data_temp_list.at(1);
                tmpMsgData = data_temp;
            } else {
                continue;
            }
//            qDebug() << "heding a = " << data_temp;
//            qDebug() << "heding size = " << data_temp.split(",").size();
        } else {
            tmpMsgData = data;
        }

        QStringList strList = tmpMsgData.split(",");
        //updata gps data
        if(gpsParser.Parser(strList)&&gpsParser.Valid())
        {
 //           qDebug() << "gps parser ok";
            gpsParser.alt = hight;
            gps = gpsParser;
            emit GpsPotData(gpsParser);
        }
        else
        {
            qDebug() << "[ERROR] gps data";
        }
        GpsMutex.unlock();
    }
}

bool GpsAnalyzer::CheckXor(QByteArray data)
{
    QByteArray Array = data.mid(1,data.size() - 3);
    QByteArray checkArray = Array.mid(0,Array.size() - 3);
    uchar checkx = checkxor(checkArray);
    QByteArray v;
    v.setNum(checkx,16);
    QByteArray h =v.rightJustified(2,'0');
    QByteArray x = Array.mid(Array.size()-2,2);
    return (h.toUpper() == x)?true:false;
}

uchar GpsAnalyzer::checkxor(QByteArray data)
{
    uchar checkSum = 0;
    for(int i=0;i<data.size();i++)
    {
        QChar byte = data.at(i);
        uchar cel = byte.cell();
        checkSum ^= cel;
    }
    return checkSum;
}
