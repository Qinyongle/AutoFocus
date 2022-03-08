#ifndef GPSGETTHREAD_H
#define GPSGETTHREAD_H

#include <QObject>
#include <QThread>
#include "tcp/tcpsocket.h"
#include "tcp/tcpserver.h"
#include "gps/gps_reciever.h"
#include "gps/gps_analyzer.h"

#include "tcp/gps_tcpserver.h"


class GpsGetThread : public QObject
{
    Q_OBJECT
public:
    explicit GpsGetThread(QObject *parent = nullptr);
    ~GpsGetThread();

signals:
    void ConnectCmd(QString, int);
    void DisConnectCmd();
    void GpsPotData(GpsData);

    //server
    void TcpListen(QString,int);
    void SendGpsData(double,double,float,float);

public slots:
    // tcp
    void GpsConnected(QString,int);
    void GpsDisConnected(QString,int);

    // other
    void Connect(QString, int);

    //server_tcp
    void ClientsConnected(int clientID,QString ip,int port);
    void ClientsDisConnected(int clientID,QString ip,int port);

    //灿军 tcp_server
    void RecvGpsData(double lng,double lat,float nav,float hight);







private:
    void InitThread();


private:
//    TcpSocket *m_tcp = nullptr;
//    QThread *m_tcpThread = nullptr;
    struct CLIENT_MSG{
        QString ip;
        int port;
    };

//    GpsReciever *m_reciever = nullptr;
//    QThread *m_recieverThread = nullptr;

//    GpsAnalyzer *m_gpsAnaluzer = nullptr;
//    QThread *m_gpsAnaluzerThread = nullptr;

//    //服务器
//    TcpServer * m_tcp = nullptr;
//    QThread * m_serverThread = nullptr;

//    QHash<qintptr,CLIENT_MSG> m_clients;

    Gps_TcpServer * m_tcp = nullptr;
    QThread * m_serverThread = nullptr;


};

#endif // GPSGETTHREAD_H
