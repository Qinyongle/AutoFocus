#include "ini/inifile.h"
#include "ptzinifile.h"
#include <QSettings>
#include <QCoreApplication>
#include <QDir>
#include <QDebug>

PtzIniFiles* PtzIniFiles::m_pPtzConfig = nullptr;
PtzIniFiles::PtzIniFiles()
{
    IniFile* config = IniFile::getConfig();
    for(int i =0;i<config->PtzNum();i++)
    {
        PtzIniFile* ptzCfg = new PtzIniFile(i+1);
        ptzCfg->ReadIniFile();
        vPtzConfig.append(ptzCfg);
    }
}
PtzIniFiles::~PtzIniFiles()
{
    for(int i =0;i< vPtzConfig.size();i++)
    {
        vPtzConfig[i]->WriteIniFile();
        delete vPtzConfig[i];
    }
}
PtzIniFile* PtzIniFiles::getPtzCfg(int ptzId)
{
    if(ptzId <= vPtzConfig.size())
    {
        return vPtzConfig[ptzId -1];
    }
    else
    {
        return nullptr;
    }
}
//*******************PtzIniFile*********************************/
PtzIniFile::PtzIniFile(int id)
{
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    m_nPtzId = id;
    m_ptzConfig = new QSettings("config.ini", QSettings::IniFormat);
}

PtzIniFile::~PtzIniFile()
{
    if(m_ptzConfig != nullptr)
    {
        delete m_ptzConfig;
        m_ptzConfig = nullptr;
    }
}
void PtzIniFile::WriteKeyValue(QString Key, int KeyValue)
{
    QString realKey;
    realKey.sprintf("PTZ%d/", m_nPtzId);
    realKey += Key;
    m_ptzConfig->setValue(realKey, KeyValue);
}
void PtzIniFile::WriteKeyValue(QString Key, double KeyValue)
{    
    QString realKey;
    realKey.sprintf("PTZ%d/", m_nPtzId);
    realKey += Key;
    m_ptzConfig->setValue(realKey, KeyValue);
}

void PtzIniFile::WriteKeyValue(QString Key, QVariant KeyValue)
{
    QString realKey;
    realKey.sprintf("PTZ%d/", m_nPtzId);
    realKey += Key;
    m_ptzConfig->setValue(realKey, KeyValue);
}

QString PtzIniFile::ReadKeyValue(QString Key)
{
    QString realKey;
    realKey.sprintf("PTZ%d/", m_nPtzId);
    realKey += Key;
    QString val = m_ptzConfig->value(realKey).toString();
    if(val.isEmpty())
    {
        WriteKeyValue(Key,"");
    }
    return val;
}
void PtzIniFile::ReadIniFile()
{
    m_strRunEnv     = ReadKeyValue("RunEnv");
    m_strCtrlMode   = ReadKeyValue("CtrlMode");
    m_strEquipType  = ReadKeyValue("EquipType");
    m_strDataSource = ReadKeyValue("DataSource");
    m_strCom        = ReadKeyValue("Com");
    m_strBaud       = ReadKeyValue("BaudRate");
    m_strTcpRole    = ReadKeyValue("TcpRole");
    m_strLocalIp    = ReadKeyValue("LocalIp");
    m_strPort       = ReadKeyValue("Port");
    m_strPeerIp     = ReadKeyValue("PeerIp");
    m_strAisMmsiNo  = ReadKeyValue("AisMmsiNo");
    m_strAisDelay   = ReadKeyValue("AisDelaySeconds");
    m_strvAmend     = ReadKeyValue("VerticalAmend");
    m_strServerIp   = ReadKeyValue("ServerIp");
    m_strServerPort = ReadKeyValue("ServerPort");
    m_strFanId      = ReadKeyValue("FanId");
    m_dLocalAlt     = ReadKeyValue("LocalAlt").toDouble();
    m_dLocalLat     = ReadKeyValue("LocalLat").toDouble();
    m_dLocalLng     = ReadKeyValue("LocalLng").toDouble();
    m_dPeerAlt      = ReadKeyValue("PeerAlt").toDouble();
    m_dPeerLat      = ReadKeyValue("PeerLat").toDouble();
    m_dPeerLng      = ReadKeyValue("PeerLng").toDouble();
    m_dPtzhOffset   = ReadKeyValue("PtzHOffSet").toDouble();
    m_dPtzvOffset   = ReadKeyValue("PtzVOffSet").toDouble();

    m_ptz_check_ip  = ReadKeyValue("PtzCheckIp");
    m_ptz_check_port  = ReadKeyValue("PtzCheckPort");

}

void PtzIniFile::WriteIniFile()
{
    WriteKeyValue("RunEnv",m_strRunEnv);
    WriteKeyValue("CtrlMode",m_strCtrlMode);
    WriteKeyValue("EquipType",m_strEquipType);
    WriteKeyValue("DataSource",m_strDataSource);
    WriteKeyValue("Com",m_strCom);
    WriteKeyValue("BaudRate",m_strBaud);
    WriteKeyValue("TcpRole",m_strTcpRole);
    WriteKeyValue("LocalIp",m_strLocalIp);
    WriteKeyValue("PeerIp",m_strPeerIp);
    WriteKeyValue("Port",m_strPort);
    WriteKeyValue("ServerIp",m_strServerIp);
    WriteKeyValue("ServerPort",m_strServerPort);
    WriteKeyValue("AisMmsiNo",m_strAisMmsiNo);
    WriteKeyValue("LocalAlt",m_dLocalAlt);
    WriteKeyValue("LocalLat",m_dLocalLat);
    WriteKeyValue("LocalLng",m_dLocalLng);
    WriteKeyValue("PeerAlt",m_dPeerAlt);
    WriteKeyValue("PeerLat",m_dPeerLat);
    WriteKeyValue("PeerLng",m_dPeerLng);
    WriteKeyValue("PtzHOffSet",m_dPtzhOffset);
    WriteKeyValue("PtzVOffSet",m_dPtzvOffset);

    WriteKeyValue("PtzCheckIp",m_ptz_check_ip);
    WriteKeyValue("PtzCheckPort",m_ptz_check_port);

}
