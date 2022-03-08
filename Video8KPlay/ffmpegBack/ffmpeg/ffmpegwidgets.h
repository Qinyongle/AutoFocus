#ifndef FFMPEGWIDGETS_H
#define FFMPEGWIDGETS_H

#include <QWidget>
#include <QObject>
#include <QImage>
#include <QMutex>
extern "C"
{	// 因为ffmpeg是C库函数, 所以在C++中使用需要加上extern "C"包含头文件防止报错.
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include "libavutil/opt.h"
}
class FFmpegWidgets : public QObject
{
    Q_OBJECT
public:
    enum IMAGE_TYPE {
        RGB32,
        NV12,
        YUV,
    };
    explicit FFmpegWidgets(QString name = "", QObject *parent = nullptr);
    ~FFmpegWidgets();
signals:
    void SendImg(QImage);
    void SendImg(QString, QImage);
    void SendImg(unsigned char *,unsigned int,unsigned int);
    void SendImg(QString, unsigned char *,unsigned int,unsigned int);
    void SendFrameData(long long, long long);
    void SendCurrentFramTime(int);
public slots:
    void RunStart(FFmpegWidgets::IMAGE_TYPE);
    void RunStop();
    void SetRtsp(QString, QString, QString);
    void PlayingState(bool);
    void GetVideoFrameInt(long);
private:
    void InitThread();

private:
    bool m_running;
    bool m_playingState = false;
    long m_videoFrameInt = 0;
private:
    QString m_url;
    QString m_user;
    QString m_passwd;

    QString m_name;

    QMutex mutex;

};

#endif // FFMPEGWIDGETS_H

/*
av_seek_frame(AVFormatContext *s, int stream_index, int64_t timestamp,
                  int flags);
*/
