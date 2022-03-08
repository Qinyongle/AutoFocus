#ifndef ANDIOPLAY_H
#define ANDIOPLAY_H

#include <QObject>
#include <QAudioOutput>
#include <QTimer>
#include <QThread>
#include <QMutex>
#include <QQueue>
#include <thread>
#include <memory>
#include <future>
//#include "ffmpeg/ffmpegwidgets.h"
class AudioPlay : public QObject
{
    Q_OBJECT
public:
    explicit AudioPlay(QObject *parent = nullptr);
    ~AudioPlay();
    double GetVolume() {
        if(m_audioOutput != nullptr) {
            return m_audioOutput->volume();
        } else {
            return 0.0;
        }
    };

signals:
    void StartDecodeTimer(int);
    void StopDecodeTimer();

public slots:
    void ThreadStart(int, int, int, bool);
    void GetData(QByteArray);
    void RunState(bool);
    void SetVolume(double);

private slots:
    void SendImgEventTimer();

private:
    void InitThread();
    void AudioGetThread(int rate, int count);

private:
    QAudioFormat m_fmt;
    QAudioOutput *m_audioOutput = nullptr;
    QIODevice *m_streamOut = nullptr;

    QByteArray m_andioBuffer;

    QTimer *m_sendImgTimer;
    QThread *m_sendImgTimerThread;


    bool m_runState = false;
    QMutex m_videoDataMutex;
    QQueue<QByteArray> m_videoDataQueue;

    std::unique_ptr<std::thread> m_audioGetThread = nullptr;
};

#endif // ANDIOPLAY_H
