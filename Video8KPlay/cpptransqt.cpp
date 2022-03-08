#include "cpptransqt.h"
#include "QDebug"
CppTransQt::CppTransQt(QObject *parent) : QObject(parent) ,FfmpegDecoder()
{
    InitThread();
}

CppTransQt::~CppTransQt()
{

}

void CppTransQt::InitThread()
{
    _sendImg = [this](unsigned char * data,unsigned int width,unsigned int height) {
        emit this->SendImg(data, width, height);
    };
//    _sendAudio = [this](std::vector<char> data) {
//        emit this->SendAudio(QByteArray(data.data(), data.size()));
//    };
    _sendPicturePath = [=] (QString path) {

        emit this->SendPicPath(path);
    };

    _sendSignal = [=] () {
        emit this->SendToOpencv();
    };
}

void CppTransQt::GetPicCmd()
{
    m_getPicSwitch = true;
}

void CppTransQt::RunStop()
{
    this->ThreadQuit();
}


