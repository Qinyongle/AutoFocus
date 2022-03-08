#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include <QFileDialog>
#include <QGridLayout>
#include <QDateTime>
#include <QSettings>
#include "webapi/webapi.h"
#include <QDesktopWidget>
#include <QFile>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_mainWindow(new MainWindowWidget)
{
    QDir::setCurrent(QCoreApplication::applicationDirPath());

    ui->setupUi(this);
    qRegisterMetaType<QSize>("QSize");
    qRegisterMetaType<QResizeEvent *>("QResizeEvent *");
    qRegisterMetaType<QAction *>("QAction *");
//    connect(this,SIGNAL(BlurTest()),this,SLOT(PicBlurTest()));
    InitUi();
    InitThread();
    InitConfig();

    InitZoneSize();
    InitPicture();
}

MainWindow::~MainWindow()
{
    m_ptzCtrlWindow->close();
    m_ptzCtrlWindow->deleteLater();
    m_aboutWidget->close();
    m_aboutWidget->deleteLater();
    m_streamConfig->close();
    m_streamConfig->deleteLater();
    m_cameraConfigWidget->close();
    m_cameraConfigWidget->deleteLater();
    AllZoneClose();
    emit RunStopCmd();
    m_ffmpegThread->quit();
    m_ffmpegThread->wait();
    emit AudioQuit(false);
    m_audioPlayThread->quit();
    m_audioPlayThread->wait();
    qDebug() << "MainWindow::~MainWindow";
    delete ui;
}

int MainWindow::getWindowX()
{
   return this->pos().x();
}

int MainWindow::getWindowY()
{
    return this->pos().y();
}

void MainWindow::resizeEvent(QResizeEvent* size)
{
    if(m_originalLable != nullptr && m_originalLable->m_qimageSize.width() != 0) {
        double d = m_originalLable->m_qimageSize.width() / (m_originalLable->m_qimageSize.height() * 1.0);
        if(size->size().width() / (size->size().height()) > d) {
            m_originalLable->resize((size->size().height()) * d,
                                    size->size().height());
        } else {
            m_originalLable->resize(size->size().width(),
                                    size->size().width() / d);
        }
    }

}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    //键盘空格控制播放暂停
    switch (event->key())
        {
        case Qt::Key_F1:
        SwitchStream1();
        break;
        case Qt::Key_F2:
        SwitchStream2();
        break;
    }
}

void MainWindow::SwitchStream1()
{
    SendSwitch(m_url);
}

void MainWindow::SwitchStream2()
{
    SendSwitch(switch_url);
}

void MainWindow::InitPicture()
{
    mainPro = new MainProgram();
    m_thread = new QThread();
    mainPro->moveToThread(m_thread);
    connect(m_thread, &QThread::finished, mainPro, &QObject::deleteLater);
    m_thread->start();
    //发送图片间隔---配置文件中读取
    mainPro->SetSendTimeInterval(m_differ);
    mainPro->SetServiceUrl(Server_Url);
    connect(this, SIGNAL(SendPicState()),
            mainPro, SLOT(LoadThread()));
    connect(mainPro, SIGNAL(GetPicCmd()),
            m_ffmpeg, SLOT(GetPicCmd()));
    connect(m_ffmpeg, SIGNAL(SendPicPath(QString)),
            mainPro, SLOT(GetPicPath(QString)));

//    connect(m_ffmpeg->GetFrameSignalPoint(), SIGNAL(SendPicPath(QString)),
//            mainPro, SLOT(GetPicPath(QString)));

    //gps_server_listen
    mainPro->SetGpsTcp(Gps_Ip,Gps_Port);


    m_hvOpencv = new PtzOpencv;
    m_hvOpencvThread = new QThread;
    m_hvOpencv->moveToThread(m_hvOpencvThread);
    PtzOpencv::connect(m_hvOpencvThread,&QThread::finished,m_hvOpencv,&QObject::deleteLater);
    PtzOpencv::connect(m_hvOpencv,&PtzOpencv::imageReady,m_hvOpencv,&PtzOpencv::OpenCvLaplacian);     //当接收到照片准备好，则执行分析图片
    PtzOpencv::connect(m_hvOpencv,&PtzOpencv::SendAngle,m_ptzCtrlWindow,[=]{emit m_ptzCtrlWindow->SendAngle();});
    PtzOpencv::connect(m_ptzCtrlWindow,&PtzCtrl::CheckDonefromPtzThread,m_hvOpencv,[=]{m_hvOpencv->CheckDone=1;});
    PtzOpencv::connect(m_ptzCtrlWindow,&PtzCtrl::StepMaxDonefromPtzThread,m_hvOpencv,[=]{m_hvOpencv->StepMaxDone=1;});
    m_hvOpencvThread->start();


    capture=new ImageCapture;
    m_imageCapture = new QThread;
    capture->moveToThread(m_imageCapture);
    ImageCapture::connect(m_imageCapture,&QThread::finished,capture,&QWidget::deleteLater);
    ImageCapture::connect(capture,&ImageCapture::capture_trigg,capture,&ImageCapture::capture);
    connect(this,SIGNAL(BlurTest()),this,SLOT(PicBlurTest()));
    m_imageCapture->start();
}




void MainWindow::InitUi()
{

    const int windowWidth = 1250;
    const int windowHeight = windowWidth / 16.0 * 9 + 100;
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowIcon(QIcon("./pic/mainLogo.png"));
    this->resize(windowWidth,windowHeight);
    this->setMinimumWidth(10);
    this->setMinimumHeight(150);
    this->setWindowTitle(QString::fromLocal8Bit("CTMT播放器"));
    this->setCentralWidget(m_mainWindow);
    QDesktopWidget *desk = QApplication::desktop();
    //显示屏个数
    qDebug()<<desk->screenCount();
    //获取主屏幕索引序列
    int primScreen = desk->primaryScreen();
    QRect rect = desk->screenGeometry(primScreen);
    //判断主副显示器序列号
    if(primScreen > 0){
        Aux_rect = desk->screenGeometry(0);
    }
    else{
        Aux_rect = desk->screenGeometry(primScreen+1);
    }
    qDebug()<<rect<<Aux_rect;
    this->move((rect.width() - windowWidth) / 2,
               (rect.height() - windowHeight) / 2);
    m_mainWindow->show();


    m_menuBar = ui->menubar;
    m_menuBar->setGeometry(0,0,width(),80);   //设置大小
    m_menuBar->setStyleSheet("font:10pt");
    // 功能
    m_open = new QMenu(QString::fromLocal8Bit("功能"));
    //m_open->setStyleSheet("background:rgb(105,105,105);color:white;");
    m_openFile = new QAction(QString::fromLocal8Bit("打开文件"));
    connect(m_openFile,SIGNAL(triggered()),this,SLOT(OpenFile()));
    m_closePlay = new QAction(QString::fromLocal8Bit("关闭播放"));
    connect(m_closePlay,SIGNAL(triggered()),this,SLOT(ClosePlay()));
    m_openStream = new QAction(QString::fromLocal8Bit("打开串流"));
    connect(m_openStream,SIGNAL(triggered()),this,SLOT(OpenStream()));
    m_open->addAction(m_openFile);
    m_open->addAction(m_openStream);
    m_open->addAction(m_closePlay);
    m_menuBar->addMenu(m_open);
    // 解码
    m_decodeConfigMenu = new QMenu(QString::fromLocal8Bit("解码器配置"));
    m_decodeSelectMenu = new QMenu(QString::fromLocal8Bit("解码器选择"));
    connect(m_decodeSelectMenu,SIGNAL(triggered(QAction *)),
            this, SLOT(DecodeMenuTriggered(QAction *)));
    m_intelDecodeAct = new QAction(QString::fromLocal8Bit("因特尔解码器"));
    m_intelDecodeAct->setCheckable(true);
    m_intelDecodeAct->setChecked(true);
    m_nvidiaDecodeAct = new QAction(QString::fromLocal8Bit("英伟达解码器"));
    m_nvidiaDecodeAct->setCheckable(true);
    m_decodeSelectMenu->addAction(m_intelDecodeAct);
    m_decodeSelectMenu->addAction(m_nvidiaDecodeAct);
    m_decodeConfigMenu->addMenu(m_decodeSelectMenu);
    m_menuBar->addMenu(m_decodeConfigMenu);
    // 选区
    m_selectedWindow = new QMenu(QString::fromLocal8Bit("选区配置"));
    m_zoneNumSet = new QMenu(QString::fromLocal8Bit("数量配置"));
    connect(m_zoneNumSet,SIGNAL(triggered(QAction *)),
            this, SLOT(ZoneNumMenuTriggered(QAction *)));
    for(int i = 1; i < ZONE_NUM; i++) {
        m_zoneNumAct[i] = new QAction(QString::number(i));
        m_zoneNumAct[i]->setCheckable(true);
        if(i == 1) {
            m_zoneNumAct[i]->setChecked(true);
        }
        m_zoneNumSet->addAction(m_zoneNumAct[i]);
    }
    m_selectedWindow->addMenu(m_zoneNumSet);
    m_menuBar->addMenu(m_selectedWindow);

    // 云台
    m_ptzConfigWindow = new PtzConfig();
    m_ptzConfigWindow->hide();
    m_ptzCtrlWindow = new PtzCtrl();
    m_ptzCtrlWindow->hide();
    m_ptzMenu = new QMenu(QString::fromLocal8Bit("云台"));
    m_ptzConfigAction = new QAction(QString::fromLocal8Bit("云台配置"));
    connect(m_ptzCtrlWindow,&PtzCtrl::CapturePic,this,[=] {emit GetPosStart(50);});
    connect(m_ptzCtrlWindow,&PtzCtrl::StopCapturePic,this,[=]{emit GetPosStop();MaxBlurValue=0;});
    connect(m_ptzConfigAction,&QAction::triggered,this,[=]{
        m_ptzConfigWindow->show();
    });
    m_ptzMenu->addAction(m_ptzConfigAction);

    m_ptzCtrlAction = new QAction(QString::fromLocal8Bit("云台控制"));
    connect(m_ptzCtrlAction,&QAction::triggered,this,[=]{
        //m_ptzCtrlWindow->setWindowFlag(Qt::WindowStaysOnTopHint);     //置顶窗
        m_ptzCtrlWindow->show();
        m_ptzCtrlWindow->raise();
    });
    m_ptzMenu->addAction(m_ptzCtrlAction);
    m_menuBar->addMenu(m_ptzMenu);

    // 相机设置
    m_cameraConfig = new QAction(QString::fromLocal8Bit("相机设置"));
    connect(m_cameraConfig,SIGNAL(triggered()),
            this, SLOT(CameraConfigConfig()));
    m_menuBar->addAction(m_cameraConfig);

    // 所有选取置顶
    m_selectedZoneBeTop = new QAction(QString::fromLocal8Bit("所有选区置顶"));
    m_menuBar->addAction(m_selectedZoneBeTop);

    // 关于
    m_aboutAct = new QAction(QString::fromLocal8Bit("关于"));
    connect(m_aboutAct,SIGNAL(triggered()),
            this, SLOT(AboutActionTriggered()));
    m_aboutWidget = new QWidget();
    m_aboutWidget->setFixedSize(400, 300);
    m_aboutWidget->setWindowTitle(QString::fromLocal8Bit("关于"));
    QLabel *aboutLable = new QLabel();
    aboutLable->setText(QString::fromLocal8Bit("软件供应商：深圳市置辰海信科技有限公司\r\n\r\n"
                                               "软件版本号：V1.1.1"));
    aboutLable->setAlignment(Qt::AlignCenter);
    QHBoxLayout *aboutLaout = new QHBoxLayout();
    aboutLaout->addWidget(aboutLable);
    aboutLaout->setContentsMargins(0, 0, 0, 0);
    m_aboutWidget->setLayout(aboutLaout);
    m_menuBar->addAction(m_aboutAct);

    // 显示区
    m_originalLable = new MosaicLable(this);
    m_originalLable->hide();
    connect(this,SIGNAL(SetDrawZoneSize(int)),
            m_originalLable,SLOT(GetDrawZoneSize(int)));
    connect(m_originalLable,SIGNAL(SpaceKeyPress()),
            this,SLOT(LableKeyPressed()));
    connect(m_originalLable,SIGNAL(FocusZone(int, int, int, int)),
            this,SLOT(ConfigFocusZone(int, int, int, int)));
    m_originalLable->raise();

//    QHBoxLayout *lableLayout = new QHBoxLayout();
//    lableLayout->addStretch();
//    lableLayout->addWidget(m_originalLable);
//    lableLayout->addStretch();
//    lableLayout->setStretchFactor(m_originalLable, 1);

    // 进度条
    m_playButton = new QPushButton();
    m_playButton->setStyleSheet("QPushButton{background-color:black; height:30;\
                                color: white;   border-radius: 10px;  border: 1px groove gray;\
                                border-style: outset;}"
                               "QPushButton:hover{background-color:white; color: black;}"
                              "QPushButton:pressed{background-color:rgb(85, 170, 255);\
                                               border-style: inset; }");
    m_playButton->setText(QString::fromLocal8Bit("播放"));//border-radius:10px;
    m_playButton->setFixedHeight(30);
    connect(m_playButton, SIGNAL(clicked()), this, SLOT(PlayButtonClicked()));

    m_numShow = new QLineEdit();
    m_numShow->setEnabled(false);
    m_numShow->setText("0:0:0");
    m_numShow->setStyleSheet("background:rgb(10,10,10);color:white;border-width:0;border-style:outset");

    m_speedSlider = new QSlider();
    m_speedSlider->setOrientation(Qt::Horizontal); //
    connect(m_speedSlider, SIGNAL(valueChanged(int)), this, SLOT(SliderValueChanged(int)));
    connect(m_speedSlider, SIGNAL(sliderPressed()), this, SLOT(SliderPressed()));
    connect(m_speedSlider, SIGNAL(sliderReleased()), this, SLOT(SliderReleased()));

    // 音量调节
    m_volumeButton = new QPushButton();
    m_volumeButton->setStyleSheet("QPushButton{background-color:black; height:30;\
                                color: white;   border-radius: 10px;  border: 1px groove gray;\
                                border-style: outset;}"
                               "QPushButton:hover{background-color:white; color: black;}"
                              "QPushButton:pressed{background-color:rgb(85, 170, 255);\
                                               border-style: inset; }");
    m_volumeButton->setFixedHeight(30);
    m_volumeButton->setText(QString::fromLocal8Bit("音量"));
    connect(m_volumeButton, SIGNAL(clicked()), this, SLOT(VolumeButtonClicked()));

    m_volumeShow = new QLineEdit();
    m_volumeShow->setEnabled(false);
    m_volumeShow->setText("0");
    m_volumeShow->setStyleSheet("background:rgb(10,10,10);color:white;border-width:0;border-style:outset");


    m_volumeSlider = new QSlider();
    m_volumeSlider->setOrientation(Qt::Horizontal);
    QFile qss(":/style/style/volumSlider.qss");
    qss.open(QFile::ReadOnly);
    m_volumeSlider->setStyleSheet(qss.readAll());
    connect(m_volumeSlider, SIGNAL(sliderMoved(int)), this, SLOT(VolumeSliderMoved(int)));
    connect(m_volumeSlider, SIGNAL(valueChanged(int)),
            this, SLOT(VolumeSliderVaalueChanged(int)));
    qss.close();

    m_volumeSlider->setMaximum(100);
    m_volumeSlider->setMinimum(0);

    QHBoxLayout *palySpeedLayout = new QHBoxLayout();
    palySpeedLayout->addWidget(m_playButton);
    palySpeedLayout->addWidget(m_numShow);
    palySpeedLayout->addWidget(m_speedSlider);
    palySpeedLayout->addSpacing(30);
    palySpeedLayout->addWidget(m_volumeButton);
    palySpeedLayout->addWidget(m_volumeShow);
    palySpeedLayout->addWidget(m_volumeSlider);
    palySpeedLayout->setStretchFactor(m_playButton, 2);
    palySpeedLayout->setStretchFactor(m_numShow, 2);
    palySpeedLayout->setStretchFactor(m_speedSlider, 20);
    palySpeedLayout->setStretchFactor(m_volumeButton, 2);
    palySpeedLayout->setStretchFactor(m_volumeShow, 1);
    palySpeedLayout->setStretchFactor(m_volumeSlider, 2);


    m_frameSpeedLable = new QLabel(QString::fromLocal8Bit("视频帧率："));
    m_frameSpeedLable->setStyleSheet("background:rgb(10,10,10);color:white;max-width:200");
    m_frameSpeedLable->setText(QString(QString::fromLocal8Bit("原始视频帧率：") + "%0").arg(0));
    m_speedNumLable = new QLabel(QString::fromLocal8Bit("延时速度值（单位ms/帧）"));
    m_speedNumLable->setStyleSheet("background:rgb(10,10,10);color:white;max-width:180");
    m_speedNumSpinBox = new QSpinBox();
    m_speedNumSpinBox->setStyleSheet("background:rgb(10,10,10);color:white;border-width:0;max-width:70");
    m_speedNumSpinBox->setMaximum(MAX_DELAY_SPEED);
    m_speedNumSpinBox->setMinimum(MIN_DELAY_SPEED);
    connect(m_speedNumSpinBox, SIGNAL(valueChanged(int)), this, SLOT(SpeedNumSpinBoxValueChanged(int)));
    m_addSpeed = new QPushButton(QString::fromLocal8Bit("增加10ms延时"));
    m_addSpeed->setStyleSheet("QPushButton{background-color:black; height:30;max-width:150;\
                              color: white;   border-radius: 10px;  border: 1px groove gray;\
                              border-style: outset;}"
                             "QPushButton:hover{background-color:white; color: black;}"
                            "QPushButton:pressed{background-color:rgb(85, 170, 255);\
                                             border-style: inset; }");
    connect(m_addSpeed, SIGNAL(clicked()), this, SLOT(AddSpeedClicked()));
    m_reduceSpeed = new QPushButton(QString::fromLocal8Bit("减少10ms延时"));
    m_reduceSpeed->setStyleSheet("QPushButton{background-color:black; height:30;max-width:150;\
                                 color: white;   border-radius: 10px;  border: 1px groove gray;\
                                 border-style: outset;}"
                                "QPushButton:hover{background-color:white; color: black;}"
                               "QPushButton:pressed{background-color:rgb(85, 170, 255);\
                                                border-style: inset; }");
    connect(m_reduceSpeed, SIGNAL(clicked()), this, SLOT(ReduceSpeedClicked()));
    m_resetSpeed = new QPushButton(QString::fromLocal8Bit("关闭延时"));
    m_resetSpeed->setStyleSheet("QPushButton{background-color:black; height:30;max-width:150;\
                                color: white;   border-radius: 10px;  border: 1px groove gray;\
                                border-style: outset;}"
                               "QPushButton:hover{background-color:white; color: black;}"
                              "QPushButton:pressed{background-color:rgb(85, 170, 255);\
                                               border-style: inset; }");
    connect(m_resetSpeed, SIGNAL(clicked()), this, SLOT(ResetSpeedClicked()));

    QHBoxLayout *palySpeedReviceLayout = new QHBoxLayout();
    palySpeedReviceLayout->addWidget(m_frameSpeedLable);
    palySpeedReviceLayout->addStretch();
    palySpeedReviceLayout->addWidget(m_speedNumLable);
    palySpeedReviceLayout->addWidget(m_speedNumSpinBox);
    palySpeedReviceLayout->addStretch();
    palySpeedReviceLayout->addWidget(m_addSpeed);
    palySpeedReviceLayout->addStretch();
    palySpeedReviceLayout->addWidget(m_reduceSpeed);
    palySpeedReviceLayout->addStretch();
    palySpeedReviceLayout->addWidget(m_resetSpeed);
    palySpeedReviceLayout->addStretch();

    palySpeedReviceLayout->setStretchFactor(m_frameSpeedLable, 1);
    palySpeedReviceLayout->setStretchFactor(m_speedNumLable, 1);
    palySpeedReviceLayout->setStretchFactor(m_speedNumSpinBox, 1);
    palySpeedReviceLayout->setStretchFactor(m_addSpeed, 2);
    palySpeedReviceLayout->setStretchFactor(m_reduceSpeed, 2);
    palySpeedReviceLayout->setStretchFactor(m_resetSpeed, 2);


    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addStretch();
    mainLayout->addWidget(m_originalLable);
    mainLayout->addStretch();
    mainLayout->addLayout(palySpeedLayout);
    mainLayout->addLayout(palySpeedReviceLayout);
    mainLayout->setStretchFactor(m_originalLable, 20);
    mainLayout->setStretchFactor(palySpeedLayout, 1);
    mainLayout->setStretchFactor(palySpeedReviceLayout, 1);
    mainLayout->setContentsMargins(0, 0, 0, 0);


    m_mainWindow->setLayout(mainLayout);

    m_streamConfig = new StreamConfigWidget();
    connect(m_streamConfig, SIGNAL(SendStreamUrl(QString)), this, SLOT(GetStreamUrl(QString)));
    m_streamConfig->setFixedSize(400, 200);
    m_streamConfig->hide();

    m_cameraConfigWidget = new CameraConfigWidget();
    m_cameraConfigWidget->setFixedSize(400, 300);
    m_cameraConfigWidget->hide();

}

void MainWindow::InitConfig()
{
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    QSettings *config = new QSettings("./config.ini", QSettings::IniFormat);
    QString decodeType = config->value("SYS/DecodeType").toString();
    m_url = config->value("SYS/Url").toString();
    switch_url = config->value("SYS/SwitchUrl").toString();
    if(decodeType == "nvidia") {
        m_intelDecodeAct->setChecked(false);
        m_nvidiaDecodeAct->setChecked(true);
        m_videoDecodeType = QtFfmpegDecoder::CUDA;
    } else if(decodeType == "intel") {
        m_intelDecodeAct->setChecked(true);
        m_nvidiaDecodeAct->setChecked(false);
        m_videoDecodeType = QtFfmpegDecoder::QSV;
    }
    m_volumeZero = GetVolumeZeroConfig();
    m_volumeNumber = GetVolumeConfig();
    if(m_volumeNumber == 0) {
        m_volumeNumber = m_audioPlay->GetVolume() * 100;
    } else {
        emit SetVolume(m_volumeNumber / 100.0);
    }
    m_volumeSlider->setValue(m_volumeNumber);

    Server_Url = config->value("SYS/ServerUrl").toString();
    QString gps_msg = config->value("SYS/GPS").toString();
    Gps_Ip = gps_msg.mid(0,gps_msg.indexOf(":"));
    Gps_Port = gps_msg.mid(gps_msg.indexOf(":")+1).toInt();
    //读取存储视频路径
    m_path = config->value("SYS/SavePath").toString();
    m_name = config->value("SYS/VideoName").toString();
    m_differ = config->value("SYS/PicDiffer").toInt();
    qDebug()<<"m_path:"<<m_path<<m_name<<m_differ;
}

void MainWindow::InitThread()
{
    m_audioPlay = new AudioPlay();
    m_audioPlayThread = new QThread();
    m_audioPlay->moveToThread(m_audioPlayThread);
    connect(m_audioPlayThread, &QThread::finished,
            m_audioPlay, &QObject::deleteLater);
    connect(this, SIGNAL(AudioQuit(bool)),
            m_audioPlay, SLOT(RunState(bool)), Qt::DirectConnection);
    connect(this, SIGNAL(SetVolume(double)),
            m_audioPlay, SLOT(SetVolume(double)), Qt::DirectConnection);

    m_audioPlayThread->start(QThread::InheritPriority);



    m_getPosTimer =new QTimer();
    m_getPosTimerThread = new QThread();
    m_getPosTimer->moveToThread(m_getPosTimerThread);
    connect(this,SIGNAL(GetPosStart(int)),m_getPosTimer,SLOT(start(int)));
    connect(m_getPosTimerThread,&QThread::finished,m_getPosTimer,&QObject::deleteLater);
    connect(m_getPosTimer,SIGNAL(timeout()),this,SLOT(GetWindowPos()));
    connect(this,SIGNAL(GetPosStop()),m_getPosTimer,SLOT(stop()));
    m_getPosTimerThread->start();

   // m_getPosTimer->start(1000);
//    //实时窗口
//    m_ffmpeg = new VideoRealTimeGet(m_originalLable);
//    m_ffmpegThread = new QThread();
//    m_ffmpeg->moveToThread(m_ffmpegThread);
//    connect(m_ffmpegThread, &QThread::finished,
//                m_ffmpeg, &QObject::deleteLater);
//    connect(this, SIGNAL(RunStartCmd(QString, QtFfmpegDecoder::DECODE_TYPE)),
//            m_ffmpeg, SLOT(RunStart(QString, QtFfmpegDecoder::DECODE_TYPE)));
//    connect(this, SIGNAL(RunStopCmd()),
//            m_ffmpeg, SLOT(RunStop()), Qt::DirectConnection);
//    connect(this, SIGNAL(PlayingState(bool)),
//            m_ffmpeg, SLOT(SetPlayState(bool)), Qt::DirectConnection);
//    connect(m_ffmpeg, SIGNAL(FrameMasageSend(unsigned long long, double)),
//            this, SLOT(GetFrameData(unsigned long long, double)));
//    connect(m_ffmpeg, SIGNAL(FramePositionSend(double)),
//            this, SLOT(GetCurrentFramTime(double)));
//    connect(this, SIGNAL(SetVideoFrameInt(long)),
//            m_ffmpeg, SLOT(SetFrameTime(long)), Qt::DirectConnection);
//    connect(this, SIGNAL(SetDecodeSpeedDelay(int)),
//            m_ffmpeg, SLOT(SetPlayDelayTime(int)), Qt::DirectConnection);
//    // andio
//    connect(m_ffmpeg->GetFrameSignalPoint(), SIGNAL(SendAudio(QByteArray)),
//            m_audioPlay, SLOT(GetData(QByteArray)), Qt::DirectConnection);
//    connect(m_ffmpeg->GetFrameSignalPoint(), SIGNAL(AudioStart(int, int, int, bool)),
//            m_audioPlay, SLOT(ThreadStart(int, int, int, bool)));
////    connect(m_ffmpeg, SIGNAL(AudioQuit(bool)),
////            m_audioPlay, SLOT(RunState(bool)), Qt::DirectConnection);
//    m_ffmpegThread->start(QThread::LowPriority);
//    //emit AudioStart(48000, 2, 1000 / 25);

    m_ffmpeg = new RealPlayWidget(m_originalLable);
    m_ffmpegThread = new QThread();
    m_ffmpeg->moveToThread(m_ffmpegThread);
    connect(m_ffmpegThread, &QThread::finished,
            m_ffmpeg, &QObject::deleteLater);
    connect(this, SIGNAL(StartCmd(QString)),
            m_ffmpeg, SLOT(RunStart(QString)));
    connect(this, SIGNAL(RunStopCmd()),
            m_ffmpeg, SLOT(RunStop()), Qt::DirectConnection);

    connect(this, SIGNAL(SendSwitch(QString)),
            m_ffmpeg, SLOT(SwtichStream(QString)));
    //重连

//    connect(this, SIGNAL(PlayingState(bool)),
//            m_ffmpeg, SLOT(SetPlayState(bool)), Qt::DirectConnection);

    m_ffmpegThread->start();



    // 关于
    m_sendPic = new QAction(QString::fromLocal8Bit("图片发送"));
    connect(m_sendPic,SIGNAL(triggered()),
            this, SLOT(PictureActionTriggered()));
    m_menuBar->addAction(m_sendPic);
}

void MainWindow::OpenFile()
{
    //定义文件对话框类
    QFileDialog *fileDialog = new QFileDialog(this);
    //定义文件对话框标题
    fileDialog->setWindowTitle(QString::fromLocal8Bit("打开文件"));
    //设置默认文件路径
    fileDialog->setDirectory(".");
    //设置文件过滤器
    fileDialog->setNameFilter(tr("Video(*.MOV;*.mp4;*.m3u8)"));
    //设置可以选择多个文件,默认为只能选择一个文件QFileDialog::ExistingFiles
    fileDialog->setFileMode(QFileDialog::ExistingFile);
    //设置视图模式
    fileDialog->setViewMode(QFileDialog::Detail);
    //打印所有选择的文件的路径
    QStringList fileNames;
    if(fileDialog->exec()) {
        fileNames = fileDialog->selectedFiles();
    }
    for(auto tmp:fileNames) {
        m_filePath = tmp.toUtf8();
        qDebug()<<tmp<<endl;
    }
    emit RunStopCmd();
    OpenFfmpeg(m_filePath);
    m_speedNumSpinBox->setValue(0);
}

void MainWindow::OpenStream()
{

    m_streamConfig->show();
}

void MainWindow::GetStreamUrl(QString url)
{
    emit RunStopCmd();
    m_filePath = url;
    Ip_temp=url;
    OpenFfmpeg(m_filePath);
    //this->setWindowFlag(Qt::WindowStaysOnTopHint,true);
}

void MainWindow::ClosePlay()
{
    qDebug() << "close play";
    emit RunStopCmd();
    m_playButton->setText(QString::fromLocal8Bit("播放"));
     //this->setWindowFlag(Qt::WindowStaysOnTopHint,false);
}

void MainWindow::OpenFfmpeg(QString url)
{
    emit PlayingState(true);
    emit RunStartCmd(url, m_videoDecodeType);
    emit StartCmd(url);
    m_playButton->setText(QString::fromLocal8Bit("暂停"));
    m_mainWindow->NotBackgroud();
//    this->setWindowFlag(Qt::WindowStaysOnTopHint);
    this->show();

}

void MainWindow::PlayButtonClicked()
{
    if(m_playButton->text() == QString::fromLocal8Bit("播放")) {
        m_playButton->setText(QString::fromLocal8Bit("暂停"));
        emit PlayingState(true);
    } else {
        m_playButton->setText(QString::fromLocal8Bit("播放"));
        emit PlayingState(false);
    }
}

void MainWindow::LableKeyPressed()
{
    if(m_playButton->text() == QString::fromLocal8Bit("播放")) {
        m_playButton->setText(QString::fromLocal8Bit("暂停"));
        emit PlayingState(true);
    } else {
        m_playButton->setText(QString::fromLocal8Bit("播放"));
        emit PlayingState(false);
    }
}

void MainWindow::SliderValueChanged(int data)
{
    m_selectedFrameTime = data / SLIDER_SMOOTH_NUM;
    int dataNum = data / SLIDER_SMOOTH_NUM;
    int hour = dataNum / 3600;
    int min = dataNum % 3600 / 60;
    int sec = dataNum % 60;
    //qDebug() << "dataNum = " << dataNum << " " << min << " "<< sec;
    m_numShow->setText(QString::number(hour) + ":" +
                       QString::number(min) + ":" +
                       QString::number(sec));
}

void MainWindow::SliderPressed()
{
    emit PlayingState(false);
}


void  MainWindow::SliderReleased()
{
    emit PlayingState(true);
    qDebug() << "SliderReleassed" << m_selectedFrameTime;
    emit SetVideoFrameInt(m_selectedFrameTime);
    if(m_playButton->text() == QString::fromLocal8Bit("播放")) {
        m_playButton->setText(QString::fromLocal8Bit("暂停"));
    }
}

void MainWindow::GetFrameData(unsigned long long timeTotal, double frameRate)
{
//    m_videoMaxFramCount = framCount;
//    m_videoMaxTime = framCount / frameRate;
//    m_videoMaxTime = framCount / 1000000;
//    m_videoMaxFramCount = frameRate * m_videoMaxTime;
    m_videoMaxTime = timeTotal;
    qDebug() << "videoMaxTime = " << m_videoMaxTime;
    m_videoMaxFramCount = m_videoMaxTime * frameRate;
    m_speedSlider->setMaximum(m_videoMaxTime * SLIDER_SMOOTH_NUM);
    m_speedSlider->setMinimum(0);
    m_frameSpeedLable->setText(QString(QString::fromLocal8Bit("原始视频帧率：") + "%1").arg(frameRate));
}

void MainWindow::GetCurrentFramTime(double data)
{
    int dataTemp = data * SLIDER_SMOOTH_NUM;
    m_speedSlider->setValue(dataTemp);
}

void MainWindow::DecodeMenuTriggered(QAction *act)
{
    if(act->isChecked()) {
        QDir::setCurrent(QCoreApplication::applicationDirPath());
        QSettings *config = new QSettings("./config.ini", QSettings::IniFormat);
        if(act->text() == QString::fromLocal8Bit("因特尔解码器")) {
            m_intelDecodeAct->setChecked(true);
            m_nvidiaDecodeAct->setChecked(false);
            m_videoDecodeType = QtFfmpegDecoder::QSV;
            config->setValue("SYS/DecodeType", "intel");
        } else if(act->text() == QString::fromLocal8Bit("英伟达解码器")) {
            m_intelDecodeAct->setChecked(false);
            m_nvidiaDecodeAct->setChecked(true);
            m_videoDecodeType = QtFfmpegDecoder::CUDA;
            config->setValue("SYS/DecodeType", "nvidia");
        }
    }
}

void MainWindow::ZoneNumMenuTriggered(QAction * act)
{
    if(act->isChecked()) {
        m_zoneSize = act->text().toInt();
        for(int i = 1; i < ZONE_NUM; i++) {
            if(i == m_zoneSize) {
                continue;
            } else {
                m_zoneNumAct[i]->setChecked(false);
            }
        }
    } else {
        m_zoneNumAct[1]->setChecked(true);
        m_zoneSize = 1;
    }
    InitZoneSize();
    emit SetDrawZoneSize(m_zoneSize);
    qDebug() << "ZoneNumMenuTriggered" << act->text() << m_zoneSize;
}
void MainWindow::AllZoneClose()
{
    for(int i = 0; i < m_selectedLableList.size(); i++) {
        if(m_selectedLableList.at(i).widget != nullptr) {
            m_selectedLableList.at(i).lable->close();
            m_selectedLableList.at(i).lable->deleteLater();
            m_selectedLableList.at(i).widget->close();
            m_selectedLableList.at(i).widget->deleteLater();
        }
    }
    m_selectedLableList.clear();
}

void MainWindow::InitZoneSize()
{
    AllZoneClose();
    for(int i = 0; i < m_zoneSize; i++) {
        SELECTED_LABLE lable;
        lable.widget = new SelectedWidegt();
        lable.widget->setWindowTitle(QString::number(i));
        connect(lable.widget,SIGNAL(SpaceKeyPress()),
                this,SLOT(LableKeyPressed()));
        lable.lable = new SelectedLable(lable.widget);
        lable.lable->SetId(i);
        connect(lable.widget,SIGNAL(ResizeSig(QResizeEvent*)),
                lable.lable,SLOT(SelectedWidgetSizeChange(QResizeEvent *)));
        connect(lable.lable,SIGNAL(SelectedZoneMove(int, QSize, QPoint, QPoint, bool)),
                m_originalLable,SLOT(SelectedZoneMove(int, QSize, QPoint, QPoint, bool)));
        connect(lable.lable,SIGNAL(SelectedZoneWheel(int, QSize, QPoint, bool)),
                m_originalLable,SLOT(SelectedZoneWheel(int, QSize, QPoint, bool)));
        connect(lable.lable,SIGNAL(SelectedLableHide(int)),
                m_originalLable,SLOT(LableClose(int)));
        connect(lable.lable,SIGNAL(SpaceKeyPress()),
                this,SLOT(LableKeyPressed()));
        connect(m_originalLable,SIGNAL(SendSelectedZone(int, MosaicLable::MOUSE_CHOICE_ZONE)),
                lable.lable,SLOT(GetMouseChoiceZone(int, MosaicLable::MOUSE_CHOICE_ZONE)));
        connect(m_originalLable,SIGNAL(CloseSelectedZone(int)),
                lable.lable,SLOT(CloseThisZone(int)));
        connect(m_ffmpeg, SIGNAL(SendImg(unsigned char *,unsigned int,unsigned int)),
                lable.lable, SLOT(slotShowYuv(unsigned char *,unsigned int,unsigned int)));
//        connect(m_ffmpeg->GetFrameSignalPoint(), SIGNAL(SendImg(unsigned char *,unsigned int,unsigned int)),
//                lable.lable, SLOT(slotShowYuv(unsigned char *,unsigned int,unsigned int)));
        //选区移动至副显示器
        connect(this,SIGNAL(SendAuxRect(QRect)),
                lable.lable,SLOT(SetAuxrect(QRect)));
        //选区置顶
        connect(m_selectedZoneBeTop,SIGNAL(triggered()),lable.widget, SLOT(WidgetTop()));

        QHBoxLayout *selectedLayout = new QHBoxLayout();
        selectedLayout->addWidget(lable.lable);
        selectedLayout->setStretchFactor(lable.lable, 1);
        selectedLayout->setContentsMargins(0, 0, 0, 0);
        lable.widget->setLayout(selectedLayout);
        lable.widget->hide();
        m_selectedLableList.append(lable);
        emit SendAuxRect(Aux_rect);
    }
}


void MainWindow::AddSpeedClicked()
{
    if(m_currentSpeedValue + 10 > MAX_DELAY_SPEED) {
        return;
    }
    m_currentSpeedValue += 10;
    m_speedNumSpinBox->setValue(m_currentSpeedValue);
}

void MainWindow::ReduceSpeedClicked()
{
    double realSpeed = 1000 / (m_videoMaxFramCount / (m_videoMaxTime * 1.0));
    if(m_currentSpeedValue - 10 < MIN_DELAY_SPEED || realSpeed + (m_currentSpeedValue - 10) <= 10) {
        return;
    }
    m_currentSpeedValue -= 10;
    m_speedNumSpinBox->setValue(m_currentSpeedValue);
}

void MainWindow::ResetSpeedClicked()
{
    m_speedNumSpinBox->setValue(0);
}

void MainWindow::SpeedNumSpinBoxValueChanged(int data)
{

    m_currentSpeedValue = data;
    emit SetDecodeSpeedDelay(data);

    qDebug() << "speed = " << data;
}

void MainWindow::CameraConfigConfig()
{
    //m_cameraConfigWidget->setWindowFlag(Qt::WindowStaysOnTopHint);
   // m_cameraConfigWidget->show();
    m_cameraConfigWidget->raise();
}

void MainWindow::ConfigFocusZone(int x, int y, int w, int h)
{
    QString ip;
    if(m_filePath.contains("rtsp")) {
        if(m_filePath.split("://").size() == 2) {
            ip = m_filePath.split("://").at(1).split("/").at(0);
        } else {
            return;
        }
        qDebug() << "ip = " << ip;
        WebApi::Get(ip + QString("/ctrl/af?action=update_roi&x=%0&y=%1&w=%2&h=%3")
                    .arg(x).arg(y).arg(w).arg(h));
    }
    //    WebApi::Get(m_ipStr );
}

void MainWindow::GetWindowPos()
{
    /*获取播放器窗口的坐标（获取图像中心部位的边缘解决由于景深产生的误判）*/
    W_Height=(m_originalLable->height()-80)/3;
    W_Width=m_originalLable->width()/3;
    W_Posx=getWindowX()+W_Width;
    W_Posy=getWindowY()+80+W_Height;
    emit BlurTest();
}

void MainWindow::PicBlurTest()
{
    capture->SendCaptureSignal();
    m_hvOpencv->SignalToOpencv();
}


void MainWindow::AboutActionTriggered()
{
    m_aboutWidget->show();
}

//图片发送
void MainWindow::PictureActionTriggered()
{
    if(!Pic_state){
        Pic_state = true;
        m_time = m_differ;
        qDebug()<<Pic_state<<m_time;
        emit SendPicState();
        //emit GetPosStart(500);
    }
    else{
        //取消发送
        Pic_state = false;
        //emit GetPosStop();
        qDebug()<<"cancel:"<<Pic_state<<m_time;
    }
}


void MainWindow::VolumeSliderMoved(int data)
{
    qDebug() << "volume = " << data;
    emit SetVolume(data / 100.0);
    m_volumeNumber = data;
    SetVolumeConfig(data);
}

void MainWindow::VolumeButtonClicked()
{
    if(m_volumeButton->text() == QString::fromLocal8Bit("音量")) {
        m_volumeButton->setText(QString::fromLocal8Bit("静音"));
        m_volumeSlider->setValue(0);
        SetVolumeZeroConfig(true);
        emit SetVolume(0);

    } else {
        m_volumeButton->setText(QString::fromLocal8Bit("音量"));
        m_volumeSlider->setValue(m_volumeNumber);
        SetVolumeZeroConfig(false);
        emit SetVolume(m_volumeNumber / 100.0);
    }
}

void MainWindow::VolumeSliderVaalueChanged(int data)
{
    m_volumeShow->setText(QString::number(data));
}




