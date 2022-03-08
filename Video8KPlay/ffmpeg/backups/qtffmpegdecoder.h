#ifndef QTFFMPEGDECODER_H
#define QTFFMPEGDECODER_H

#include <QObject>
#include "ffmpeg/ffmpegdecoder.h"
class QtFfmpegDecoder : public QObject, public FfmpegDecoder
{
    Q_OBJECT
public:
    explicit QtFfmpegDecoder(QObject *parent = nullptr);
    ~QtFfmpegDecoder();
    void RunStop();
private:
    void InitThread();

signals:
    void SendImg(unsigned char *,unsigned int,unsigned int);
    void AudioStart(int, int, int, bool);
    void SendAudio(QByteArray);
};

#endif // QTFFMPEGDECODER_H
