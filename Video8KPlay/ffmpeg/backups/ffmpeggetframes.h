#ifndef FFMPEGGETFRAMES_H
#define FFMPEGGETFRAMES_H

#include <queue>
#include <thread>
#include <memory>
#include <future>
#include <functional>
#include <iostream>
#include <string>

extern "C"
{	// 因为ffmpeg是C库函数, 所以在C++中使用需要加上extern "C"包含头文件防止报错.
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavformat/avio.h"
#include <libavformat/version.h>
#include "libavutil/audio_fifo.h"
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include "libavutil/opt.h"
#include <libavutil/time.h>
#include <libavutil/mathematics.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libavutil/pixfmt.h>
#include <libswresample/swresample.h>
#include <libavdevice/avdevice.h>
}

class FfmpegGetFrames
{
public:
    enum STREAM_TYPE {
        STREAM,
        FILE,
    };
    enum FILE_TYPE {
        MP4,
        M3U8,
    };

    typedef struct PLAY_TIME_SLOT_STRUCT {
        unsigned long begain = 0;
        unsigned long end = 0;
        bool able = false;
        void clear() {
            begain = 0;
            end = 0;
            able = false;
        }
    }PLAY_TIME_SLOT;

    explicit FfmpegGetFrames();
    ~FfmpegGetFrames();
    void RunStart(std::string, int, std::string, std::string, std::string);
    void RunStop();
    void SetFrameTime(long time);
    void SetPlayState(bool state);
    void SetPlayDelayTime(int time);
    void SetPlayTimeSlot(unsigned long begain, unsigned long end);
    void SetPlayTimeUnabled();

public:
    std::function<void(std::string)> _bindTest = nullptr;
    std::function<void(AVFormatContext *, FfmpegGetFrames::STREAM_TYPE)> _bindAVFormatSend = nullptr;
    std::function<void(AVPacket)> _bindAVPacketSend = nullptr;
    std::function<void(AVPacket *)> _bindAVPacketSendPtr = nullptr;
    std::function<void(AVPacket *)> _bindAudioPacketSendPtr = nullptr;

    std::function<void(unsigned long long)> _fileTotalFrameSend = nullptr;
    std::function<void(double)> _frameRateSend = nullptr;
    std::function<void(unsigned long long, double)> _frameMasageSend = nullptr;
    std::function<void(double)> _framePositionSend = nullptr;
    std::function<void()> _getFrameQuit = nullptr;



private:
    void InitThread();
    void InitConfig();
    void FileSendFrame(AVPacket *);
    void VideoSend(AVPacket *);
    void AudioSend(AVPacket *);
protected:
    virtual void GetFrame();


private:
    std::string m_url;
    std::unique_ptr<std::thread> m_getFrameThread = nullptr;
    bool m_running = false;
    STREAM_TYPE m_streamType;
    FILE_TYPE m_fileType;

    AVFormatContext *m_pFormatCtx = nullptr;

    int m_videoindex = -1;
    int m_audioindex = -1;
    //std::unique_ptr<std::thread> m_sendFrameTimer = nullptr;

    double m_frameRate = 0.0;
    unsigned long long m_frameTotal = 0;
    unsigned long long m_timeTotal = 0;
    // = nullptr;

    long m_setFrameTime = -1;
    bool m_rePlayed = false;
    bool m_playState = true;

    int m_palyDelayTime = 0;

    PLAY_TIME_SLOT m_playTimeSlot;
};

#endif // FFMPEGGETFRAMES_H
