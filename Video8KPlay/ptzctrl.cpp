#include "ptzctrl.h"

PtzCtrl::PtzCtrl(QWidget *parent) : QWidget(parent)
{
    InitUi();
    InitThread();
}


PtzCtrl::~PtzCtrl()
{
    TcpQuit();
    m_ptzThread->quit();
    m_ptzThread->wait();
}






void PtzCtrl::InitUi()
{
    this->setWindowTitle(QString::fromLocal8Bit("云台控制"));
    this->resize(700, 400);
    m_stateShow = new QLabel;
    //m_stateShow->setText(QString::fromLocal8Bit("旋转设备未连接；伸缩设备未连接"));
    m_stateShow->setText(QString::fromLocal8Bit("云台设备未连接"));
    m_hLRotate = new QPushButton(QString::fromLocal8Bit("左"));
    connect(m_hLRotate, &QPushButton::pressed, this, [=] {emit LRotate();});
    connect(m_hLRotate, &QPushButton::released, this, [=] {emit RotateStop();});
    m_hRRotate = new QPushButton(QString::fromLocal8Bit("右"));
    connect(m_hRRotate, &QPushButton::pressed, this, [=] {emit RRotate();});
    connect(m_hRRotate, &QPushButton::released, this, [=] {emit RotateStop();});
    m_vURotate = new QPushButton(QString::fromLocal8Bit("上"));
    m_vURotate->setEnabled(false);
    connect(m_vURotate, &QPushButton::pressed, this, [=] {emit URotate();});
    connect(m_vURotate, &QPushButton::released, this, [=] {emit RotateStop();});
    m_vDRotate = new QPushButton(QString::fromLocal8Bit("下"));
    m_vDRotate->setEnabled(false);
    connect(m_vDRotate, &QPushButton::pressed, this, [=] {emit DRotate();});
    connect(m_vDRotate, &QPushButton::released, this, [=] {emit RotateStop();});
    m_zORotate = new QPushButton(QString::fromLocal8Bit("伸"));
    connect(m_zORotate, &QPushButton::pressed, this, [=] {emit ZOut();});
    connect(m_zORotate, &QPushButton::released, this, [=] {emit ZStop();});
    m_zIRotate = new QPushButton(QString::fromLocal8Bit("缩"));
    connect(m_zIRotate, &QPushButton::pressed, this, [=] {emit ZIn();});
    connect(m_zIRotate, &QPushButton::released, this, [=] {emit ZStop();});

    m_Focus = new QRadioButton(QString::fromLocal8Bit("自动对焦"));
//    connect(m_Focus,&QPushButton::pressed,QPushButton,[=]{emit toggled();});


    auto hButtonLaout = new QHBoxLayout;
    hButtonLaout->addWidget(m_hLRotate);
    hButtonLaout->addWidget(m_hRRotate);

    auto hAngleSetLabel = new QLabel(QString::fromLocal8Bit("水平角度设置"));
    m_hAngleSet = new QLineEdit;
    m_hAngleSetButton = new QPushButton(QString::fromLocal8Bit("水平角度设置"));
    connect(m_hAngleSetButton, &QPushButton::clicked, this, [=] {
        emit HAngleSet(m_hAngleSet->text().toDouble());});
    auto hAngleSetLaout = new QHBoxLayout;
    hAngleSetLaout->addWidget(hAngleSetLabel);
    hAngleSetLaout->addWidget(m_hAngleSet);
    hAngleSetLaout->addWidget(m_hAngleSetButton);


    auto vAngleSetLabel = new QLabel(QString::fromLocal8Bit("垂直角度设置"));
    m_vAngleSet = new QLineEdit;
    m_vAngleSet->setEnabled(false);
    m_vAngleSetButton = new QPushButton(QString::fromLocal8Bit("垂直角度设置"));
    m_vAngleSetButton->setEnabled(false);
    connect(m_vAngleSetButton, &QPushButton::clicked, this, [=] {
        emit VAngleSet(m_vAngleSet->text().toDouble());});
    auto vAngleSetLaout = new QHBoxLayout;
    vAngleSetLaout->addWidget(vAngleSetLabel);
    vAngleSetLaout->addWidget(m_vAngleSet);
    vAngleSetLaout->addWidget(m_vAngleSetButton);

    auto zButtonLaout = new QHBoxLayout;
    zButtonLaout->addWidget(m_zORotate);
    zButtonLaout->addWidget(m_zIRotate);

    auto focusLaout = new QHBoxLayout;
    focusLaout->addWidget(m_Focus);



    auto zAngleSetLabel = new QLabel(QString::fromLocal8Bit("伸缩度设置"));
    m_zAngleSet = new QLineEdit;
    m_zAngleSetButton = new QPushButton(QString::fromLocal8Bit("伸缩度设置"));
    connect(m_zAngleSetButton, &QPushButton::clicked, this, [=] {
        emit ZSet(m_zAngleSet->text().toDouble());});
    auto zAngleSetLaout = new QHBoxLayout;
    zAngleSetLaout->addWidget(zAngleSetLabel);
    zAngleSetLaout->addWidget(m_zAngleSet);
    zAngleSetLaout->addWidget(m_zAngleSetButton);

    auto buttonLayout = new QVBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_stateShow);
    buttonLayout->addWidget(m_vURotate);
    buttonLayout->addLayout(hButtonLaout);
    buttonLayout->addWidget(m_vDRotate);
    buttonLayout->addLayout(hAngleSetLaout);
    buttonLayout->addLayout(vAngleSetLaout);
    buttonLayout->addStretch();
    buttonLayout->addLayout(zButtonLaout);
    buttonLayout->addLayout(zAngleSetLaout);
    buttonLayout->addStretch();
    buttonLayout->addLayout(focusLaout);
     buttonLayout->addStretch();






    auto hStateLable = new QLabel(QString::fromLocal8Bit("水平角度："));
    m_hStateShow = new QLineEdit();
    m_hStateShow->setEnabled(false);
    m_hStateShow->setMaximumWidth(100);
    auto vStateLable = new QLabel(QString::fromLocal8Bit("垂直角度："));
    m_vStateShow = new QLineEdit();
    m_vStateShow->setEnabled(false);
    m_vStateShow->setMaximumWidth(100);
    auto hStateLaout = new QHBoxLayout;
    hStateLaout->addStretch();
    hStateLaout->addWidget(hStateLable);
    hStateLaout->addWidget(m_hStateShow);
    hStateLaout->addStretch();
    auto vStateLaout = new QHBoxLayout;
    vStateLaout->addStretch();
    vStateLaout->addWidget(vStateLable);
    vStateLaout->addWidget(m_vStateShow);
    vStateLaout->addStretch();

    auto zStateLable = new QLabel(QString::fromLocal8Bit("伸缩度："));
    m_zoomStateShow = new QLineEdit;
    m_zoomStateShow->setEnabled(false);
    m_zoomStateShow->setMaximumWidth(100);
    auto zoomStateLaout = new QHBoxLayout;
    zoomStateLaout->addStretch();
    zoomStateLaout->addWidget(zStateLable);
    zoomStateLaout->addWidget(m_zoomStateShow);
    zoomStateLaout->addStretch();

    auto stateLayout = new QVBoxLayout;
    stateLayout->addLayout(hStateLaout);
    stateLayout->addLayout(vStateLaout);
    stateLayout->addLayout(zoomStateLaout);

    auto mainLayout = new QHBoxLayout;
    mainLayout->addLayout(buttonLayout);
    mainLayout->addLayout(stateLayout);

    mainLayout->setStretchFactor(buttonLayout, 2);
    mainLayout->setStretchFactor(stateLayout, 1);

    this->setLayout(mainLayout);

}

void PtzCtrl::InitThread()
{
    m_ptz = new PtzThreads();
    m_ptzThread = new QThread;
    m_ptz->moveToThread(m_ptzThread);
    connect(m_ptzThread, &QThread::finished, m_ptz, &QObject::deleteLater);
    connect(this, SIGNAL(TcpRun()), m_ptz, SLOT(Run()));
    connect(this, SIGNAL(TcpQuit()), m_ptz, SLOT(Quit()));
    connect(this, SIGNAL(RRotate()), m_ptz, SLOT(RRotate()));
    connect(this, SIGNAL(LRotate()), m_ptz, SLOT(LRotate()));
    connect(this, SIGNAL(HAngleSet(double)), m_ptz, SLOT(HAngleSet(double)));
    connect(this, SIGNAL(URotate()), m_ptz, SLOT(URotate()));
    connect(this, SIGNAL(DRotate()), m_ptz, SLOT(DRotate()));
    connect(this, SIGNAL(VAngleSet(double)), m_ptz, SLOT(VAngleSet(double)));
    connect(this, SIGNAL(RotateStop()), m_ptz, SLOT(RotateStop()));
    connect(this, SIGNAL(ZIn()), m_ptz, SLOT(ZIn()));
    connect(this, SIGNAL(ZOut()), m_ptz, SLOT(ZOut()));
    connect(this, SIGNAL(ZSet(double)), m_ptz, SLOT(ZSet(double)));
    connect(this, SIGNAL(ZStop()), m_ptz, SLOT(ZStop()));
    connect(this,&PtzCtrl::SendAngle,m_ptz,[=]{emit m_ptz->SendAngle();});
    connect(m_ptz,&PtzThreads::CheckDoneFromControll,this,[=]{emit CheckDonefromPtzThread();});
    connect(m_ptz,&PtzThreads::StepMaxDoneFromControll,this,[=]{emit StepMaxDonefromPtzThread();});
    connect(m_ptz, &PtzThreads::HVConnectState,
            this, [=](bool state) {
        m_hvDevConnected = state;
        ConnectStateUpdate();});
    connect(m_ptz, &PtzThreads::ZoomeConnectState,
            this, [=](bool state) {
        m_zoomDevConnected = state;
        ConnectStateUpdate();});
    connect(m_ptz, &PtzThreads::HAngle,
            this, [=](double angle) {
        m_hStateShow->setText(QString::number(angle));});
    connect(m_ptz, &PtzThreads::VAngle,
            this, [=](double angle) {
        m_vStateShow->setText(QString::number(angle));});
    connect(m_ptz, &PtzThreads::ZoomAngle,
            this, [=](double angle) {
    m_zoomStateShow->setText(QString::number(angle));});

    connect(m_Focus,&QRadioButton::toggled,m_ptz,[=]{
        if(m_Focus->isChecked())
        {
         emit  m_ptz->Timestart(50);
         emit CapturePic();
        }
        else
        {
         emit  m_ptz->Timestop();
         emit StopCapturePic();
        }
        });
    m_ptzThread->start();
}


void PtzCtrl::ConnectStateUpdate()
{
    QString str1 = m_hvDevConnected ? QString::fromLocal8Bit("云台设备已连接；"):
                                      QString::fromLocal8Bit("云台设备未连接；");
//    QString str2 = m_zoomDevConnected ? QString::fromLocal8Bit("伸缩设备已连接"):
//                                        QString::fromLocal8Bit("伸缩设备未连接");
    m_stateShow->setText(str1);
}



void PtzCtrl::showEvent(QShowEvent *event)
{
    emit TcpRun();
    QWidget::showEvent(event);
}

void PtzCtrl::hideEvent(QHideEvent *event)
{
    emit TcpQuit();
    QWidget::hideEvent(event);
}
