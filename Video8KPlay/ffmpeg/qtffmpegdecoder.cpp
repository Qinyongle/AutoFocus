#include "qtffmpegdecoder.h"
#include <QDebug>
QtFfmpegDecoder::QtFfmpegDecoder(QObject *parent) : QObject(parent)
{
    InitThread();
}

QtFfmpegDecoder::~QtFfmpegDecoder()
{

}

void QtFfmpegDecoder::InitThread()
{
    _sendImg = [this](unsigned char * data,unsigned int width,unsigned int height) {
        emit this->SendImg(data, width, height);
    };

    _sendAudio = [this](std::vector<char> data) {
        emit this->SendAudio(QByteArray(data.data(), data.size()));
    };

    _sendAudioConfig = [=] (int rate, int count, int frameRate, bool state) {
        emit this->AudioStart(rate, count, frameRate, state);
    };
    _sendPicturePath = [=] (QString path) {
        emit this->SendPicPath(path);
    };
}

void QtFfmpegDecoder::GetPicCmd()
{
    m_getPicSwitch = true;
}

void QtFfmpegDecoder::RunStop()
{
    this->ThreadQuit();
}
