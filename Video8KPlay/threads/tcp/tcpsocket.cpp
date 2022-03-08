#include "tcpsocket.h"
#include <QHostAddress>
#include <QNetworkProxy>
#ifdef _WINDOWS
#include <windows.h>
#elif _UNIX
#include <unistd.h>
#endif

TcpSocket::TcpSocket(QObject *parent,int Id) :
    QTcpSocket(parent)
{
    connect(this,SIGNAL(readyRead()),this,SLOT(ReadData()));
    connect(this,SIGNAL(disconnected()),this,SLOT(DisConnected()));
    connect(this,SIGNAL(connected()),this,SLOT(Connected()));
    m_nId = Id;
}
bool TcpSocket::operator ==(TcpSocket& s)
{
    if(m_nId == s.m_nId)
        return true;
    else
        return false;
}

void TcpSocket::ReadData()
{
    QByteArray data = this->readAll();
    if(m_nId != 0)
        emit RecieveData(m_nId,data);
    else
        emit RecieveData(data);
}
void TcpSocket::SendData(QByteArray data)
{
    if(this->state()==QAbstractSocket::ConnectedState)
        this->write(data);
}

void TcpSocket::DisConnected()
{   
    if(m_nId != 0)
        emit DisConnected(m_nId,this->peerAddress().toString(),this->peerPort());
    else
        emit DisConnected(this->peerAddress().toString(),this->peerPort());
}
void TcpSocket::Connected()
{
    if(m_nId != 0)
        emit Connected(m_nId,this->peerAddress().toString(),this->peerPort());
    else
        emit Connected(this->peerAddress().toString(),this->peerPort());
}

void TcpSocket::disConTcp(int i)
{
    if ((i == m_nId) || (i == -1&&m_nId>0))
    {
        this->disconnectFromHost();
        this->deleteLater();
    }
}

void TcpSocket::disconnect()
{
    this->disconnectFromHost();
    this->abort();
}

void TcpSocket::ConnectServer(QString ip,int port)
{
    this->setProxy(QNetworkProxy::NoProxy);
    this->connectToHost(ip,QString::number(port).toUShort());
#ifdef _WINDOWS
    Sleep(1000);
#elif _UNIX
    sleep(1);
#endif
}

void TcpSocket::SetServer(QString ip,int port)
{
    if((this->state()==QAbstractSocket::ConnectedState)
        ||(this->state()==QAbstractSocket::ConnectingState))
    {
        qDebug() << "this->state() ==" << this->state();
        return;
    }
    else
    {
        this->abort();
        ConnectServer(ip,port);
    }
}
