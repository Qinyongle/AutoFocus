#ifndef FFMPEGDECODER_H
#define FFMPEGDECODER_H
#include "ffmpeggetframes.h"
#include <string>
#include <QDebug>
#include <iostream>
#include "threads/ptz/ptz_opencv.h"
#include "imagecapture.h"
class FfmpegDecoder
{

public:
    #define MAX_AUDIO_FRAME_SIZE 192000
    enum DECODE_TYPE {
        QSV,
        CUDA,
    };
    FfmpegDecoder();
    ~FfmpegDecoder();
    void SetAVFormat(AVFormatContext *, FfmpegGetFrames::STREAM_TYPE);
    void GetAVPacket(AVPacket *);
    void SetDecodeState(bool state) {m_decodeState = state;}
    bool GetDecodeState() {return m_decodeState;}
    void SetDecodeType(FfmpegDecoder::DECODE_TYPE type) {m_decodeType = type;}


public:
    std::function<void(double)> _framePositionSend = nullptr;
    std::function<void(unsigned char *,unsigned int,unsigned int)> _sendImg = nullptr;
    std::function<void(std::vector<char>)> _sendAudio = nullptr;
    std::function<void(int, int, int, bool)> _sendAudioConfig = nullptr;
    std::function<void(QString)> _sendPicturePath = nullptr;
    std::function<void()> _sendSignal = nullptr;
    PtzOpencv temp_signal;
private:
    void InitThread();

    void jpg_save(uint8_t *pRGBBuffer, int width, int height);
    void SwsSaveThread(struct SwsContext *c, const uint8_t *const srcSlice[],
                       const int srcStride[], int srcSliceY, int srcSliceH,
                       uint8_t *const dst[], const int dstStride[]);

protected:
    virtual void ThreadRun();
    virtual void ThreadQuit();
    virtual void AsyncAVDecode(unsigned char *buffer,unsigned char *vBuffer, unsigned char *aBuffer,
                                  AVFrame *frame,AVPacket packet);
protected:
    bool m_getPicSwitch = false;
    QString m_PicPath;

private:
    AVFrame *pFrameRGB = NULL;
    struct SwsContext *pSWSCtx = NULL;
    bool m_running = false;
    FfmpegGetFrames::STREAM_TYPE m_streamType;
    DECODE_TYPE m_decodeType;
    int m_videoIndex = -1;
    int m_audioIndex = -1;
    std::unique_ptr<std::thread> m_sendTimerThread = nullptr;
    std::unique_ptr<std::thread> m_decodeThread = nullptr;
//    std::unique_ptr<std::thread> m_decodeThread2 = nullptr;
//    std::unique_ptr<std::thread> m_videoDecodeThread = nullptr;
//    std::unique_ptr<std::thread> m_audioDecodeThread = nullptr;

    AVFormatContext *m_pFormatCtx = nullptr;
    AVCodecContext *m_viCodecCtx = nullptr;
    AVCodecContext *m_auCodecCtx = nullptr;
    AVCodec *m_viCodec = nullptr;
    AVCodec *m_auCodec = nullptr;

    // audio
    SwrContext *m_swrCtx = nullptr;
    int m_audioOutChannels = 0;
    AVSampleFormat m_audioOutSampleFmt;

    std::mutex m_getAVPacketMutex;
//    std::mutex m_getaudioPacketMutex;
    std::queue<AVPacket> m_AVPacketQueue;
//    std::queue<AVPacket> m_audioPacketQueue;

    bool m_decodeState = true;


    double m_timeBase = 0.0;

};

#endif // FFMPEGDECODER_H
