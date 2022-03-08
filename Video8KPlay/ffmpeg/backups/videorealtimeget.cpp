#include "videorealtimeget.h"
#include <QDebug>
VideoRealTimeGet::VideoRealTimeGet(QWidget *p, QObject *parent) : QObject(parent)
{
    qRegisterMetaType<QtFfmpegDecoder::DECODE_TYPE>("QtFfmpegDecoder::DECODE_TYPE");
    m_playBackPlayLable = p;
    InitThread();
}

VideoRealTimeGet::~VideoRealTimeGet()
{
    RunStop();
    m_qtFfmpegDecoder ? m_qtFfmpegDecoder->deleteLater() : void();
    qDebug() << "~PlayBackPlayer()";
}


void VideoRealTimeGet::InitThread()
{
    m_getFrames = std::unique_ptr<FfmpegGetFrames>(new FfmpegGetFrames);

    m_qtFfmpegDecoder = new QtFfmpegDecoder;
    connect(m_qtFfmpegDecoder, SIGNAL(SendImg(unsigned char *,unsigned int,unsigned int)),
            m_playBackPlayLable, SLOT(slotShowYuv(unsigned char *,unsigned int,unsigned int)));

    m_getFrames->_bindAVFormatSend = [this] (AVFormatContext *iFormat, FfmpegGetFrames::STREAM_TYPE data) {
        m_playBackPlayLable ? m_qtFfmpegDecoder->SetAVFormat(iFormat, data) : void();
    };

    m_getFrames->_bindAVPacketSendPtr = [this] (AVPacket *data) {
        m_playBackPlayLable ? m_qtFfmpegDecoder->GetVideoPacket(data) : void();
    };

    m_getFrames->_bindAudioPacketSendPtr = [=] (AVPacket *data) {
        m_playBackPlayLable ? m_qtFfmpegDecoder->GetAudioPacket(data) : void();
    };

    m_qtFfmpegDecoder->_framePositionSend = [this](double data) {
        emit FramePositionSend(data);
    };

    m_getFrames->_frameMasageSend = [this](unsigned long long total, double rate) {
        emit FrameMasageSend(total, rate);
    };
}

void VideoRealTimeGet::RunStart(QString url, QtFfmpegDecoder::DECODE_TYPE type)
{
    m_qtFfmpegDecoder->SetDecodeType(type);
    m_getFrames->RunStart("", 0, "", "", url.toLocal8Bit().toStdString());
}

void VideoRealTimeGet::RunStop()
{
    m_getFrames ? m_getFrames->RunStop() : void();
    m_qtFfmpegDecoder ? m_qtFfmpegDecoder->RunStop() : void();
}

void VideoRealTimeGet::SetFrameTime(long time)
{
    m_getFrames ? m_getFrames->SetFrameTime(time) : void();
}

void VideoRealTimeGet::SetPlayState(bool state)
{
    m_getFrames ? m_getFrames->SetPlayState(state) : void();
}

void VideoRealTimeGet::SetPlayDelayTime(int time)
{
    m_getFrames ? m_getFrames->SetPlayDelayTime(time) : void();
}

void VideoRealTimeGet::SetPlaySlot(unsigned long begain, unsigned long end)
{
    m_getFrames ? m_getFrames->SetPlayTimeSlot(begain, end) : void();
}
