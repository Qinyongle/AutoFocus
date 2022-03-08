#include "gpsgetthread.h"
#include <QDebug>
GpsGetThread::GpsGetThread(QObject *parent) : QObject(parent)
{
    InitThread();
}


GpsGetThread::~GpsGetThread()
{
    emit DisConnectCmd();
//    m_tcpThread ? m_tcpThread->quit(), m_tcpThread->wait() : bool();

    m_serverThread ? m_serverThread->quit(), m_serverThread->wait() : bool();
//    m_recieverThread ? m_recieverThread->quit(), m_recieverThread->wait() : bool();
//    m_gpsAnaluzerThread ? m_gpsAnaluzerThread->quit(), m_gpsAnaluzerThread->wait() : bool();
}


void GpsGetThread::InitThread()
{
//    m_gpsAnaluzer = new GpsAnalyzer;
//    m_gpsAnaluzerThread = new QThread;
//    m_gpsAnaluzer->moveToThread(m_gpsAnaluzerThread);
//    connect(m_gpsAnaluzer,&GpsAnalyzer::GpsPotData,
//            this,[=](GpsData data) {
//        emit GpsPotData(data);});
//    m_gpsAnaluzerThread->start();


//    m_reciever = new GpsReciever;
//    m_recieverThread = new QThread;
//    m_reciever->moveToThread(m_recieverThread);
//    connect(m_recieverThread, &QThread::finished, m_reciever, &QObject::deleteLater);
//    connect(m_reciever,SIGNAL(dataReady()),m_gpsAnaluzer,SLOT(DoAnalyze()));
//    m_recieverThread->start();

      //client
//    m_tcp = new TcpSocket;
//    m_tcpThread = new QThread;
//    m_tcp->moveToThread(m_tcpThread);
//    connect(m_tcpThread, &QThread::finished, m_tcp, &QObject::deleteLater);
//    connect(this, SIGNAL(ConnectCmd(QString,int)), m_tcp, SLOT(SetServer(QString,int)));
//    connect(this, SIGNAL(DisConnectCmd()), m_tcp, SLOT(disconnect()), Qt::DirectConnection);
//    connect(m_tcp,SIGNAL(Connected(QString,int)),this,SLOT(GpsConnected(QString,int)));
//    connect(m_tcp,SIGNAL(DisConnected(QString,int)),this,SLOT(GpsDisConnected(QString,int)));
//    connect(m_tcp,SIGNAL(RecieveData(QByteArray)),m_reciever,SLOT(DoRecive(QByteArray)));
//    m_tcpThread->start();


    //server
//    m_tcp = new TcpServer;
//    m_serverThread = new QThread;
//    m_tcp->moveToThread(m_serverThread);
//    connect(m_serverThread, &QThread::finished, m_tcp, &QObject::deleteLater);
//    connect(this, SIGNAL(TcpListen(QString,int)), m_tcp, SLOT(SetTcpListen(QString,int)));
//    connect(m_tcp,SIGNAL(ClientConnected(int,QString,int)),this,SLOT(ClientsConnected(int,QString,int)));
//    connect(m_tcp,SIGNAL(ClientDisConnected(int,QString,int)),this,SLOT(ClientsDisConnected(int,QString,int)));
//    connect(m_tcp,SIGNAL(SendReadData(QByteArray)),m_reciever,SLOT(DoRecive(QByteArray)));
//    m_serverThread->start();

    //gps_server
    m_tcp = new Gps_TcpServer;
    m_serverThread = new QThread;
    m_tcp->moveToThread(m_serverThread);
    connect(m_serverThread, &QThread::finished, m_tcp, &QObject::deleteLater);
    connect(m_tcp, SIGNAL(SendGpsData(double,double,float,float)),
            this,SLOT(RecvGpsData(double,double,float,float)));


    //SLOT

    m_serverThread->start();




}



void GpsGetThread::GpsConnected(QString ip, int port)
{
    qDebug() << "tcp connected " << ip << ":" << port;
}

void GpsGetThread::GpsDisConnected(QString ip, int port)
{
    qDebug() << "tcp Disconnected " << ip << ":" << port;
}

void GpsGetThread::Connect(QString ip, int port)
{
////    emit ConnectCmd(ip, port);
//    //server
//    emit TcpListen(ip,port);

        m_tcp->TcpServerListen(ip,port);
}

void GpsGetThread::ClientsConnected(int clientID, QString ip, int port)
{
//    CLIENT_MSG client;
//    client.ip = ip;
//    client.port = port;
//    m_clients.insert(clientID,client);
}

void GpsGetThread::ClientsDisConnected(int clientID, QString ip, int port)
{
//    QHash<qintptr,CLIENT_MSG>::iterator it;
//    for(it = m_clients.begin();it != m_clients.end();it++){
//        if(it.key() == clientID){
//            m_clients.remove(clientID);
//            break;
//        }
    //    }
}

void GpsGetThread::RecvGpsData(double lng, double lat, float nav, float hight)
{
    emit SendGpsData(lng,lat,nav,hight);
}
