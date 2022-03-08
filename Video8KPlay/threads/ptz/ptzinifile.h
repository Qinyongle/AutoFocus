#ifndef PTZINIFILE_H
#define PTZINIFILE_H

#include<QString>
#include<QVariant>
#include <QMutex>
#include <QVector>
#include <QSettings>
#include <QMutex>
#include <iostream>
#include <iomanip>
using namespace std;

class PtzIniFile;

class PtzIniFiles
{
public:
    static PtzIniFiles* getConfig()
    {
        static QMutex logmutex;
        if(m_pPtzConfig == nullptr)
        {
            QMutexLocker locker(&logmutex);
            if(m_pPtzConfig == nullptr)
            {
                m_pPtzConfig = new PtzIniFiles();
            }
        }
        return m_pPtzConfig;
    }
    static void Release()                    //必须，否则会导致内存泄露
    {
        if(nullptr != m_pPtzConfig)
        {
            delete m_pPtzConfig;
            m_pPtzConfig = nullptr;
        }
    }
public slots:
    PtzIniFile* getPtzCfg(int ptzId);
private:
    PtzIniFiles();
    ~PtzIniFiles();

    static PtzIniFiles* m_pPtzConfig;
    QVector<PtzIniFile*> vPtzConfig;
};

class PtzIniFile
{
public:
    PtzIniFile(int id = 1);
    ~PtzIniFile();

    void WriteIniFile();
    void ReadIniFile();
    void WriteKeyValue(QString Key, int KeyValue);
    void WriteKeyValue( QString Key, double KeyValue);
    void WriteKeyValue(QString Key, QVariant KeyValue);
    QString ReadKeyValue(QString Key);

public:
    QString RunEnv(){return m_strRunEnv;}
    QString CtrlMode(){return m_strCtrlMode;}
    QString EquipType(){return m_strEquipType;}
    QString TcpRole(){return m_strTcpRole;}
    QString DataSource() {return m_strDataSource;}
    QString Com(){return m_strCom;}
    QString BaudRate(){return m_strBaud;}
    QString LocalIp(){return m_strLocalIp;}
    QString Port(){return m_strPort;}
    QString PeerIp() {return m_strPeerIp;}
    QString AisMmsiNo(){return m_strAisMmsiNo;}
    QString AisDelay(){return m_strAisDelay;}
    QString vAmend(){return m_strvAmend;}
    QString ServerIp(){return m_strServerIp;}
    QString ServerPort(){return m_strServerPort;}
    QString FanId(){return m_strFanId;}

    double LocalLng(){return m_dLocalLng;}
    double LocalLat(){return m_dLocalLat;}
    double LocalAlt(){return m_dLocalAlt;}
    double PeerLng(){return m_dPeerLng;}
    double PeerLat(){return m_dPeerLat;}
    double PeerAlt(){return m_dPeerAlt;}
    double PtzhOffset(){return m_dPtzhOffset;}
    double PtzvOffset(){return m_dPtzvOffset;}

    QString ptz_check_ip(){ return m_ptz_check_ip;}
    QString ptz_check_port(){ return m_ptz_check_port;}


public:
    void RunEnv(QString str) {m_strRunEnv = str;}
    void CtrlMode(QString str) {m_strCtrlMode = str;}
    void EquipType(QString str){m_strEquipType = str;}
    void TcpRole(QString str){m_strTcpRole = str;}
    void DataSource(QString str){m_strDataSource = str;}
    void Com(QString com) {m_strCom = com;}
    void BaudRate(QString baud){m_strBaud = baud;}
    void LocalIp(QString str){m_strLocalIp = str;}
    void Port(QString str){m_strPort = str;}
    void PeerIp(QString str){m_strPeerIp = str;}
    void AisDelay(QString str){ m_strAisDelay = str;}
    void vAmend(QString str){m_strvAmend = str;}
    void ServerIp(QString str){m_strServerIp = str;}
    void ServerPort(QString str){m_strServerPort =str;}
    void FanId(QString str){m_strFanId = str;}
    void AisMmsiNo(QString mmsi){m_strAisMmsiNo = mmsi;}

    void LocalLng(double lng){ m_dLocalLng = lng;}
    void LocalLat(double lat){ m_dLocalLat = lat;}
    void LocalAlt(double alt){ m_dLocalAlt = alt;}
    void PeerLng(double lng){ m_dPeerLng = lng;}
    void PeerLat(double lat){ m_dPeerLat = lat;}
    void PeerAlt(double alt){ m_dPeerAlt = alt;}
    void PtzhOffset(double h){ m_dPtzhOffset = h;}
    void PtzvOffset(double v){ m_dPtzvOffset = v;}

    void ptz_check_ip(QString ip){ m_ptz_check_ip = ip;}
    void ptz_check_port(QString port){ m_ptz_check_port = port;}

private:
    QSettings *m_ptzConfig;
    QString m_strRunEnv;
    QString m_strCtrlMode;
    QString m_strEquipType;

    QString m_strDataSource;
    QString m_strCom;
    QString m_strBaud;
    QString m_strTcpRole;
    QString m_strLocalIp;
    QString m_strPort;
    QString m_strAisMmsiNo;
    QString m_strAisDelay;
    QString m_strvAmend;
    QString m_strPeerIp;
    QString m_strServerIp;
    QString m_strServerPort;
    QString m_strFanId;

    double m_dLocalLng;
    double m_dLocalLat;
    double m_dLocalAlt;
    double m_dPeerLng;
    double m_dPeerLat;
    double m_dPeerAlt;
    double m_dPtzhOffset;
    double m_dPtzvOffset;
    int m_nPtzId;

    QString m_ptz_check_ip;
    QString m_ptz_check_port;


};

#endif // PTZINIFILE_H
