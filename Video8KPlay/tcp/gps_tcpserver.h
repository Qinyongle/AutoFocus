#ifndef Gps_TcpServer_H
#define Gps_TcpServer_H
#include <QTcpServer>
#include <QTcpSocket>
#include <QMutex>
#include <QTimer>
#include <QFile>
#include <QSettings>
#include <QJsonParseError>
#include <QJsonObject>

class Gps_TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit Gps_TcpServer();
    ~Gps_TcpServer();

    bool saveFile(QString fileName);

private:
    QTcpServer *tcpServer; //服务器类
    QTcpSocket *tcpClient; //客户端类
    int MAXConnect; //TCP最大连接数

    QTimer *socket_timer;

    QTimer *Tcp_state_timer;

    QString Chick_str;

    QByteArray TCP_Msg;

    QString _lng2;
    QString _lat2;
    QString _offsetangle;

    bool connect_flag;

signals:
    void send_DATA();

    void send_CTL(float);
    void SendGpsData(double,double,float,float);

public slots:
    void TcpServerListen(QString, int);
    void newConnectionSlot();
    void readyReadSlot();
    void SendData(QString data);
    void DisConnected();
    void delete_socket(QAbstractSocket::SocketError);
    void socket_State_check();
    void Tcp_State_check();
};

#endif // Gps_TcpServer_H

