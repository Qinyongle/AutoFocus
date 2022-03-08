#include "gps_tcpserver.h"


Gps_TcpServer::Gps_TcpServer()
{
      qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");
}


Gps_TcpServer::~Gps_TcpServer()
{

}

//服务器IP和端口的绑定
void Gps_TcpServer::TcpServerListen(QString ip, int port)
{
    static QMutex Mutex;
    QMutexLocker locker(&Mutex); //在函数调用时加锁，函数调用结束后，释放...

    connect_flag = false;

    qDebug()<<"*****Gps_TcpServer bind*****";
    qDebug()<<"Gps_TcpServer IP: "<<ip;
    qDebug()<<"Gps_TcpServer port: "<<port;

    MAXConnect = 1000;
    tcpServer = new QTcpServer();  //初始化服务器指针

    /**********绑定端口和IP地址，开始监听端口**********/
   if(tcpServer->listen(QHostAddress(ip),port))  //绑定IP和端口
   {
       qDebug()<<"Gps_TcpServer bind sccess!"<<"\r\n";
   }
   else
   {
       qDebug()<<"Gps_TcpServer bind error!"<<"\r\n";
   }

   tcpServer->setMaxPendingConnections(MAXConnect);  //设置最大的绑定数目为1000
   connect(tcpServer,SIGNAL(newConnection()),this,SLOT(newConnectionSlot()));
}

//客户端连接上的信号
void Gps_TcpServer::newConnectionSlot()
{
    static QMutex Mutex;
    QMutexLocker locker(&Mutex); //在函数调用时加锁，函数调用结束后，释放...

    qDebug()<<"# new client connect."<<"\r\n";

//    socket_timer = new QTimer();
//    connect(socket_timer, SIGNAL(timeout()), this, SLOT(socket_State_check()));
//    socket_timer->start(5000);

//    Tcp_state_timer = new QTimer();
//    connect(Tcp_state_timer, SIGNAL(timeout()), this, SLOT(Tcp_State_check()));
//    Tcp_state_timer->start(20000);

    tcpClient=tcpServer->nextPendingConnection(); //连接对象
    connect(tcpClient, SIGNAL(readyRead()), this, SLOT(readyReadSlot()), Qt::DirectConnection);//关联读信息的槽函数
    connect(tcpClient, SIGNAL(disconnected()), this, SLOT(DisConnected()), Qt::DirectConnection);
    connect(tcpClient, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(delete_socket(QAbstractSocket::SocketError)));//建立槽函数，获取错误信息
}

void Gps_TcpServer::DisConnected()
{
    qDebug()<< " #" <<this->tcpClient<<" client socket exit...";
    //释放socket指针， 防止内存泄漏
//    this->tcpClient->deleteLater();
//    socket_timer->stop();
//    Tcp_state_timer->stop();

    connect_flag = false;
}

//删除APP PC掉线的客户端
void Gps_TcpServer::delete_socket(QAbstractSocket::SocketError)
{
    QTcpSocket *mkt = (QTcpSocket*)sender();
    qDebug()<<"mkt_error:"<<mkt->error();
    switch(mkt->error())
    {
    case QAbstractSocket::RemoteHostClosedError://客户端断开
//        socket_timer->stop();   //客户端断开连接， 停止心跳定时器
//        Tcp_state_timer->stop();
        qDebug()<<" # client: "<< mkt->peerAddress().toString()<< " " << QString::number(mkt->peerPort(),10) << " disconnected."<<"\r\n";    //显示客户端断开信息
        connect_flag = false;
        break;
    default:
        break;
    }
}

//全局心跳监听socket状态，断开则响应信号释放线程
void Gps_TcpServer::socket_State_check()
{
    if(tcpClient->isOpen())
    {
        tcpClient->write("Server");
        qDebug()<<"*****socket_State_check*****"<<"\r\n";
    }
}

void Gps_TcpServer::Tcp_State_check()
{

    if(Chick_str.indexOf("Client") != -1)
    {
        qDebug()<<"### connect state Ok..."<<"\r\n";
        Chick_str.clear();
        connect_flag = true;
    }
    else
    {
        qDebug()<<"### connect state No..."<<"\r\n";
        Chick_str.clear();
        connect_flag = false;
        Tcp_state_timer->stop();
        socket_timer->stop();
    }
}

//服务端接收消息信号
void Gps_TcpServer::readyReadSlot()
{

    TCP_Msg = TCP_Msg.append(tcpClient->readAll());

    qDebug()<<"###### TCP ->Msg: "<<TCP_Msg;
    QString str = TCP_Msg;
    TCP_Msg.clear();
    QStringList list = str.split('\n');
    for(int i=0; i<list.size(); i++)
    {
        qDebug()<<"list ["<<i<<"]"<<": "<<list.at(i);
        if(list.at(i).indexOf("gpsdata") != -1)
        {
            QJsonParseError json_error;
            QJsonDocument jsonDoc(QJsonDocument::fromJson(list.at(i).toLatin1(), &json_error));
            if(json_error.error != QJsonParseError::NoError)
            {
                qDebug()<<"json error!";
                TCP_Msg.clear();
                return;
            }
            QJsonObject rootObj = jsonDoc.object();

            double lng = rootObj.value("lng").toDouble();
            double lat = rootObj.value("lat").toDouble();
            float Nav = rootObj.value("Nav").toDouble();
            float hight = rootObj.value("h").toDouble();
            qDebug()<<"Gpsdata:"<<lng<<"|"<<lat<<"|"<<Nav<<"|"<<hight;
            emit SendGpsData(lng,lat,Nav,hight);
//            qDebug()<<" ### lng: "<<lng;
//            qDebug()<<"### lat: "<<lat;
//            qDebug()<<" ### Nav: "<<Nav;
//            qDebug()<<"### hight: "<<hight<<"\r\n";

            break ;
        }
    }


}

void Gps_TcpServer::SendData(QString data)
{
    static QMutex Mutex;
    QMutexLocker locker(&Mutex); //在函数调用时加锁，函数调用结束后，释放...

    if(tcpClient)
    {
        tcpClient->write(data.toLatin1());
    }
}







