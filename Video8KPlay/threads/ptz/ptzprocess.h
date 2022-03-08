#ifndef PTZPROCESS_H
#define PTZPROCESS_H

#include <QObject>
#include <QString>
#include <QTimer>
#include <QPair>
#include <QMap>
#include <QHostAddress>
#include <QStringListModel>
#include <QThread>
#include "log/log.h"
#include "ini/inifile.h"
#include "ptzinifile.h"
#include "tcp/tcpsocket.h"
#include "tcp/tcpserver.h"
#include "serial/serialport.h"
#include "gps/datastruct.h"
#include "ptz_protocol.h"
#include "data/jsonreciever.h"
#include "ptz_reciever.h"
#include "ptz_analyzer.h"
#include "data/calculateangle.h"
#include "auto_search/auto_search.h"
#include "auto_search/wireless.h"
#include "auto_search/data_deal.h"
class PtzProcess : public QObject
{
    Q_OBJECT
public:
    explicit  PtzProcess(QObject *parent = nullptr, int ID=1);
        ~PtzProcess();
    enum socketType{SERVER,CLIENT};
    enum runEnvType{SHIP,BANK};
    enum ptzCtlType{AUTO,MANUAL};
    enum equipType {OLD,NEW};
    enum searchType {ROUGH,DELIBERATE};
	enum peerMsgType{PEER_MSG,PEER_INFORM,PPER_INQUIRE,PEER_RESPOND};  

signals:
    void PtzDataComing(QByteArray);
    void GPsMessage(GpsData);
    void sendEquipType(int);
    void ReportState(QJsonObject&);
    void WebRspMessage(QJsonObject&);
    void AutoSendHCmd();
    void AutoSendVCmd();
    void LogRecord(int,LogMsgType,QStringList,QDateTime);
    void dataUpdate(ptzParam);
    void ResetPtzClient(QString,int);
    void SendPtzctrlbyNet(QByteArray);
    void clientOrignal(QByteArray);
    void clientOrignal(int,QByteArray);
    void serverOrignal(QByteArray);
    void serverOrignal(int,QByteArray);//to screen to identify
    void PtzEnvrtReport(QJsonObject&);
    void PtzParamReport(QJsonObject&);

    // tcp
    void ptz_tcp_cloase();
    //serial port
    void ptzSerialClose();
    void ptzSerialSend(QByteArray);
    void ptzSetBaudRate(int);
    void ptzSerialOpen(QString portName,
                    int baudRate,
                    QSerialPort::DataBits dataBits,
                    QSerialPort::Parity parity,
                    QSerialPort::StopBits stopBits);
    // screen
    void send_auto_search_switch(bool);
    // reconfig
    void send_auto_search_reconfig();
    void send_data_deal_reconfig();
    void send_wireless_reconfig();

public slots:
    //data drive
    void SendPtzHCmd();
    void SendPtzVCmd();
    //timer
    void SendNetMsg();  
    void Connectnet();
    void DisConnect();
    void StateCheck();
    void Report();
    //ais
    void getAisPos(QString,double lng,double lat);//ais
    void EnableAisPos();
    //web
    void WebConnectOrder(QJsonObject&);
	void SendWebOrderRsp();
    void WebPtzCtrl(QJsonObject&);
    void SendPtzEnvrt();
    void SendPtzEnvrtSyn(int);
    //gps
    void GpsPotUpDate(GpsData msg);
    void resetGpsSignal();

    void getPtzHAngle(double);
    void getPtzVAngle(double);
    void answerEquipType();
    //peer msg
    void DoAnalyzeJson(QByteArray data);
    void PeerInformMsgAnalyze(QJsonObject& json);

    //com
    void SetPtzComSlot();
    void ControlOpened(bool);
    void ControlClosed(bool);
    //server
    void ClientReadData(int clientID,QByteArray data);
    void ClientConnect(int clientID,QString IP,int Port);
    void ClientDisConnect(int clientID,QString IP,int Port);
    //client
    void ReadData();
    void clientConnected();
    void clientDisConnected();
    //ptzSocket
    void ptzConnected(QString,int);
    void ptzDisConnected(QString,int);
    //calc
    void ToHAngle(double);
    void ToVAngle(double);
    void Distance(double);

    // check
    void ptz_no_response();
    void ptz_check_socket_state();
    void ptz_check_screen_config(QJsonObject &json);
    void ptz_check_reconfig();
    void gps_latlon_check();
    void gps_angle_check();
    // screen
    void get_auto_search_switch(bool);
        // reconfig
    void get_auto_search_reconfig();
    void get_data_deal_reconfig();
    void get_wireless_reconfig();
    void get_ptz_hoffset_reconfig(double);
    // id
    void get_id_state(bool);
    void get_id_check_switch(bool);

private:
    int getId(){ return m_nPtzId;}
    PtzIniFile* getPtzConfig();
    void InitVarables();
    void Init();
    void ReadPtzParam();
    void LoadConfig();
    void SaveConfig();
	void LoadPtz();
    void stopRun();
    void SetPtzSendBack(bool);

    void SendPtzParam();
    bool sumCheck(QByteArray buffer);
    bool ptzCtrlAnalyze(QJsonObject& json);
    bool runEnvAnalyze(QJsonObject& json);
    bool ctrlModeAnalyze(QJsonObject& json);
    bool equipTypeAnalyze(QJsonObject& json);
    bool dataSource(QJsonObject& json);
    bool netSetAnalyze(QJsonObject& json);
    void tcpRoleAnalyze(QJsonObject& json);
    void aisMmsiNoAnalyze(QJsonObject& json);
    void localPosAnalyze(QJsonObject& json);
    void peerPosAnalyze(QJsonObject& json);
    void ptzOffsetAnalyze(QJsonObject& json);
    void track(QJsonObject& json);
    void vertical_amend(QJsonObject& json);
    //timer
    void ConnectPtzServer();
    void ConnectPeerServer();
    void LocalServerListen();

    void SendPtzCmd(QByteArray);

    // check
    void ptz_over_time();

private:
    int m_nPtzId;//PTZ's id
    double m_dPtzPrehAngle;
    ptzParam* m_ptzParam;
    runEnvType m_RunEnv;
    runEnvType m_PeerRunEnv;
    ptzCtlType m_CtrlType;
    ptzCtlType m_CtrlTypeSave;
    equipType m_EquipType;
    socketType m_TcpRole;
    socketType m_TcpPeerRole;

    SerialPort* m_ptzSerialPort;
    QThread SerialPortThread;
    bool m_bPtzSerialOpened;

    JsonReciever* m_serverReciever;
    QThread serverRecieveThread;
    TcpServer* m_tcpServer;
    JsonReciever* m_clientReciever;
    QThread clientRecieveThread;
    QTcpSocket* m_tcpClient;
    //ptz data
    PtzAnalyzer* m_PtzAnalyzer;
    QThread m_PtzAnalyzeThread;
    PtzReciever* m_PtzReciever;
    QThread m_PtzRecieveThread;
    TcpSocket* m_PtzSocket;
    QThread m_PtzClientThread;

    CalculateAngle* m_Calcute;
    QThread CalcuThread;

    QTimer*   m_timerSendNetMsg;
    QTimer*   m_timerReport;
    QTimer*   m_timerScreen;
    QTimer*   m_timerStateCheck;
    QTimer*   m_timerAisPos;

    // wireless
    wireless *m_wireless;
    QThread wireless_thread;

    // auto search
    auto_search *m_auto_search;

    // data deal
    data_deal *m_data_deal;
    QThread data_deal_thread;

private:
    bool m_bVAmend;
    uchar m_Hspeed;
    uchar m_Vspeed;
    bool m_bInitGps;
    bool m_bInitGpsAngle;

    bool m_bPtzUsed;
    double m_dPtzScale;
    int m_nPtzLostCount;
    static const int PTZ_LOST_LIMIT = 5;

    bool m_bUseAisMsg;
    int m_nAisDelaySecd;//s

    bool m_bClientConnected;
    bool m_bServerConnected;
    int m_nClientNetLostCount;
    int m_nServerNetLostCount;
    static const int NET_LOST_LIMIT = 3;

    QString m_mmsiNo;
    // ptz
    int m_setptz_angle;
    QTimer*   m_timer_check_angle;
    QTimer*   m_timer_check_socket_state;
    TcpSocket* m_ptz_check_socket;
    QThread ptz_check_socket_thread;

    QString m_ptz_check_socket_ip;
    QString m_ptz_check_socket_port;
    bool m_ptz_check_link_state;
    int m_ptz_restart_times;
    int m_gps_hoffset;
    bool m_ptz_link_state;

    // gps check timer
    QTimer*   m_gps_lonlat_check_timer;
    QTimer*   m_gps_angle_check_timer;
    bool m_gps_latlon_overtime;
    bool m_gps_angle_overtime;

    // id
    bool m_id_is_true;
    bool m_id_check_switch;
};

#endif // PTZPROCESS_H
