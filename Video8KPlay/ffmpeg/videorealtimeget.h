#ifndef VIDEOREALTIMEGET_H
#define VIDEOREALTIMEGET_H

#include <QObject>
#include <QWidget>
#include "ffmpeg/ffmpeggetframes.h"
#include "ffmpeg/ffmpegsavevideos.h"
#include "ffmpeg/ffmpegdecoder.h"
#include "ffmpeg/qtffmpegdecoder.h"
#include <thread>
#include <memory>
#include <QThread>
#include "cpptransqt.h"
class VideoRealTimeGet : public QObject
{
    Q_OBJECT
public:
#define MAX_DELAY_DOUBLE 20
    explicit VideoRealTimeGet(QWidget *p, QObject *parent = nullptr);
    ~VideoRealTimeGet();
    QtFfmpegDecoder *GetFrameSignalPoint() {return m_qtFfmpegDecoder;}

signals:
    void FramePositionSend(double);
    void FrameMasageSend(unsigned long long, double);

public slots:
    void RunStart(QString, QtFfmpegDecoder::DECODE_TYPE);
    void RunStop();
    void SetFrameTime(long);
    void SetPlayState(bool);
    void SetPlayDelayTime(int);
    void SetPlaySlot(unsigned long, unsigned long);
    // pic
    void GetPicCmd();

    //save_video
    void SetSavePath(QString path, QString name);

private:
    void InitThread();

private:
    std::unique_ptr<FfmpegGetFrames> m_getFrames = nullptr;
    QtFfmpegDecoder *m_qtFfmpegDecoder = nullptr;
    QWidget *m_playBackPlayLable = nullptr;

//    std::unique_ptr<FfmpegSaveVideos> m_saveVideos = nullptr;

    QString m_savePath;
    QString m_saveName;


};

#endif // VIDEOREALTIMEGET_H
