#ifndef PTZTHREADS_H
#define PTZTHREADS_H

#include <QObject>
#include <QThread>
#include "QDebug"
#include "ffmpeg/ffmpegdecoder.h"
#include "tcp/tcpsocket.h"
#include "ptz/ptz_reciever.h"
#include "ptz/ptz_analyzer.h"
#include "ptz/zoomreceiver.h"
#include "ptz/ptz_opencv.h"
#include "ptz/ptz_controller.h"
#include <QTimer>


class PtzThreads : public QObject
{
    Q_OBJECT
public:
    explicit PtzThreads(QObject *parent = nullptr);
    ~PtzThreads();

signals:
    // tcp
    void HVConnect(QString, int);
    void HVDisconnect();
    void HVSendData(QByteArray);
//    void ZoomConnect(QString, int);
//    void ZoomDisconnect();
//    void ZoomSendData(QByteArray);

    // other
    void HVConnectState(bool);
    void ZoomeConnectState(bool);
    void HAngle(double);
    void VAngle(double);
    void ZoomAngle(double);

    //controll
     void Timestart(int);
     void Timestop();
     void SendAngle();
     void CheckDoneFromControll();
     void StepMaxDoneFromControll();
public slots:
    // tcp
    void HVConnected(QString, int);
    void HVDisconnected(QString, int);
    void ZoomeConnected(QString, int);
    void ZoomeDisconnected(QString, int);



    // other
    void Run();
    void Quit();
    void RRotate();
    void LRotate();
    void HAngleSet(double);
    void URotate();
    void DRotate();
    void VAngleSet(double);
    void RotateStop();
    void ZIn();
    void ZOut();
    void ZSet(double);
    void ZStop();

    void fout(int);
private:
    void InitThread();



private:
    PtzAnalyzer *m_hvAnalyzer = nullptr;
    QThread *m_hvAnalyzerThread = nullptr;

    PtzReciever *m_hvReciever = nullptr;
    QThread *m_hvRecieverThread = nullptr;

//    PtzOpencv *m_hvOpencv=nullptr;
//    QThread *m_hvOpencvThread = nullptr;

    TcpSocket *m_hvSocket = nullptr;
    QThread *m_hvSocketThread = nullptr;

    ZoomReceiver *m_zoomReceiver = nullptr;
    QThread *m_zoomReceiverThread = nullptr;

    ptz_controller *m_hvController = nullptr;
    QThread *m_ControllerThread = nullptr;

    QTimer *m_controllerTimer = nullptr;
    QThread *TimerThread = nullptr;
//    TcpSocket *m_zoomSocket = nullptr;
//    QThread *m_zoomSocketThread = nullptr;

};

#endif // PTZTHREADS_H
