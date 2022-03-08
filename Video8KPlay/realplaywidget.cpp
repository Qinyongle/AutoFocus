#include "realplaywidget.h"

RealPlayWidget::RealPlayWidget(QWidget *p, QObject *parent) : QObject(parent)
{
    m_homePagePlayLable = p;
    InitThread();
}

RealPlayWidget::~RealPlayWidget()
{
    RunStop();
    m_cppTransQtThread->quit();
    m_cppTransQtThread->wait();
}

void RealPlayWidget::RunStart(QString url)
{
    m_url = url;
    auto *folder = new QDir;

    if(!folder->exists(m_savePath + m_saveName.toUtf8())) {
//            qDebug()<<"保存路径不存在,进行创建:"<<path +i.config.name.toUtf8();
        folder->mkdir(m_savePath + m_saveName.toUtf8());
    }
    auto currentDateTime = QDateTime::currentDateTime();
    auto pathTemp = m_savePath + m_saveName.toUtf8() + "/" +
            currentDateTime.toString("yyyy-MM-dd-hh-mm-ss");


    if(!folder->exists(pathTemp)) {
//            qDebug()<<"追加时间保存路径不存在,进行创建:"<<pathTemp;
        folder->mkdir(pathTemp);
    }
    pathTemp += "/";
//        std::cout << "yyyyyyyy" << pathTemp.toLocal8Bit().toStdString()
//                  << "kkkkkkkk" << i.config.name.toLocal8Bit().toStdString()
//                  << std::endl;
//    m_savePath = pathTemp;
    QString _save_path = pathTemp;
//    QString _save_name = pathTemp + m_saveName;
    m_saveVideos->SetFilePath(_save_path.toLocal8Bit().toStdString(),
                              m_saveName.toLocal8Bit().toStdString());
    m_getFrames->RunStart("", 0, "", "", url.toLocal8Bit().toStdString());
}

void RealPlayWidget::RunStop()
{
    m_getFrames ? m_getFrames->RunStop() : void();
    m_saveVideos ? m_saveVideos->RunStop() : void();

    m_cppTransQt ? m_cppTransQt->RunStop() : void();
}

void RealPlayWidget::GetPicCmd()
{
    m_cppTransQt->GetPicCmd();
}

void RealPlayWidget::SwtichStream(QString _url)
{
       RunStop();
       m_url = _url;
       QThread::sleep(2);
       RunStart(_url);
}

void RealPlayWidget::InitThread()
{
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    QSettings *config = new QSettings("./config.ini", QSettings::IniFormat);
    m_decodeType = config->value("SYS/DecodeType").toString();
    m_savePath = config->value("SYS/SavePath").toString();
    m_saveName = config->value("SYS/VideoName").toString();



    m_getFrames = std::unique_ptr<FfmpegGetFrames>(new FfmpegGetFrames);
    m_saveVideos = std::unique_ptr<FfmpegSaveVideos>(new FfmpegSaveVideos);

    if(m_homePagePlayLable) {
        m_cppTransQt = new CppTransQt;
        m_cppTransQtThread = new QThread();
        m_cppTransQt->moveToThread(m_cppTransQtThread);
        connect(m_cppTransQtThread, &QThread::finished, m_cppTransQt, &QObject::deleteLater);
        connect(m_cppTransQt, SIGNAL(SendImg(unsigned char *,unsigned int,unsigned int)),
                m_homePagePlayLable, SLOT(slotShowYuv(unsigned char *,unsigned int,unsigned int)));
        connect(m_cppTransQt, SIGNAL(SendPicPath(QString)),
                this, SIGNAL(SendPicPath(QString)));
        connect(m_cppTransQt, SIGNAL(SendImg(unsigned char *,unsigned int,unsigned int)),
                this, SIGNAL(SendImg(unsigned char *,unsigned int,unsigned int)));


        m_cppTransQtThread->start();
    }
    m_getFrames->_bindAVFormatSend = [this] (AVFormatContext *iFormat, FfmpegGetFrames::STREAM_TYPE data) {
        if(resavestate){
            m_saveVideos->SetReSaveSate(resavestate);
            resavestate = false;
        }
        m_saveVideos->SetAVFormat(iFormat, data);
        if(m_decodeType == "intel") {
            m_cppTransQt->SetDecodeType(CppTransQt::QSV);
        } else {
            m_cppTransQt->SetDecodeType(CppTransQt::CUDA);
        }
        m_homePagePlayLable ? m_cppTransQt->SetAVFormat(iFormat, data) : void();
    };

    m_getFrames->_bindAVPacketSendPtr = [this] (AVPacket *data) {
        m_homePagePlayLable ? m_cppTransQt->GetAVPacket(data) : void();
        m_saveVideos->GetAVPacket(data);
    };

    m_getFrames->_bindAVPacketSendPtr = [this] (AVPacket *data) {
        m_homePagePlayLable ? m_cppTransQt->GetAVPacket(data) : void();
        m_saveVideos->GetAVPacket(data);
    };

    m_getFrames->_replaystate = [this] (bool state) {
        if(state){
            resavestate = state;
//            RunStop();
            m_getFrames->RunReStop();
            m_saveVideos ? m_saveVideos->RunStop() : void();

            m_cppTransQt ? m_cppTransQt->RunStop() : void();
            qDebug()<<"restart:"<<m_url;

            RunStart(m_url);
        }
    };
    //发送总时间--读取文件视频是需要
//    m_getFrames->_frameMasageSend = [this](unsigned long long total, double rate) {
//        emit FrameMasageSend(total, rate);
//    };
}

