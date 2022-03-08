#include "audioplay.h"
#include <QDebug>
AudioPlay::AudioPlay(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<QByteArray>("QByteArray&");
    InitThread();
}

AudioPlay::~AudioPlay()
{
    m_sendImgTimerThread->quit();
    m_sendImgTimerThread->wait();
    RunState(false);
    if(m_audioGetThread) {
        m_runState = false;
        m_audioGetThread->joinable() ? m_audioGetThread->join() : void();
    }
    qDebug() << "~AudioPlay()";
}


void AudioPlay::InitThread()
{
    m_sendImgTimer = new QTimer();
    m_sendImgTimer->setTimerType(Qt::PreciseTimer);
    m_sendImgTimerThread = new QThread();
    m_sendImgTimer->moveToThread(m_sendImgTimerThread);
    connect(m_sendImgTimerThread, &QThread::finished,
                m_sendImgTimer, &QObject::deleteLater);//, Qt::DirectConnection
    connect(m_sendImgTimer, SIGNAL(timeout()), this, SLOT(SendImgEventTimer()));
    connect(this, SIGNAL(StartDecodeTimer(int)), m_sendImgTimer, SLOT(start(int)));
    connect(this, SIGNAL(StopDecodeTimer()), m_sendImgTimer, SLOT(stop()));

    m_sendImgTimerThread->start(QThread::InheritPriority);

    m_fmt.setSampleRate(48000);
    m_fmt.setSampleSize(16);
    m_fmt.setChannelCount(2);
    m_fmt.setCodec("audio/pcm");
    m_fmt.setByteOrder(QAudioFormat::LittleEndian);
    m_fmt.setSampleType(QAudioFormat::SignedInt);
    m_audioOutput = new QAudioOutput(m_fmt);
    m_streamOut = m_audioOutput->start();
}


void AudioPlay::GetData(QByteArray data)
{
//    m_videoDataMutex.lock();
    m_videoDataQueue.enqueue(data);
//    m_videoDataMutex.unlock();

}

void AudioPlay::ThreadStart(int rate, int count, int, bool )
{

    if(m_audioGetThread) {
        m_runState = false;
        m_audioGetThread->joinable() ? m_audioGetThread->join() : void();
        m_audioGetThread.reset((new std::thread([=] (int rate, int count){
            this->AudioGetThread(rate, count);
        }, rate, count)));

    } else {
        m_audioGetThread = std::unique_ptr<std::thread>(new std::thread([=] (int rate, int count){
            this->AudioGetThread(rate, count);
        }, rate, count));
    }
}


void AudioPlay::AudioGetThread(int rate, int count)
{
    m_fmt.setSampleRate(rate);
    m_fmt.setSampleSize(16);
    m_fmt.setChannelCount(count);
    m_fmt.setCodec("audio/pcm");
    m_fmt.setByteOrder(QAudioFormat::LittleEndian);
    m_fmt.setSampleType(QAudioFormat::SignedInt);

    if(m_audioOutput != nullptr) {
        m_audioOutput->deleteLater();
        m_audioOutput = nullptr;
    }

    m_audioOutput = new QAudioOutput(m_fmt);
    m_streamOut = m_audioOutput->start();

    m_runState = true;
    while(m_runState) {
        if(m_videoDataQueue.size() > 1) {
//            qDebug() << "GetData" << m_videoDataQueue.size();
//            m_videoDataMutex.lock();
            QByteArray atemp = m_videoDataQueue.dequeue();
//            m_videoDataMutex.unlock();
            m_andioBuffer.append(atemp);
            while(m_andioBuffer.length() > 4096) {
                int writeBytes = qMin(m_andioBuffer.length(), m_audioOutput->bytesFree());
                m_streamOut->write(m_andioBuffer.data(), writeBytes);
                m_andioBuffer = m_andioBuffer.right(m_andioBuffer.length() - writeBytes);
                    //qDebug() << "writeBytes = " << writeBytes;
            }
        } else {
            //QThread::msleep(1);
        }
    }
}

void AudioPlay::SendImgEventTimer()
{
    if(m_streamOut == nullptr) {
        return;
    }
}

void AudioPlay::RunState(bool state)
{
    m_runState = state;
}

void AudioPlay::SetVolume(double data)
{
    if(m_audioOutput != nullptr) {
        m_audioOutput->setVolume(data);
        qDebug() << "volume data = " << m_audioOutput->volume();
    }

}

