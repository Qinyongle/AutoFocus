#ifndef TcpSocket_H
#define TcpSocket_H

#include <QTcpSocket>

class TcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpSocket(QObject *parent = nullptr,int Id =0);
    int getId() { return m_nId; }
    bool operator ==(TcpSocket& );

signals:
    void DisConnected(int Id,QString IP,int Port);
    void Connected(int Id,QString IP,int Port);
	void RecieveData(int Id,QByteArray);
    void DisConnected(QString IP,int Port);
    void Connected(QString IP,int Port);
    void RecieveData(QByteArray);
	
public slots:
    void ReadData();
    void SendData(QByteArray);
    void DisConnected();
    void Connected();
    void disConTcp(int i);//respond slot from server disconect signal
	void SetServer(QString ip,int port);
	void ConnectServer(QString ip,int port);
    void disconnect();

private:
    int m_nId;
};
#endif // TcpSocket_H
