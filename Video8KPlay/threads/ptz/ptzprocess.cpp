#include "ptzprocess.h"
#include "gps/gps_reciever.h"
#include <cmath>
#include <QTime>
#include <QMutex>
#include <QQueue>
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSerialPortInfo>
#include <QAbstractSocket>

PtzProcess::PtzProcess(QObject *parent, int ID) : QObject(parent)

//    , m_auto_search(new auto_search(this))
//    , m_data_deal(new data_deal)
{
    qRegisterMetaType<ptzParam>();
    qRegisterMetaType<QSerialPort::DataBits>("QSerialPort::DataBits");
    qRegisterMetaType<QSerialPort::Parity>("QSerialPort::Parity");
    qRegisterMetaType<QSerialPort::StopBits>("QSerialPort::StopBits");
    qRegisterMetaType<QSerialPort::SerialPortError>("QSerialPort::SerialPortError");

    IniFile* config = IniFile::getConfig();
    m_gps_hoffset = config->GpsHOffset().toInt();
    m_id_check_switch = config->id_check_switch() == "true" ? true : false;

    m_nPtzId = ID;
    m_id_is_true = false;
    m_ptz_link_state = false;

    Init();
    LoadConfig();
//    connect(this,SIGNAL(AutoSendHCmd()),this,SLOT(SendPtzHCmd()));
//	connect(this,SIGNAL(AutoSendVCmd()),this,SLOT(SendPtzVCmd()));
    LoadPtz();
    Connectnet();

    // check


    m_timer_check_angle = new QTimer();
    connect(m_timer_check_angle,SIGNAL(timeout()),this,SLOT(ptz_no_response()));
    m_ptz_restart_times = 0;

    m_timer_check_socket_state = new QTimer();
    connect(m_timer_check_socket_state,SIGNAL(timeout()),this,SLOT(ptz_check_socket_state()));
    m_timer_check_socket_state->start(2000);
        // gps
    m_gps_lonlat_check_timer = new QTimer(this);
    connect(m_gps_lonlat_check_timer,SIGNAL(timeout()),this,SLOT(gps_latlon_check()));

    m_gps_angle_check_timer = new QTimer(this);
    connect(m_gps_angle_check_timer,SIGNAL(timeout()),this,SLOT(gps_angle_check()));

    m_gps_latlon_overtime = false;
    m_gps_angle_overtime = false;
 

}
void PtzProcess::LoadPtz()
{
    PtzIniFile* cfg = getPtzConfig();
    if(cfg->DataSource()=="net")
    {
        emit ptz_tcp_cloase();
        emit ptzSerialClose();
        ConnectPtzServer();
    }
    else if(cfg->DataSource()=="com")
    {
        emit ptz_tcp_cloase();
        emit ptzSerialClose();
        SetPtzComSlot();
    }
}
PtzProcess::~PtzProcess()
{
    SaveConfig();
   
    m_PtzClientThread.quit();
    m_PtzClientThread.wait();

    m_PtzRecieveThread.quit();
    m_PtzRecieveThread.wait();

    m_PtzAnalyzeThread.quit();
    m_PtzAnalyzeThread.wait();

    CalcuThread.quit();
    CalcuThread.wait();

    serverRecieveThread.quit();
    serverRecieveThread.wait();

    clientRecieveThread.quit();
    clientRecieveThread.wait();

    SerialPortThread.quit();
    SerialPortThread.wait();

    if(m_ptzParam!=nullptr)
    {
        delete m_ptzParam;
        m_ptzParam = nullptr;
    }
}
void PtzProcess::InitVarables()
{
    m_RunEnv = SHIP;
    m_CtrlType = AUTO;
    m_EquipType = NEW;
    m_TcpRole = CLIENT;

    m_mmsiNo ="";
    m_nAisDelaySecd = 60;
    m_bUseAisMsg = false;
    //varivale init
    m_bInitGps = true;

    m_Hspeed = 0X3F;//biggest speed
    m_Vspeed = 0X3F;
    m_bVAmend = false;
    m_dPtzScale = 0.5;
    m_dPtzPrehAngle = 0;
    m_bPtzSerialOpened = false;

    m_nPtzLostCount = 0;
    m_bPtzUsed = false;

    //net
    m_bClientConnected = false;
    m_nClientNetLostCount = 0;
    m_bServerConnected = false;
    m_nServerNetLostCount = 0;
}

void PtzProcess::Init()
{
    InitVarables();

    m_ptzParam = new ptzParam();
    //
    m_PtzAnalyzer = new PtzAnalyzer(m_nPtzId);
    m_PtzAnalyzer->moveToThread(&m_PtzAnalyzeThread);
    connect(&m_PtzAnalyzeThread,&QThread::finished,m_PtzAnalyzer,&QObject::deleteLater);
    connect(m_PtzAnalyzer,SIGNAL(ptzHAngle(double)),this,SLOT(getPtzHAngle(double)));
    connect(m_PtzAnalyzer,SIGNAL(ptzVAngle(double)),this,SLOT(getPtzVAngle(double)));
    connect(this, SIGNAL(sendEquipType(int)),m_PtzAnalyzer,SLOT(setEquipType(int)));
    connect(m_PtzAnalyzer,SIGNAL(LogRecord(int,LogMsgType,QStringList,QDateTime)),
            this,SIGNAL(LogRecord(int,LogMsgType,QStringList,QDateTime)));
    m_PtzAnalyzeThread.start();

    m_PtzReciever = new PtzReciever();
    m_PtzReciever->moveToThread(&m_PtzRecieveThread);
    connect(&m_PtzRecieveThread,&QThread::finished,m_PtzReciever,&QObject::deleteLater);
    connect(m_PtzReciever,SIGNAL(dataReady()),m_PtzAnalyzer,SLOT(DoAnalyze()));
    m_PtzRecieveThread.start();

    m_PtzSocket = new TcpSocket();
    m_PtzSocket->moveToThread(&m_PtzClientThread);
    connect(this,SIGNAL(ResetPtzClient(QString,int)),m_PtzSocket,SLOT(SetServer(QString,int)));
    connect(this,SIGNAL(SendPtzctrlbyNet(QByteArray)),m_PtzSocket,SLOT(SendData(QByteArray)));
    connect(this,SIGNAL(ptz_tcp_cloase()),m_PtzSocket,SLOT(disconnect()));
    connect(m_PtzSocket,SIGNAL(RecieveData(QByteArray)),m_PtzReciever,SLOT(DoRecive(QByteArray)));
    connect(m_PtzSocket,SIGNAL(Connected(QString,int)),this,SLOT(ptzConnected(QString,int)));
    connect(m_PtzSocket,SIGNAL(DisConnected(QString,int)),this,SLOT(ptzDisConnected(QString,int)));
    m_PtzClientThread.start();
    //client-reciever
    m_clientReciever = new JsonReciever();
    m_clientReciever->moveToThread(&clientRecieveThread);
    connect(&clientRecieveThread, &QThread::finished, m_clientReciever, &QObject::deleteLater);
    /*the parameter int in signal serverOrignal(int,QByteArray) is useless for
     * JsonReciever slot DoRecive(int,QByteArray),but is meanful in mainprocess slot SendPtzPeer(int,QByteArray),
     * put in this way is just for emit less signals;
    */
    connect(this,SIGNAL(serverOrignal(QByteArray)),m_clientReciever,SLOT(DoRecive(QByteArray)));
    connect(m_clientReciever,SIGNAL(sendJson(QByteArray)),this,SLOT(DoAnalyzeJson(QByteArray)));
    clientRecieveThread.start();
    //net-client
    m_tcpClient=new QTcpSocket(this);
    connect(m_tcpClient,SIGNAL(readyRead()),this,SLOT(ReadData()));
    connect(m_tcpClient,SIGNAL(connected()),this,SLOT(clientConnected()));
    connect(m_tcpClient,SIGNAL(disconnected()),this,SLOT(clientDisConnected()));
    //server-reciever
    m_serverReciever = new JsonReciever();
    m_serverReciever->moveToThread(&serverRecieveThread);
    connect(&serverRecieveThread, &QThread::finished, m_serverReciever, &QObject::deleteLater);
    connect(this,SIGNAL(clientOrignal(QByteArray)),m_serverReciever,SLOT(DoRecive(QByteArray)));
    connect(m_serverReciever,SIGNAL(sendJson(QByteArray)),this,SLOT(DoAnalyzeJson(QByteArray)));
    serverRecieveThread.start();
    //net-server
    m_tcpServer=new TcpServer(this);
    connect(m_tcpServer,SIGNAL(ClientConnected(int,QString,int)),
            this,SLOT(ClientConnect(int,QString,int)));
    connect(m_tcpServer,SIGNAL(ClientDisConnected(int,QString,int)),
            this,SLOT(ClientDisConnect(int,QString,int)));
    connect(m_tcpServer,SIGNAL(ClientReadData(int,QByteArray)),
            this,SLOT(ClientReadData(int,QByteArray)));


	m_Calcute = new CalculateAngle();
    m_Calcute->moveToThread(&CalcuThread);
    connect(&CalcuThread,&QThread::finished,m_Calcute,&QObject::deleteLater);
    connect(this,SIGNAL(dataUpdate(ptzParam)),m_Calcute,SLOT(Calculate(ptzParam)));
    connect(m_Calcute,SIGNAL(sendHorizon(double)),this, SLOT(ToHAngle(double)));
    connect(m_Calcute,SIGNAL(sendVertical(double)),this,SLOT(ToVAngle(double)));
    connect(m_Calcute,SIGNAL(sendDistance(double)),this,SLOT(Distance(double)));
    CalcuThread.start();
	
    //serial port
    m_ptzSerialPort = new SerialPort();
    m_ptzSerialPort->moveToThread(&SerialPortThread);
    connect(&SerialPortThread,&QThread::finished,m_ptzSerialPort,&QObject::deleteLater);

    connect(this, SIGNAL(ptzSerialOpen( QString,
                               int,
                               QSerialPort::DataBits,
                               QSerialPort::Parity,
                               QSerialPort::StopBits)),
      m_ptzSerialPort, SLOT(open( QString,
                               int,
                               QSerialPort::DataBits,
                               QSerialPort::Parity,
                               QSerialPort::StopBits)));

    connect(m_ptzSerialPort,SIGNAL(controlOpened(bool)),this,SLOT(ControlOpened(bool)));
    connect(m_ptzSerialPort,SIGNAL(controlClosed(bool)),this,SLOT(ControlClosed(bool)));
    connect(m_ptzSerialPort,SIGNAL(ReadData(QByteArray)),m_PtzReciever,SLOT(DoRecive(QByteArray)));
    connect(this, SIGNAL(ptzSerialClose()), m_ptzSerialPort, SLOT(close()));
    connect(this, SIGNAL(ptzSerialSend(QByteArray)), m_ptzSerialPort, SLOT(send(QByteArray)));
    connect(this, SIGNAL(ptzSetBaudRate(int)), m_ptzSerialPort, SLOT(setBaudRate(int)));
    SerialPortThread.start();
    //calculate
    connect(this,SIGNAL(AutoSendHCmd()),this,SLOT(SendPtzHCmd()));
    connect(this,SIGNAL(AutoSendVCmd()),this,SLOT(SendPtzVCmd()));

    m_ptz_check_socket = new TcpSocket(this);

    // wireless
//    m_wireless = new wireless(this);
////    m_wireless->moveToThread(&wireless_thread);
//    connect(&wireless_thread,&QThread::finished,m_wireless,&QObject::deleteLater);
//    connect(m_wireless,SIGNAL(send_wireless_data(WIRELESS_DATA)),
//            m_auto_search,SLOT(get_wireless_data(WIRELESS_DATA)));
//    connect(m_wireless,SIGNAL(send_wireless_data(WIRELESS_DATA)),
//            m_data_deal,SLOT(get_wireless_data(WIRELESS_DATA)));
//    connect(this,SIGNAL(send_wireless_reconfig()),
//            m_wireless,SLOT(reconfig()));
////    wireless_thread.start();

//    // auto search
//    connect(m_auto_search,SIGNAL(send_set_tx_power(int)),
//            m_wireless,SLOT(set_wireless_power(int)));
//    connect(m_auto_search,SIGNAL(send_h_angle(double)),
//            this,SLOT(ToHAngle(double)));
//    connect(m_auto_search,SIGNAL(send_connected_ptz_angle(double)),
//            m_data_deal,SLOT(get_connected_ptz_angle(double)));
//    connect(m_auto_search,SIGNAL(send_charge_state(bool)),
//            m_data_deal,SLOT(get_find_state(bool)));
////    connect(m_auto_search,SIGNAL(send_wireless_signal(int)),
////            m_data_deal,SLOT(get_connected_ptz_angle(double)));
//    connect(this,SIGNAL(send_auto_search_switch(bool)),
//            m_auto_search,SLOT(get_auto_search_switch(bool)));
//    connect(this,SIGNAL(send_auto_search_reconfig()),
//            m_auto_search,SLOT(reconfig()));
//    m_auto_search->start();

//    // data deal
//    m_data_deal->moveToThread(&data_deal_thread);
//    connect(&data_deal_thread,&QThread::finished,m_data_deal,&QObject::deleteLater);
//    connect(m_data_deal,SIGNAL(send_set_tx_power(int)),
//            m_wireless,SLOT(set_wireless_power(int)));
//    connect(m_data_deal,SIGNAL(send_h_angle(double)),
//            this,SLOT(ToHAngle(double)));
//    connect(m_data_deal,SIGNAL(send_find_again(bool)),
//            m_auto_search,SLOT(get_find_again(bool)));
//    //void send_hoffset(double);
//    connect(m_data_deal,SIGNAL(send_hoffset(double)),
//            this,SLOT(get_ptz_hoffset_reconfig(double)));
//    connect(this,SIGNAL(send_auto_search_switch(bool)),
//            m_data_deal,SLOT(get_auto_search_switch(bool)));
//    connect(this,SIGNAL(send_data_deal_reconfig()),
//            m_data_deal,SLOT(reconfig()));
//    data_deal_thread.start();

    //timer
    m_timerSendNetMsg = new QTimer(this);
    connect(m_timerSendNetMsg,SIGNAL(timeout()),this,SLOT(SendNetMsg()));
    m_timerSendNetMsg->start(500);

    m_timerReport = new QTimer(this);
    connect(m_timerReport,SIGNAL(timeout()),this,SLOT(Report()));
    m_timerReport->start(5000);

    m_timerStateCheck = new QTimer(this);
    connect(m_timerStateCheck,SIGNAL(timeout()),this,SLOT(StateCheck()));
    m_timerStateCheck->start(1000);

    m_timerAisPos = new QTimer(this);
    connect(m_timerAisPos,SIGNAL(timeout()),this,SLOT(EnableAisPos()));

    m_timerScreen = new QTimer(this);
    connect(m_timerScreen,SIGNAL(timeout()),this,SLOT(SendPtzEnvrt()));
    m_timerScreen->start(1500);


}
PtzIniFile* PtzProcess::getPtzConfig()
{
    PtzIniFiles* cfgs = PtzIniFiles::getConfig();
    return cfgs->getPtzCfg(m_nPtzId);
}

void PtzProcess::LoadConfig()
{
    PtzIniFile* cfg = getPtzConfig();

    if(cfg->vAmend().toLower()=="yes")
        m_bVAmend = true;
    else
        m_bVAmend = false;

    if(cfg->RunEnv().toLower()=="ship") {
        m_RunEnv = SHIP;
    } else if(cfg->RunEnv().toLower()=="bank") {
        m_RunEnv = BANK;
    }

    if(cfg->CtrlMode().toLower()=="auto")
    {
        m_CtrlType = AUTO;
    } else if(cfg->CtrlMode().toLower()=="manual") {
        m_CtrlType =  MANUAL;
    }

    if(cfg->EquipType().toLower()=="big") {
        m_EquipType = OLD;
    } else if(cfg->EquipType().toLower()=="small") {
        m_EquipType = NEW;
    }

    if(cfg->TcpRole().toLower()=="client")
    {
        m_TcpRole = CLIENT;
    } else if(cfg->TcpRole().toLower()=="server") {
        m_TcpRole = SERVER;
    }

    m_mmsiNo =cfg->AisMmsiNo();
    m_nAisDelaySecd = cfg->AisDelay().toInt();
    m_nAisDelaySecd =(m_nAisDelaySecd< 60)?60:m_nAisDelaySecd;

    ReadPtzParam();

    m_ptz_check_socket_ip = cfg->ptz_check_ip();
    m_ptz_check_socket_port = cfg->ptz_check_port();
}
void PtzProcess::ReadPtzParam()
{
    PtzIniFile* cfg = getPtzConfig();

    m_ptzParam->LocalLng(cfg->LocalLng()); gps.lng = cfg->LocalLng();
    m_ptzParam->LocalLat(cfg->LocalLat()); gps.lat = cfg->LocalLat();
    m_ptzParam->LocalAlt(cfg->LocalAlt()); gps.alt = cfg->LocalAlt();

 	m_ptzParam->PtzHOffset(cfg->PtzhOffset());
    m_ptzParam->PtzVOffset(cfg->PtzvOffset());
    m_ptzParam->PeerLng(cfg->PeerLng());
	
    m_ptzParam->PeerLat(cfg->PeerLat());
	
    m_ptzParam->PeerAlt(cfg->PeerAlt());

    emit dataUpdate(*m_ptzParam);
}

void PtzProcess::get_ptz_hoffset_reconfig(double angle)
{
    m_ptzParam->PtzHOffset(angle);
}

void PtzProcess::SaveConfig()
{
    PtzIniFile* cfg = getPtzConfig();

    cfg->PtzhOffset(m_ptzParam->PtzHOffset());
    cfg->PtzvOffset(m_ptzParam->PtzVOffset());
    cfg->LocalLng(m_ptzParam->LocalLng());
    cfg->LocalLat(m_ptzParam->LocalLat());
    cfg->LocalAlt(m_ptzParam->LocalAlt());
    cfg->PeerLng(m_ptzParam->PeerLng());
    cfg->PeerLat(m_ptzParam->PeerLat());
    cfg->PeerAlt(m_ptzParam->PeerAlt());
}

void PtzProcess::getAisPos(QString mmsino,double lng, double lat)
{
    if(!m_bUseAisMsg) return;
    if(m_mmsiNo !=mmsino) return;
    //qDebug()<<"bingo--------------------"<<mmsino<<QString::number(lng,'f',7)<<QString::number(lat,'f',7);
    Ptzmutex.lock();
    m_ptzParam->PeerLng(lng);
    m_ptzParam->PeerLat(lat);
    Ptzmutex.unlock();
    emit dataUpdate(*m_ptzParam);
}

void PtzProcess::EnableAisPos()
{
    m_bUseAisMsg = true;
}

void PtzProcess::ToHAngle(double h)
{
    m_ptzParam->ToPtzHAngle(h);
    emit AutoSendHCmd();

}

void PtzProcess::ToVAngle(double v)
{
    m_ptzParam->ToPtzVAngle(v);
    emit AutoSendVCmd();
}

void PtzProcess::Distance(double d)
{
    m_ptzParam->Distance(d);
    SendPtzParam(); //finish calculate
}

void PtzProcess::ptzConnected(QString ip,int port)
{
    QStringList list;
    list<<QString::number(m_nPtzId)<<"connected"<<ip<<QString::number(port);
    LogRecord(21,OPERATOR,list,QDateTime::currentDateTime());
}

void PtzProcess::ptzDisConnected(QString ip,int port)
{
    QStringList list;
    list<<QString::number(m_nPtzId)<<"disconnected"<<ip<<QString::number(port);
    LogRecord(21,OPERATOR,list,QDateTime::currentDateTime());
}

void PtzProcess::ConnectPtzServer()
{
    PtzIniFile* cfg = getPtzConfig();
    QString strPtzIp = cfg->ServerIp();
    int PtzPort = cfg->ServerPort().toInt();
    //qDebug()<<strPtzIp<<","<<PtzPort;
    if (strPtzIp.isEmpty()||PtzPort==0) {
        LogRecord(25,ERROR,QStringList(), QDateTime());
        return;
    }
    emit ResetPtzClient(strPtzIp,PtzPort);
}
void PtzProcess::ConnectPeerServer()
{   
    PtzIniFile* cfg = getPtzConfig();
    QString strTcpIp = cfg->PeerIp();//
    QString strTcpPort = cfg->Port();//
	if(strTcpIp.isEmpty()||strTcpPort.isEmpty()) return;
    if( (m_tcpClient->state()==QAbstractSocket::ConnectingState)
            ||(m_tcpClient->state()==QAbstractSocket::ConnectedState) ) {
        m_tcpClient->abort();
    }
    m_tcpClient->connectToHost(strTcpIp,strTcpPort.toUShort());
}

void PtzProcess::LocalServerListen()
{
    PtzIniFile* cfg = getPtzConfig();
    QString m_strTcpPort = cfg->Port();
    LogRecord(23,OPERATOR,QStringList()<<QString::number(m_nPtzId)<<m_strTcpPort,QDateTime::currentDateTime());
    m_tcpServer->close();
    m_tcpServer->clear();
    m_tcpServer->listen(QHostAddress::Any,m_strTcpPort.toUShort());
}

void PtzProcess::getPtzHAngle(double hAngle)
{
    m_bPtzUsed = true;
    m_ptz_link_state = true;
    m_nPtzLostCount = 0;

    Ptzmutex.lock();
    m_ptzParam->PtzHAngle(hAngle);
    Ptzmutex.unlock();
    emit AutoSendHCmd();
}
void PtzProcess::getPtzVAngle(double vAngle)
{
    m_bPtzUsed = true;
    m_nPtzLostCount = 0;

    Ptzmutex.lock();
    m_ptzParam->PtzVAngle(vAngle);
    Ptzmutex.unlock();
    emit AutoSendVCmd();
}
void PtzProcess::answerEquipType()
{
    if(m_EquipType == OLD) {
        emit sendEquipType(1);
    } else {
        emit sendEquipType(0);
    }
}

void PtzProcess::SetPtzComSlot()
{
    if(!m_bPtzSerialOpened) {
        QString str;
#ifdef WINDOWS
        str = "\\\\.\\";
#else
        str ="";
#endif
        PtzIniFile* cfg = getPtzConfig();
        QString com = cfg->Com();

        QString baud = cfg->BaudRate();
        str += com;
        emit ptzSerialOpen(str, baud.toInt(),QSerialPort::Data8,
                  QSerialPort::NoParity,QSerialPort::OneStop);
    } else {
        QString str;
#ifdef WINDOWS
        str = "\\\\.\\";
#else
        str ="";
#endif
        PtzIniFile* cfg = getPtzConfig();
        QString com = cfg->Com();

        QString baud = cfg->BaudRate();
        str += com;
        emit ptzSerialOpen(str, baud.toInt(),QSerialPort::Data8,
                  QSerialPort::NoParity,QSerialPort::OneStop);
    }


}
void PtzProcess::ControlOpened(const bool bOpened)
{
    if(!bOpened) {
        LogRecord(21,ERROR,QStringList()<<QString::number(m_nPtzId),QDateTime::currentDateTime());
    } else {
        PtzIniFile* cfg = getPtzConfig();
        QString com = cfg->Com();

        QString baud = cfg->BaudRate();
        QStringList list;
        list << QString::number(m_nPtzId)<<com<<baud;
        LogRecord(21,OPERATOR,list,QDateTime::currentDateTime());
        m_bPtzSerialOpened = true;
    }
}

void PtzProcess::ControlClosed(bool bClosed)
{
    if (!bClosed) {
        LogRecord(22,ERROR,QStringList()<<QString::number(m_nPtzId),QDateTime::currentDateTime());
    } else {
        LogRecord(22,OPERATOR,QStringList()<<QString::number(m_nPtzId),QDateTime::currentDateTime());
        m_bPtzSerialOpened = false;
    }
}

void PtzProcess::Connectnet()
{
    PtzIniFile* cfg = getPtzConfig();

    QString strTcpPort = cfg->Port();
    if(m_TcpRole == CLIENT) {
        QString strTcpIp = cfg->PeerIp();

        if (strTcpIp.isEmpty()) {
            qDebug()<<"ptz-server ip invalid.";
            return;
        }
        m_tcpClient->abort();
        m_tcpClient->connectToHost(strTcpIp,strTcpPort.toUShort());
        QStringList list;
        list<<QString::number(m_nPtzId)<<"connect to"<<strTcpIp<<strTcpPort;
        LogRecord(25,OPERATOR, list, QDateTime::currentDateTime());
    }
    if(m_TcpRole == SERVER) {
        if(!strTcpPort.isEmpty()) {
            m_tcpServer->close();
            m_tcpServer->clear();
            m_tcpServer->listen(QHostAddress::Any,strTcpPort.toUShort());
        } else {
            m_tcpServer->listen(QHostAddress::Any,6060);
            cfg->Port(QString::number(6060));
            strTcpPort = "6060";
        }
        QStringList list;
        list<<QString::number(m_nPtzId)<<strTcpPort;
        LogRecord(23,OPERATOR, list, QDateTime::currentDateTime());
    } 
}

void PtzProcess::DisConnect()
{
    if(m_TcpRole == CLIENT) {
        QString ip = m_tcpClient->peerAddress().toString();
        int port = m_tcpClient->peerPort();
        m_tcpClient->abort();
        QStringList list;
        list<<QString::number(m_nPtzId)<<"disconnect to"<<ip<<QString::number(port);
        LogRecord(25,OPERATOR,list,QDateTime::currentDateTime());
    }

    if(m_TcpRole == SERVER) {
        m_tcpServer->close();
        m_tcpServer->clear();
        LogRecord(24,OPERATOR,QStringList()<<QString::number(m_nPtzId),QDateTime::currentDateTime());
    }
}


void PtzProcess::GpsPotUpDate(GpsData msg)
{
    // 如果身份认证没有通过，则不控制云台
    if(!m_id_is_true && m_id_check_switch) return;

    if(msg.lng == 0.0 || msg.lat == 0.0) {
        return;
    }

    if(m_ptzParam->LocalLng() > 0 || m_ptzParam->LocalLat() > 0) {
        if(fabs(msg.lng - m_ptzParam->LocalLng()) > 1.0
                ||fabs(msg.lat - m_ptzParam->LocalLat()) > 1.0 ) {
            if(m_bInitGps)
                m_bInitGps = false;
            else {
                if(!m_gps_latlon_overtime) {
                    m_gps_latlon_overtime = true;
                    m_gps_lonlat_check_timer->start(1 * 60 * 1000);
                } else {
                    return;
                }
            }
        }
    } else {
        m_gps_lonlat_check_timer->stop();
        m_gps_latlon_overtime = false;
    }

    if(m_ptzParam->Heading() < 10.0 && msg.heading > 350.0) {

    } else {
        if(m_ptzParam->Heading() > 0.0) {
            if(fabs(msg.heading - m_ptzParam->Heading()) > m_gps_hoffset) {
                if(m_bInitGpsAngle)
                    m_bInitGpsAngle = false;
                else {
                    if(!m_gps_angle_overtime) {
                        m_gps_angle_overtime = true;
                        m_gps_angle_check_timer->start(1 * 60 * 1000);
                    } else {
                        return;
                    }
                }
            }
        }
    }
    Ptzmutex.lock();
    m_ptzParam->local = msg;
    Ptzmutex.unlock();
    emit dataUpdate(*m_ptzParam);
}

void PtzProcess::ClientReadData(int,QByteArray data)
{
    if (!data.isEmpty()) {
        m_timerAisPos->stop();
        m_bUseAisMsg = false;
        m_bServerConnected = true;
        m_nServerNetLostCount = 0;
        emit clientOrignal(data);
        emit clientOrignal(m_nPtzId,data);
    }
}
void PtzProcess::ClientConnect(int,QString IP,int Port)
{
    int pos = IP.lastIndexOf(":");
    IP = IP.mid(pos+1);
    QStringList list;
    list<<IP<<QString::number(Port)<<" on line";
    emit LogRecord(21,INFORM,list, QDateTime::currentDateTime());
}

void PtzProcess::ClientDisConnect(int,QString IP,int Port)
{
    int pos = IP.lastIndexOf(":");
    IP = IP.mid(pos+1);
    QStringList list;
    list<<IP<<QString::number(Port)<<" off line";
    emit LogRecord(21,INFORM,list,QDateTime::currentDateTime());
}

void PtzProcess::clientConnected()
{
    QStringList list;
    list<<"connected to";
    list << m_tcpClient->peerAddress().toString();
    list << QString::number(m_tcpClient->peerPort());
    emit LogRecord(25,OPERATOR,list,QDateTime::currentDateTime());
}

void PtzProcess::clientDisConnected()
{
    QStringList list;
    list<<"disconnected to";
    list << m_tcpClient->peerAddress().toString();
    list << QString::number( m_tcpClient->peerPort());
    emit LogRecord(25,OPERATOR,list,QDateTime::currentDateTime());
}

void PtzProcess::ReadData()
{
    QByteArray buffer= m_tcpClient->readAll();
    if (!buffer.isEmpty()) {
        m_timerAisPos->stop();
        m_bUseAisMsg = false;
        m_bClientConnected = true;
        m_nClientNetLostCount = 0;
        emit serverOrignal(buffer);
        emit serverOrignal(m_nPtzId,buffer);
    }
}

void PtzProcess::SendPtzCmd(QByteArray Array)
{
    if(m_bPtzSerialOpened)
        emit ptzSerialSend(Array);
    else if(m_PtzSocket->state()==QAbstractSocket::ConnectedState)
        emit SendPtzctrlbyNet(Array);
    else
        qDebug()<<"ptz is unlinked.";
}

void PtzProcess::StateCheck()
{
    PtzIniFile* cfg = getPtzConfig();
    m_nPtzLostCount++;
    if(m_nPtzLostCount > PTZ_LOST_LIMIT) {
        m_nPtzLostCount = 0;
        m_bPtzUsed = false;
        if(cfg->DataSource()=="net") {
            ConnectPtzServer();
        }
    }
    if(m_TcpRole==CLIENT) {
        m_nClientNetLostCount++;
        if(m_nClientNetLostCount > NET_LOST_LIMIT) {
            m_bClientConnected = false;
            m_nClientNetLostCount = 0;
            ConnectPeerServer();
            if(!m_timerAisPos->isActive()) {
                m_timerAisPos->start(m_nAisDelaySecd*1000);
            }
        }
    }
    if(m_TcpRole==SERVER) {
        m_nServerNetLostCount++;
        if(m_nServerNetLostCount > NET_LOST_LIMIT) {
            m_bServerConnected = false;
            m_nServerNetLostCount = 0;
            if(!m_timerAisPos->isActive()) {
                m_timerAisPos->start(m_nAisDelaySecd*1000);
            }
        }

        QString strTcpPort = cfg->Port();
        if(!strTcpPort.isEmpty()
                && m_tcpServer->serverPort()!=strTcpPort.toInt()) {
            static int serverLostCount;
            serverLostCount++;
            if(serverLostCount>10) {
                LocalServerListen();
                serverLostCount = 0;
            }
        }
    }
}
void PtzProcess::stopRun()
{
    PtzProtocol ptzPro;
    QByteArray array = ptzPro.StopPtz();
    SendPtzCmd(array);
}

void PtzProcess::SendPtzHCmd()
{
    SendPtzParam();
    if(m_CtrlType == MANUAL) {
        m_timer_check_angle->stop();
        return;
    }

    double h1 = m_ptzParam->PtzHAngle();
    double h0 = m_ptzParam->ToPtzHAngle();
    if(fabs(h1 - h0) < m_dPtzScale) {
        if(m_dPtzPrehAngle != h1) {
            stopRun();
            m_dPtzPrehAngle = h1;
        }
        return;
    }

    ushort data = m_ptzParam->ToPtzHAngle()*100;
    uchar dataH = data >>8&0xff;
    uchar dataL = data&0xff;
    PtzProtocol ptzPro;
    QByteArray array = ptzPro.SetHorizontalAngle(dataH, dataL);

    // check
    static bool is_could_timing = true;
    if(m_setptz_angle == QString::number(h1, 10, 0).toInt() ||
            m_setptz_angle == QString::number(h1, 10, 0).toInt() - 1 ||
            m_setptz_angle == QString::number(h1, 10, 0).toInt() + 1) {
        m_timer_check_angle->stop();
        m_ptz_restart_times = 0;
        is_could_timing = true;
    }
    if(m_setptz_angle != QString::number(h0, 10, 0).toInt()) {
        m_setptz_angle = QString::number(h0, 10, 0).toInt();
        if(is_could_timing) {
            m_timer_check_angle->start(1000 * 60 * 1);
            is_could_timing = false;
        }
    }

    SendPtzCmd(array);
}

void PtzProcess::SetPtzSendBack(bool bBack)
{
    uchar data =116;
    QByteArray Array;
    PtzProtocol ptzPro;
    if(bBack) {
        Array = ptzPro.LoadPresettingbit(data);
    } else {
        Array = ptzPro.SetPresettingbit(data);
    }
    SendPtzCmd(Array);
}

void PtzProcess::SendPtzVCmd()
{
    SendPtzParam();
    if(m_CtrlType != AUTO) {
        return;
    }
    if(!m_bVAmend) return;

//    qDebug() << "SendPtzVCmd";
    double v1 = m_ptzParam->PtzVAngle();
    double v0 = m_ptzParam->ToPtzVAngle();
    double dlt = fabs(v1 - v0);
    if(fmod(dlt,360) < m_dPtzScale) return;
    
    if(v0 <0) {
        v0 = -1*v0;
    } else {
        v0 = 360 -v0;
    }

    ushort  data = v0*100;
    uchar dataH = data >>8&0xff;
    uchar dataL = data&0xff;
    PtzProtocol ptzPro;
    QByteArray varray = ptzPro.SetVerticalAngle(dataH, dataL);

    SendPtzCmd(varray);
}

// net
void PtzProcess::SendNetMsg()
{
    QJsonObject json;
    json.insert("msgType","Ptz_PeerMsg");
    if(m_RunEnv == SHIP)
        json.insert("runEnv","ship");
    else
        json.insert("runEnv","bank");

    json.insert("heading",QString::number(m_ptzParam->Heading(),'f',2));
    json.insert("pitch",QString::number(m_ptzParam->Pitch(),'f',3));
    json.insert("hOffset",QString::number(m_ptzParam->PtzHOffset(),'f',2));
    json.insert("vOffset",QString::number(m_ptzParam->PtzVOffset(),'f',2));
    json.insert("lng",QString::number(m_ptzParam->LocalLng(),'f',7));
    json.insert("lat",QString::number(m_ptzParam->LocalLat(),'f',7));
    json.insert("alt",QString::number(m_ptzParam->LocalAlt(),'f',2));

    if(m_bPtzUsed)
        json.insert("ptzPassback","on");
    else
        json.insert("ptzPassback","off");

    json.insert("peerId",m_nPtzId);

    QByteArray msg = QJsonDocument(json).toJson(QJsonDocument::Compact);
    if(m_TcpRole == CLIENT) {
        if(m_tcpClient->isOpen()) {
            m_tcpClient->write(msg);
        }
    }
    if(m_TcpRole == SERVER) {
        m_tcpServer->SendDataAll(msg.data(),msg.size());
    }
}

// web
void PtzProcess::SendWebOrderRsp()
{
    PtzIniFile* cfg = getPtzConfig();
    QJsonObject json;
    json.insert("msgType","webCtrlRsp");
    json.insert("fanId",cfg->FanId().toInt());
    emit WebRspMessage(json);
}

void PtzProcess::Report()
{
    PtzIniFile* cfg = getPtzConfig();
    QJsonObject json;
    json.insert("msgType","ptzReport");
    json.insert("fanId",cfg->FanId());
    if(m_RunEnv == SHIP)
        json.insert("runEnv","ship");
    else
        json.insert("runEnv","bank");
    json.insert("lng",m_ptzParam->LocalLng());
    json.insert("lat",m_ptzParam->LocalLat());
    qDebug() << "Report() = " << json;
    emit ReportState(json);
}

void PtzProcess::DoAnalyzeJson(QByteArray data)
{
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(data, &err);
    QFile file("peerJasonErr.txt");
    if(err.error != QJsonParseError::NoError) {
        if(file.open(QIODevice::WriteOnly | QIODevice::Text|QIODevice::Append)) {
            static int errCount=0;errCount++;
            QByteArray rec; rec.append("err");rec.append(QString::number(errCount));rec.append("_");
            QString str = QTime::currentTime().toString("hh:mm:ss");
            rec.append(str);
            rec.append(data);
            rec.append('\n');
            file.write(rec);
            file.close();
        }
        return;
    }
    QJsonObject  json = doc.object();
    if(json.value("msgType")=="Ptz_PeerMsg"){
        PeerInformMsgAnalyze(json);
    }
}
void PtzProcess::PeerInformMsgAnalyze(QJsonObject&  json)
{
    QString strRunEnv = json.value("runEnv").toString();
    if(strRunEnv =="ship"){
        m_PeerRunEnv = SHIP;
    } else{
        m_PeerRunEnv = BANK;
    }
    double lng = m_ptzParam->PeerLng();
    double lat = m_ptzParam->PeerLat();
    double alt = m_ptzParam->PeerAlt();
	QString str;
    Ptzmutex.lock();
    str = json.value("lng").toString();
    if(!str.isEmpty()) {
        m_ptzParam->PeerLng(str.toDouble());
    }
    str =json.value("lat").toString();
    if(!str.isEmpty()) {
        m_ptzParam->PeerLat(str.toDouble());
    }
    str = json.value("alt").toString();
    if(!str.isEmpty()) {
        m_ptzParam->PeerAlt(str.toDouble());
    }

    if(fabs(lng - m_ptzParam->PeerLng()) > 0.00000005
        ||fabs(lat - m_ptzParam->PeerLat()) > 0.00000005
        ||fabs(alt - m_ptzParam->PeerAlt()) >= 0.5 ) {
        emit dataUpdate(*m_ptzParam);
    }
    Ptzmutex.unlock();
}

void PtzProcess::resetGpsSignal()
{
    m_bInitGps = true;
}

void PtzProcess::SendPtzEnvrtSyn(int id)
{
    if(id != m_nPtzId) return;
    SendPtzEnvrt();
    SendPtzParam();
}

void PtzProcess::SendPtzEnvrt()
{
    PtzIniFile* cfg = getPtzConfig();
    QJsonObject json;
    json.insert("msgType","Ptz_Envrt");
    json.insert("ptzId",m_nPtzId);
    json.insert("fanId",cfg->FanId());
    if(m_RunEnv == SHIP)
        json.insert("runEnv","ship");
    else
        json.insert("runEnv","bank");

    if(m_CtrlType == AUTO)
        json.insert("ctrlMode","auto");
    else
        json.insert("ctrlMode","manual");

    if(m_EquipType == NEW)
        json.insert("equip","small");
    else
        json.insert("equip","big");

    if(m_TcpRole == CLIENT) {
        json.insert("tcpRole","client");
        json.insert("netIp",cfg->PeerIp());
    } else {
        json.insert("tcpRole","server");
        json.insert("netIp",cfg->LocalIp());
    }

    json.insert("verticalAmend",cfg->vAmend());
    json.insert("port",cfg->Port());
    json.insert("dataSource",cfg->DataSource());
    json.insert("com",cfg->Com());
    json.insert("baudRate",cfg->BaudRate());
    json.insert("serverIp",cfg->ServerIp());
    json.insert("serverPort",cfg->ServerPort());
    json.insert("aisMmsino",cfg->AisMmsiNo());
    json.insert("ptzCheckIp",m_ptz_check_socket_ip);
    json.insert("ptzCheckPort",m_ptz_check_socket_port);
    if(m_ptz_check_link_state) {
        json.insert("ptzCheckLinkState","true");
    } else {
        json.insert("ptzCheckLinkState","false");
    }
    if(m_ptz_link_state) {
        json.insert("ptzLinkState","true");
    } else {
        json.insert("ptzLinkState","false");
    }

    emit PtzEnvrtReport(json);
}
void PtzProcess::SendPtzParam()
{
    QJsonObject json;
    json.insert("msgType","Ptz_Param");
    json.insert("ptzId",m_nPtzId);
    if(m_bPtzUsed){
        json.insert("serveLink","good");
    }
    else{
        json.insert("serverLink","bad");
    }

    if(m_TcpRole == CLIENT) {
        if(m_bClientConnected)
            json.insert("peer2peer","good");
        else
            json.insert("peer2peer","bad");
    } else {
        if(m_bServerConnected)
            json.insert("peer2Peer","good");
        else
            json.insert("peer2Peer","bad");
    }

    json.insert("localLng",m_ptzParam->LocalLng());
    json.insert("localLat",m_ptzParam->LocalLat());
    json.insert("localAlt",m_ptzParam->LocalAlt());
    json.insert("peerLng",m_ptzParam->PeerLng());
    json.insert("peerLat",m_ptzParam->PeerLat());
    json.insert("peerAlt",m_ptzParam->PeerAlt());
    json.insert("distance",m_ptzParam->Distance());
    json.insert("ptzhOffset",m_ptzParam->PtzHOffset());
    json.insert("ptzvOffset",m_ptzParam->PtzVOffset());
    json.insert("trackHeading",m_ptzParam->Heading());
    json.insert("trackPitch",m_ptzParam->Pitch());
    json.insert("ptzhAngle",m_ptzParam->PtzHAngle());
    json.insert("ptzvAngle",m_ptzParam->PtzVAngle());
    json.insert("calcuhAngle",m_ptzParam->ToPtzHAngle());
    json.insert("calcuvAngle",m_ptzParam->ToPtzVAngle());

    emit PtzParamReport(json);

}
void PtzProcess::WebConnectOrder(QJsonObject& json)
{
    PtzIniFile* cfg = getPtzConfig();
    if(json.value("fanId").toInt() != cfg->FanId().toInt()) return;

    Ptzmutex.lock();
    if(!json.value("lng").isNull()) {
        double lng = json.value("lng").toDouble();
        m_ptzParam->PeerLng(lng);
    }

    if(!json.value("lat").isNull()) {
        double lat = json.value("lat").toDouble();
        m_ptzParam->PeerLat(lat);
    }

    if(!json.value("alt").isNull()) {
        double alt = json.value("alt").toDouble();
        m_ptzParam->PeerAlt(alt);
    }

    Ptzmutex.unlock();
    emit dataUpdate(*m_ptzParam);
qDebug() << "WebConnectOrder = " << json;
    SendWebOrderRsp();
    QString IP = json.value("netIp").toString();
    QString Port = json.value("port").toString();
    QString runEnv = json.value("tcpRole").toString();
    if(IP.isEmpty() || Port.isEmpty() || runEnv.isEmpty()) {
        return;
    }
    if(IP == cfg->PeerIp()&&
            Port == cfg->Port()&&
            ((runEnv == "client"&&m_TcpRole == SERVER)||
            (runEnv == "server"&&m_TcpRole == CLIENT))) {
        return;
    } else {
        DisConnect();//disconnect current link first
        cfg->PeerIp(IP);
        cfg->Port(Port);
        if(runEnv == "client") {//m_TcpPeerRole = CLIENT;
            m_TcpRole = SERVER;
        } else { //if(runEnv == "server")
            m_TcpRole = CLIENT;
        }
        Connectnet();
    }
}

bool PtzProcess::sumCheck(QByteArray buffer)
{
    if(buffer.size() != 7) return false;
    uchar head = buffer.at(0);
    uchar hardid = buffer.at(1);
    uchar tail = buffer[buffer.size()-1];
    if(head != 0xff ||hardid != 0x01) return false;

    uchar sum = 0;
    for(int i =1;i<buffer.size()-1;i++) {
        sum += (uchar)buffer.at(i);
    }
    return sum == tail;
}
bool PtzProcess::ptzCtrlAnalyze(QJsonObject& json)
{
    QString strOrder = json.value("ptzCtrl").toString();
    if(!strOrder.isEmpty()) {
        if(m_CtrlType != MANUAL) return true;
        QByteArray order = strOrder.toLatin1();
        QByteArray lastOrder = QByteArray::fromHex(order);
        if( sumCheck(lastOrder)) {
            SendPtzCmd(lastOrder);
        }
        return true;
    }
    return false;
}

bool PtzProcess::runEnvAnalyze(QJsonObject& json)
{
    PtzIniFile* cfg = getPtzConfig();

    QString runEnv = json.value("runEnv").toString();
    if(!runEnv.isEmpty()) {
        if(runEnv.toLower() == "ship") {
            m_RunEnv = SHIP;
            cfg->RunEnv("Ship");
            cfg->WriteKeyValue("RunEnv","Ship");
        } else if (runEnv.toLower() == "bank") {
            m_RunEnv = BANK;
            cfg->RunEnv("Bank");
            cfg->WriteKeyValue("RunEnv","Bank");
        }
        return true;
    }
    return false;
}

bool PtzProcess::ctrlModeAnalyze(QJsonObject& json)
{
    PtzIniFile* cfg = getPtzConfig();
    QString ctrlMode = json.value("ctrlMode").toString();
    if(!ctrlMode.isEmpty()) {
        if(ctrlMode.toLower() == "auto") {
            m_CtrlType = AUTO;
            cfg->CtrlMode("Auto");
            cfg->WriteKeyValue("CtrlMode","Auto");
        } else if (ctrlMode.toLower() == "manual") {
            m_CtrlType = MANUAL;
            stopRun();
            cfg->CtrlMode("Manual");
            cfg->WriteKeyValue("CtrlMode","Manual");
        }
        return true;
    }
    return false;
}

bool PtzProcess::equipTypeAnalyze(QJsonObject& json)
{
    PtzIniFile* cfg = getPtzConfig();

    QString equipType = json.value("equip").toString();
    if(!equipType.isEmpty()) {
        if(equipType.toLower() == "big") {
            m_EquipType = OLD;
            cfg->EquipType("Big");
            cfg->WriteKeyValue("EquipType","Big");
        } else if (equipType.toLower() == "small") {
            m_EquipType = NEW;
            cfg->EquipType("Small");
            cfg->WriteKeyValue("EquipType","Small");
        }
        answerEquipType();
        return true;
    }
    return false;
}

void PtzProcess::tcpRoleAnalyze(QJsonObject &json)
{
    PtzIniFile* cfg = getPtzConfig();
    QString tcprole = json.value("tcpRole").toString();
    if(!tcprole.isEmpty()) {
        if(tcprole.toLower() == "client") {
            m_TcpRole = CLIENT;
            cfg->TcpRole("Client");
            cfg->WriteKeyValue("TcpRole","Client");
        } else if (tcprole.toLower() == "server") {
            m_TcpRole = SERVER;
            cfg->TcpRole("Server");
            cfg->WriteKeyValue("TcpRole","Server");
        }
    }
}

bool PtzProcess::dataSource(QJsonObject& json)
{
    QString ptzDataSource = json.value("dataSource").toString();
    if(ptzDataSource.isEmpty()) return false;
    QString ptzCom = json.value("ptzCom").toString();
    QString ptzBaud = json.value("ptzBaud").toString();
    QString ptzIp = json.value("ptzIp").toString();
    QString ptzPort = json.value("ptzPort").toString();

    PtzIniFile* cfg = getPtzConfig();

    cfg->DataSource(ptzDataSource);
    cfg->WriteKeyValue("DataSource",ptzDataSource);
    if(ptzDataSource =="com") {
        cfg->Com(ptzCom);
        cfg->BaudRate(ptzBaud);
        cfg->WriteKeyValue("Com",ptzCom);
        cfg->WriteKeyValue("BaudRate",ptzBaud);

        LoadPtz();
//        SetPtzComSlot();
    } else {
        cfg->ServerIp(ptzIp);
        cfg->ServerPort(ptzPort);
        cfg->WriteKeyValue("ServerIp",ptzIp);
        cfg->WriteKeyValue("ServerPort",ptzPort);
        LoadPtz();
//        ConnectPtzServer();
    }
    return true;
}

bool PtzProcess::netSetAnalyze(QJsonObject& json)
{
    PtzIniFile* cfg = getPtzConfig();

    QString netsetting = json.value("peerLink").toString();
    if(!netsetting.isEmpty()) {
        if(netsetting == "linkOn") {
            QString ptznetip = json.value("ptzNetIp").toString();
            QString ptzport = json.value("ptzPort").toString();

            if(m_TcpRole == CLIENT){
                cfg->PeerIp(ptznetip);
                cfg->WriteKeyValue("PeerIp",ptznetip);
            } else {
                cfg->LocalIp(ptznetip);
                cfg->WriteKeyValue("LocalIp",ptznetip);
            }
            cfg->WriteKeyValue("Port",ptzport);
            cfg->Port(ptzport);
            Connectnet();
        } else if (netsetting == "linkOff") {
            if(m_TcpRole == CLIENT) {
                m_tcpClient->disconnectFromHost();
            }
            if(m_TcpRole == SERVER) {
                m_tcpServer->clear();
            }
            qDebug() << "net disconnect";
        }
        return true;
    }

    QString str = json.value("fanId").toString();
    if(!str.isEmpty()) {
        cfg->FanId(str);
        cfg->WriteKeyValue("FanId",str);
    }
    return false;
}
void PtzProcess::aisMmsiNoAnalyze(QJsonObject& json)
{
    PtzIniFile* cfg = getPtzConfig();

    QString mmsiNo = json.value("aisMmsino").toString();
    if(!mmsiNo.isEmpty()) {
        m_mmsiNo = mmsiNo;
        cfg->AisMmsiNo(m_mmsiNo);
        cfg->WriteKeyValue("AisMmsiNo",mmsiNo);
    }
}

void PtzProcess::localPosAnalyze(QJsonObject& json)
{
    PtzIniFile* cfg = getPtzConfig();
    QString localLng = json.value("localLng").toString();
    if(!localLng.isEmpty()) {
        m_ptzParam->LocalLng(localLng.toDouble());
        cfg->WriteKeyValue("LocalLng",QString::number(m_ptzParam->LocalLng(),'f',7));
    }
    QString localLat = json.value("localLat").toString();
    if(!localLat.isEmpty()) {
        m_ptzParam->LocalLat(localLat.toDouble());
        cfg->WriteKeyValue("LocalLat",QString::number(m_ptzParam->LocalLat(),'f',7));
    }
    QString localAlt = json.value("localAlt").toString();
    if(!localAlt.isEmpty()) {
        m_ptzParam->LocalAlt(localAlt.toDouble());
        cfg->WriteKeyValue("LocalAlt",QString::number(m_ptzParam->LocalAlt(),'f',2));
        gps.alt = localAlt.toDouble();
    }
    emit dataUpdate(*m_ptzParam);
}

void PtzProcess::peerPosAnalyze(QJsonObject& json)
{
    PtzIniFile* cfg = getPtzConfig();
    QString peerLng  = json.value("peerLng").toString();
    if(!peerLng.isEmpty()) {
        m_ptzParam->PeerLng(peerLng.toDouble());
        cfg->WriteKeyValue("PeerLng",QString::number(m_ptzParam->PeerLng(),'f',7));
    }
    QString peerLat  = json.value("peerLat").toString();
    if(!peerLat.isEmpty()) {
        m_ptzParam->PeerLat(peerLat.toDouble());
        cfg->WriteKeyValue("PeerLat",QString::number(m_ptzParam->PeerLat(),'f',7));
    }
    QString peerAlt  = json.value("peerAlt").toString();
    if(!peerAlt.isEmpty()) {
        m_ptzParam->PeerAlt(peerAlt.toDouble());
        cfg->WriteKeyValue("PeerAlt",QString::number(m_ptzParam->PeerAlt(),'f',2));
    }
    emit dataUpdate(*m_ptzParam);
}

void PtzProcess::ptzOffsetAnalyze(QJsonObject& json)
{
    PtzIniFile* cfg = getPtzConfig();
    QString ptzHOffset = json.value("ptzhOffset").toString();
    if(!ptzHOffset.isEmpty()) {
        m_ptzParam->PtzHOffset(ptzHOffset.toDouble());
        cfg->WriteKeyValue("PtzHOffSet",ptzHOffset);

    }
    QString ptzVOffset = json.value("ptzvOffset").toString();
    if(!ptzVOffset.isEmpty()) {
        m_ptzParam->PtzVOffset(ptzVOffset.toDouble());
        cfg->WriteKeyValue("PtzVOffSet",ptzVOffset);
    }
    emit dataUpdate(*m_ptzParam);
}

void PtzProcess::track(QJsonObject& json)
{
    QString heading = json.value("trackHead").toString();
    if(!heading.isEmpty()) {
        m_ptzParam->Heading(heading.toDouble());
    }
    QString pitch = json.value("trackPitch").toString();
    if(!pitch.isEmpty()) {
        m_ptzParam->Pitch(pitch.toDouble());
    }
    emit dataUpdate(*m_ptzParam);
}

void PtzProcess::vertical_amend(QJsonObject& json)
{
    QString str = json.value("verticalAmend").toString();
    if(!str.isEmpty()) {
        PtzIniFile* cfg = getPtzConfig();
        cfg->vAmend(str);
        cfg->WriteKeyValue("VerticalAmend",str);
        if(str == "yes") {
            m_bVAmend = true;
        } else {
            m_bVAmend = false;
        }
    }
}

void PtzProcess::WebPtzCtrl(QJsonObject& json)
{
    if(json.value("ptzId").toInt() != m_nPtzId) return;
    if(ptzCtrlAnalyze(json))    return;
    if(runEnvAnalyze(json))     return;
    if(ctrlModeAnalyze(json))   return;
    if(equipTypeAnalyze(json))  return;
    if(dataSource(json))        return;
    tcpRoleAnalyze(json);
    if(netSetAnalyze(json))     return;
    aisMmsiNoAnalyze(json);
    localPosAnalyze(json);
    peerPosAnalyze(json);
    ptzOffsetAnalyze(json);
    ptz_check_screen_config(json);
    track(json);
    vertical_amend(json);
    SendPtzEnvrt();
    SendPtzParam();
}

void PtzProcess::ptz_check_reconfig()
{
    m_timer_check_socket_state->stop();
    PtzIniFile* cfg = getPtzConfig();
    m_ptz_check_socket_ip = cfg->ptz_check_ip();
    m_ptz_check_socket_port = cfg->ptz_check_port();
    m_ptz_check_socket->abort();
    m_ptz_check_socket->ConnectServer(m_ptz_check_socket_ip, m_ptz_check_socket_port.toUShort());
    m_timer_check_socket_state->start(2000);
}

void PtzProcess::ptz_no_response()
{
    ptz_over_time();
    m_ptz_link_state = false;
}

void PtzProcess::ptz_over_time()
{
    if(m_ptz_restart_times++ <= 10) {
        if(m_ptz_check_socket->state() == QAbstractSocket::ConnectedState) {
            m_ptz_check_socket->SendData("AT+STACH1=1,5\n");
            QStringList list;
            list<<"send               AT+STACH1=1,5\n";
            LogRecord(25,OPERATOR,list,QDateTime::currentDateTime());
        }
        qDebug() << "send               AT+STACH1=1,5\n";
        if(m_ptz_restart_times >= 60) {
            m_ptz_restart_times = 11;
        }
    }
}

void PtzProcess::ptz_check_socket_state()
{
    if(m_ptz_check_socket->state() != QAbstractSocket::ConnectedState) {
        m_ptz_check_socket->abort();
        m_ptz_check_socket->ConnectServer(m_ptz_check_socket_ip, m_ptz_check_socket_port.toUShort());
        m_ptz_check_link_state = false;
    } else {
        m_ptz_check_link_state = true;
    }
}

void PtzProcess::gps_latlon_check()
{
    m_gps_latlon_overtime = false;
}
void PtzProcess::gps_angle_check()
{
    m_gps_angle_overtime = false;
}

// screen
void PtzProcess::get_auto_search_switch(bool state)
{
    emit send_auto_search_switch(state);
}

void PtzProcess::get_auto_search_reconfig()
{
    emit send_auto_search_reconfig();
}

void PtzProcess::get_data_deal_reconfig()
{
    emit send_data_deal_reconfig();
}
void PtzProcess::get_wireless_reconfig()
{
    emit send_wireless_reconfig();
}
// id
void PtzProcess::get_id_state(bool state)
{
    m_id_is_true = state;
}

void PtzProcess::get_id_check_switch(bool state)
{
    m_id_check_switch = state;
}

void PtzProcess::ptz_check_screen_config(QJsonObject &json)
{
    PtzIniFile* cfg = getPtzConfig();
    QString check_ip = json.value("ptzCheckIp").toString();
    QString check_port = json.value("ptzCheckPort").toString();
    if(!check_ip.isEmpty()) {
        cfg->ptz_check_ip(check_ip);
        cfg->WriteKeyValue("PtzCheckIp",check_ip);
    }

    if(!check_port.isEmpty()) {
        cfg->ptz_check_port(check_port);
        cfg->WriteKeyValue("PtzCheckPort",check_port);
    }
    ptz_check_reconfig();
}

