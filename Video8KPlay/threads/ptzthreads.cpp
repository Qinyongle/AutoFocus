#include "ptzthreads.h"
#include <QSettings>
#include "ptz/ptz_protocol.h"

PtzThreads::PtzThreads(QObject *parent) : QObject(parent)
{
    InitThread();
}


PtzThreads::~PtzThreads()
{
    // hv
    m_hvSocketThread->quit();
    m_hvSocketThread->wait();
    m_hvRecieverThread->quit();
    m_hvRecieverThread->wait();
    m_hvAnalyzerThread->quit();
    m_hvAnalyzerThread->wait();

//    // zoom
//    m_zoomSocketThread->quit();
//    m_zoomSocketThread->wait();
}




void PtzThreads::InitThread()
{
    m_hvAnalyzer = new PtzAnalyzer;
    m_hvAnalyzerThread = new QThread;
    m_hvAnalyzer->moveToThread(m_hvAnalyzerThread);
    connect(m_hvAnalyzerThread, &QThread::finished, m_hvAnalyzer, &QObject::deleteLater);
    connect(m_hvAnalyzer, &PtzAnalyzer::ptzHAngle,
            this, [=](double angle) {
        emit HAngle(angle);});
    connect(m_hvAnalyzer, &PtzAnalyzer::ptzVAngle,
            this, [=](double angle) {
        emit VAngle(angle);});
    m_hvAnalyzerThread->start();

    m_hvReciever = new PtzReciever;
    m_hvRecieverThread = new QThread;
    m_hvReciever->moveToThread(m_hvRecieverThread);
    connect(m_hvRecieverThread, &QThread::finished, m_hvReciever, &QObject::deleteLater);
    connect(m_hvReciever, SIGNAL(dataReady()),
            m_hvAnalyzer, SLOT(DoAnalyze()));
    m_hvRecieverThread->start();

    m_zoomReceiver = new ZoomReceiver;
    m_zoomReceiverThread = new QThread;
    m_zoomReceiver->moveToThread(m_zoomReceiverThread);
    connect(m_zoomReceiverThread, &QThread::finished,
            m_zoomReceiver, &QObject::deleteLater);
    connect(m_zoomReceiver, &ZoomReceiver::SendZoomNumber, this, [=] (double num) {
        emit ZoomAngle(num);
    });
    m_zoomReceiverThread->start();

//    m_hvOpencv = new PtzOpencv;
//    m_hvOpencvThread = new QThread;
//    m_hvOpencv->moveToThread(m_hvOpencvThread);
//    connect(m_hvOpencvThread,&QThread::finished,m_hvOpencv,&QObject::deleteLater);
//    connect(m_hvOpencv,&PtzOpencv::imageReady,m_hvOpencv,&PtzOpencv::OpenCvLaplacian);     //当接收到照片准备好，则执行分析图片
//    m_hvOpencvThread->start();


    m_hvSocket = new TcpSocket;
    m_hvSocketThread = new QThread;
    m_hvSocket->moveToThread(m_hvSocketThread);
    connect(m_hvSocketThread, &QThread::finished, m_hvSocket, &QObject::deleteLater);
    connect(this, SIGNAL(HVConnect(QString, int)),
            m_hvSocket, SLOT(SetServer(QString, int)));
    connect(this, SIGNAL(HVDisconnect()),
            m_hvSocket, SLOT(disconnect()));
    connect(this, SIGNAL(HVSendData(QByteArray)),
            m_hvSocket, SLOT(SendData(QByteArray)));
    connect(m_hvSocket, SIGNAL(Connected(QString, int)),
            this, SLOT(HVConnected(QString, int)));
    connect(m_hvSocket, SIGNAL(DisConnected(QString, int)),
            this, SLOT(HVDisconnected(QString, int)));
    connect(m_hvSocket, SIGNAL(RecieveData(QByteArray)),
            m_hvReciever, SLOT(DoRecive(QByteArray)));
    connect(m_hvSocket, SIGNAL(RecieveData(QByteArray)),
            m_zoomReceiver, SLOT(DoRecive(QByteArray)));

    m_hvSocketThread->start();

    m_hvController = new ptz_controller();
    m_ControllerThread = new QThread();
    m_hvController->moveToThread(m_ControllerThread);
    connect(m_ControllerThread,&QThread::finished,m_hvController,&QObject::deleteLater);
    connect(m_hvController,SIGNAL(CtlSend(QByteArray)),m_hvSocket,SLOT(SendData(QByteArray)));
    connect(m_hvSocket, SIGNAL(RecieveData(QByteArray)),
            m_hvController,SLOT(GetArray(QByteArray)));
    connect(this,&PtzThreads::SendAngle,m_hvController,[=]{m_hvController->CheckAngle=Check;});
    connect(m_hvController,&ptz_controller::CheckDone,this,[=]{emit CheckDoneFromControll();});
    connect(m_hvController,&ptz_controller::StepMaxDone,this,[=]{emit StepMaxDoneFromControll();});
    m_ControllerThread->start();

    m_controllerTimer = new QTimer();
    TimerThread = new QThread();
    m_controllerTimer->moveToThread(TimerThread);
    connect(this,SIGNAL(Timestart(int)),m_controllerTimer,SLOT(start(int)));
    connect(m_controllerTimer,SIGNAL(timeout()),m_hvController,SLOT(CtlMotor()));
    connect(m_hvController,SIGNAL(Timestop()),m_controllerTimer,SLOT(stop()));
    connect(TimerThread,&QThread::finished,m_controllerTimer,&QObject::deleteLater);
    TimerThread->start();
    //emit m_hvController->Timestart(200);

//    m_zoomSocket = new TcpSocket;
//    m_zoomSocketThread = new QThread;
//    m_zoomSocket->moveToThread(m_zoomSocketThread);
//    connect(m_hvSocketThread, &QThread::finished, m_hvSocket, &QObject::deleteLater);
//    connect(this, SIGNAL(ZoomConnect(QString, int)),
//            m_zoomSocket, SLOT(SetServer(QString, int)));
//    connect(this, SIGNAL(ZoomDisconnect()),
//            m_zoomSocket, SLOT(disconnect()));
//    connect(this, SIGNAL(ZoomSendData(QByteArray)),
//            m_zoomSocket, SLOT(SendData(QByteArray)));
//    connect(m_zoomSocket, SIGNAL(Connected(QString, int)),
//            this, SLOT(ZoomeConnected(QString, int)));
//    connect(m_zoomSocket, SIGNAL(DisConnected(QString, int)),
//            this, SLOT(ZoomeDisconnected(QString, int)));
//    connect(m_zoomSocket, SIGNAL(RecieveData(QByteArray)), m_zoomReceiver, SLOT(DoRecive(QByteArray)));
//    m_zoomSocketThread->start();


}




void PtzThreads::Run()
{
    auto config = new QSettings("./config.ini", QSettings::IniFormat);
    emit HVConnect(config->value("PTZ/PtzIp").toString(),
                   config->value("PTZ/PtzPort").toInt());
//    emit ZoomConnect(config->value("PTZ/ZoomIp").toString(),
//                     config->value("PTZ/ZoomPort").toInt());
}


void PtzThreads::Quit()
{
    HVDisconnect();
//    ZoomDisconnect();
}


void PtzThreads::HVConnected(QString, int)
{
    emit HVConnectState(true);
}

void PtzThreads::HVDisconnected(QString, int)
{
    emit HVConnectState(false);
}

void PtzThreads::ZoomeConnected(QString, int)
{
    emit ZoomeConnectState(true);
}

void PtzThreads::ZoomeDisconnected(QString, int)
{
    emit ZoomeConnectState(false);
}


void PtzThreads::RRotate()
{
    emit HVSendData(PtzProtocol::Right());
}

void PtzThreads::LRotate()
{
    emit HVSendData(PtzProtocol::Left());
}

void PtzThreads::HAngleSet(double angle)
{
    char datah = ((int)angle) >> 8;
    char datal = (int)angle;
    emit HVSendData(PtzProtocol::SetHorizontalAngle(datah, datal));
}

void PtzThreads::URotate()
{
    emit HVSendData(PtzProtocol::Up());
}

void PtzThreads::DRotate()
{
    emit HVSendData(PtzProtocol::Down());
}

void PtzThreads::VAngleSet(double angle)
{
    char datah = ((int)angle) >> 8;
    char datal = (int)angle;
    emit HVSendData(PtzProtocol::SetVerticalAngle(datah, datal));
}

void PtzThreads::RotateStop()
{
    emit HVSendData(PtzProtocol::StopPtz());
}

void PtzThreads::ZIn()
{
    emit HVSendData(PtzProtocol::ZoomIn());
}

void PtzThreads::ZOut()
{
    emit HVSendData(PtzProtocol::ZoomOut());
}

void PtzThreads::ZSet(double num)
{
    if(num < 150 && num > 600) {
        return;
    } else {
        emit HVSendData(PtzProtocol::SetZoomNum(num));
    }
}

void PtzThreads::ZStop()
{
    emit HVSendData(PtzProtocol::ZoomStop());
}

void PtzThreads::fout(int a)
{
    qDebug()<<"a="<<a;
}


