#ifndef CPPTRANSQT_H
#define CPPTRANSQT_H

#include <QObject>
#include "ffmpeg/ffmpegdecoder.h"
class CppTransQt : public QObject , public FfmpegDecoder
{
    Q_OBJECT
public:
    explicit CppTransQt(QObject *parent = nullptr);
    ~CppTransQt();
    void RunStop();

private:
    void InitThread();

public slots:
    // pic
    void GetPicCmd();

signals:
    void SendImg(unsigned char *,unsigned int,unsigned int);
//    void AudioStart(int, int, int, bool);
//    void SendAudio(QByteArray);
    void SendPicPath(QString);
    void SendToOpencv();
};

#endif // CPPTRANSQT_H
