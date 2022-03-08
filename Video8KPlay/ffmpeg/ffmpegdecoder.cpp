#include "ffmpegdecoder.h"
#include <QImage>
#include <QDir>
#include <QCoreApplication>

FfmpegDecoder::FfmpegDecoder()
{
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    InitThread();
}

FfmpegDecoder::~FfmpegDecoder()
{
    ThreadQuit();
    std::cout << "~FfmpegDecoder" << std::endl;

}

void FfmpegDecoder::InitThread()
{


}

void FfmpegDecoder::jpg_save(uint8_t *pRGBBuffer, int width, int height)
{

//    QImage image(pRGBBuffer, width, height, QImage::Format_RGB888);
//    image.save("./123.bmp", "bmp", 100);
//    m_PicPath = "./123.bmp";
    //_sendPicturePath ? _sendPicturePath(m_PicPath) : void();

}

void FfmpegDecoder::SwsSaveThread(SwsContext *c, const uint8_t * const srcSlice[], const int srcStride[], int srcSliceY, int srcSliceH, uint8_t * const dst[], const int dstStride[])
{

    sws_scale(c, srcSlice, srcStride, srcSliceY, srcSliceH, dst, dstStride);
    jpg_save(dst[0], m_viCodecCtx->width,m_viCodecCtx->height);
    //
    //_sendSignal ? _sendSignal() : void();

}

void FfmpegDecoder::SetAVFormat(AVFormatContext *formatCtx,
                                FfmpegGetFrames::STREAM_TYPE streamType)
{
//    qDebug()<<"SetAVFormat............";
    m_running = false;
    m_pFormatCtx = formatCtx;
    m_streamType = streamType;
    m_videoIndex = -1;
    m_audioIndex = -1;
    for (unsigned int i = 0; i < m_pFormatCtx->nb_streams; i++) {
        if (m_pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO){
            m_videoIndex = i;
        }
        if(m_pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            m_audioIndex = i;
        }
    }
    if (m_videoIndex == -1) {
        std::cout <<"Didn't find a video stream.\n" << std::endl;
        return ;
    }
    if(m_audioIndex == -1) {
        std::cout << "Didn't find a andio stream.\n" << std::endl;
    }

    m_timeBase = av_q2d(m_pFormatCtx->streams[m_videoIndex]->time_base);
    std::cout << "time base = " << m_timeBase << std::endl;

    // 获取视频流编码结构
    AVCodecParameters* viCodecpar = m_pFormatCtx->streams[m_videoIndex]->codecpar;
    std::cout << "code id = " << viCodecpar->codec_id << std::endl;
    // 硬件解码
    //查找解码器
    // cuvid 解码不支持8k H254,8K只能H265,4k以及4K以下，可以使用H264
    if(viCodecpar->codec_id == AV_CODEC_ID_H264) {
        if(m_decodeType == CUDA) {
            m_viCodec = avcodec_find_decoder_by_name("h264_cuvid");
        } else {
            m_viCodec = avcodec_find_decoder_by_name("h264_qsv");
        }
    } else if(viCodecpar->codec_id == AV_CODEC_ID_HEVC) {
        if(m_decodeType == CUDA) {
            m_viCodec = avcodec_find_decoder_by_name("hevc_cuvid");
        } else {
            m_viCodec = avcodec_find_decoder_by_name("hevc_qsv");
        }
    }

    if(!m_viCodec) {
        std::cout << "not find video decoder" << std::endl;
        return;
    }
    // 使用从AVFormatContext得到的CodecContext
    m_viCodecCtx = avcodec_alloc_context3(m_viCodec);
    viCodecpar->format = AV_PIX_FMT_NV12;
    avcodec_parameters_to_context(m_viCodecCtx, viCodecpar);
    m_viCodecCtx->thread_count = 16; //设置多线程数--核心数*2
    std::cout << "codecpar->format = " << AVPixelFormat(viCodecpar->format) << " "
              << AV_PIX_FMT_NV12 << std::endl;
    if (avcodec_open2(m_viCodecCtx, m_viCodec, nullptr) < 0) {
        std::cout <<"Could not open codec" << std::endl;
        return ;
    }

   // std::cout << "image size = " << m_viCodecCtx->width << m_viCodecCtx->height << std::endl;
    // 音频解码
    uint64_t out_channel_layout;
    int outSampleRate = 0;


    AVCodecParameters* auCodecpar;
    if(m_audioIndex != -1) {
        auCodecpar = m_pFormatCtx->streams[m_audioIndex]->codecpar;
        m_auCodec = avcodec_find_decoder(auCodecpar->codec_id);
        if(m_auCodec == 0) {
            std::cout << "can't find audio codec" << std::endl;
            return;
        }
        m_auCodecCtx = avcodec_alloc_context3(m_auCodec);
        avcodec_parameters_to_context(m_auCodecCtx, auCodecpar);
        if (avcodec_open2(m_auCodecCtx, m_auCodec, 0) < 0) {
            std::cout << "audio decode open failed" << std::endl;
            return;
        }
        out_channel_layout = m_auCodecCtx->channel_layout;
        m_audioOutSampleFmt = AV_SAMPLE_FMT_S16;
        outSampleRate = m_auCodecCtx->sample_rate;
        m_audioOutChannels = av_get_channel_layout_nb_channels(out_channel_layout);
        std::cout << "out_sample_rate = " << outSampleRate
                 << " " << m_audioOutChannels
                 << " " << auCodecpar->bits_per_coded_sample << std::endl;
        m_swrCtx = swr_alloc_set_opts(nullptr, out_channel_layout,
                                                 m_audioOutSampleFmt,outSampleRate,
                                                 m_auCodecCtx->channel_layout,
                                                 m_auCodecCtx->sample_fmt,
                                                 m_auCodecCtx->sample_rate, 0, 0);
        swr_init(m_swrCtx);
        auto frameRate = m_pFormatCtx->streams[m_audioIndex]->avg_frame_rate.num /
                (m_pFormatCtx->streams[m_audioIndex]->avg_frame_rate.den * 1.0);
        _sendAudioConfig ? _sendAudioConfig(outSampleRate,
                                            m_audioOutChannels,
                                            frameRate, true) : void();
    }
    if(m_decodeThread) {
        m_running = false;
        m_decodeThread->joinable() ? m_decodeThread->join() : void();
        m_decodeThread.reset((new std::thread([this] {
            this->ThreadRun();
        })));
    } else {
        m_decodeThread = std::unique_ptr<std::thread>(new std::thread([this] {
            this->ThreadRun();
        }));
    }
}

void FfmpegDecoder::ThreadRun()
{
    m_getAVPacketMutex.lock();
    while(m_AVPacketQueue.size() > 0) {
        AVPacket packet = m_AVPacketQueue.front();
        m_AVPacketQueue.pop();
        av_packet_unref(&packet);
    }
    m_getAVPacketMutex.unlock();
    auto frame = av_frame_alloc();
    auto vBuffer = (unsigned char *)av_malloc(av_image_get_buffer_size(
                            AV_PIX_FMT_NV12, m_viCodecCtx->width, m_viCodecCtx->height, 1));
    auto aBuffer = (unsigned char *)av_malloc(MAX_AUDIO_FRAME_SIZE * 2);
    std::cout << "FfmpegDecoder::ThreadRun" << m_AVPacketQueue.size()
              << "m_viCodecCtx->width = " << m_viCodecCtx->width
              << "m_viCodecCtx->height = " << m_viCodecCtx->height
              << std::endl;
    auto videoWidth = m_viCodecCtx->width;
    auto videoHeight = m_viCodecCtx->height;
    auto pFrame = av_frame_alloc();
    auto videoOutBuffer = (unsigned char *)av_malloc(av_image_get_buffer_size(
                                                        AV_PIX_FMT_NV12, m_viCodecCtx->width, m_viCodecCtx->height, 1));
    pSWSCtx = sws_getContext(videoWidth, videoHeight, m_viCodecCtx->pix_fmt,
                                     videoWidth, videoHeight, AV_PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL);
    pFrameRGB = av_frame_alloc();
    int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24, videoWidth, videoHeight, 1);
    uint8_t *buffer = (uint8_t *)av_malloc(numBytes);
    av_image_fill_arrays(pFrameRGB->data, pFrameRGB->linesize, buffer,
                         AV_PIX_FMT_RGB24, videoWidth, videoHeight, 1);
    m_running = true;
    while(m_running) {
        if(m_AVPacketQueue.size() > 0) {
            //std::cout << "m_videoPacketQueue.size() = " << m_AVPacketQueue.size() << std::endl;
            m_getAVPacketMutex.lock();
            AVPacket packet = m_AVPacketQueue.front();
            m_AVPacketQueue.pop();
            m_getAVPacketMutex.unlock();
            if(!m_decodeState) {
                av_packet_unref(&packet);
                continue;
            }

            if(packet.stream_index == m_videoIndex) {
                double packetTimes = packet.pts * m_timeBase;
                _framePositionSend ? _framePositionSend(packetTimes) : void();
            }

            auto sendFrameTimer = std::async(std::launch::async,
                                                          [this](unsigned char *buffer,unsigned char *vBuffer, unsigned char *aBuffer,
                                             AVFrame *frame,AVPacket packet) {
                                                                this->AsyncAVDecode(buffer,vBuffer, aBuffer, frame, packet);
                                                            }, buffer,vBuffer,aBuffer, frame, packet);
        } else {
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }
    }
    av_frame_free(&frame);
    // 暂时版本不能释放，显示线程正在读取的时候，释放会卡死
//    av_free(vBuffer);
//    av_free(aBuffer);


    std::cout << "FfmpegDecoder::VideoDecodeThread quit" << std::endl;
}


void FfmpegDecoder::AsyncAVDecode(unsigned char *buffer, unsigned char *vBuffer, unsigned char *aBuffer,
                              AVFrame *frame, AVPacket packet)
{
    int videoWidth = m_viCodecCtx->width;
    int videoHeight = m_viCodecCtx->height;
    if(!m_decodeState) {
        return;
    }
    //解码一帧视频数据
    int ret = -1;
    if(packet.stream_index == m_videoIndex) {
        ret = avcodec_send_packet(m_viCodecCtx, &packet);
        av_packet_unref(&packet);
        if(ret < 0) {
            std::cout << "[ERROR]: video avcodec_send_packet" << std::endl;
            return;
        }
        if(m_getPicSwitch){
            while(avcodec_receive_frame(m_viCodecCtx, frame) == 0) {
                int bytes = 0;
                for(int i = 0; i < videoHeight; i++) { //将y分量拷贝
                    ::memcpy(buffer + bytes,frame->data[0] + frame->linesize[0] * i,videoWidth);
                    bytes += videoWidth;
                }
                int uv = videoHeight >> 1;
                for(int j = 0; j < uv; j++){ //将uv分量拷贝
                    ::memcpy(buffer + bytes,frame->data[1] + frame->linesize[1] * j,videoWidth);
                    bytes += videoWidth;
                }
                auto sendFrameTimer = std::async(std::launch::async,
                                                 [this](struct SwsContext *c, const uint8_t *const srcSlice[],
                                                 const int srcStride[], int srcSliceY, int srcSliceH,
                                                 uint8_t *const dst[], const int dstStride[]) {
                    this->SwsSaveThread(c, srcSlice, srcStride, srcSliceY, srcSliceH, dst,
                                        dstStride);},
                pSWSCtx, frame->data, frame->linesize, 0,
                m_viCodecCtx->height, pFrameRGB->data, pFrameRGB->linesize);
                m_getPicSwitch = false;
                break;
            }
        }
        while(avcodec_receive_frame(m_viCodecCtx, frame) == 0) {

                int bytes = 0;
                for(int i = 0; i < videoHeight; i++) { //将y分量拷贝
                    ::memcpy(vBuffer + bytes,frame->data[0] + frame->linesize[0] * i,videoWidth);
                    bytes += videoWidth;
                }
                int uv = videoHeight >> 1;
                for(int j = 0; j < uv; j++){ //将uv分量拷贝
                    ::memcpy(vBuffer + bytes,frame->data[1] + frame->linesize[1] * j,videoWidth);
                    bytes += videoWidth;
                }
            _sendImg ? _sendImg(vBuffer,videoWidth,videoHeight) : void();
        }
    } else if(packet.stream_index == m_audioIndex) {
        ret = avcodec_send_packet(m_auCodecCtx, &packet);
        av_packet_unref(&packet);
        if(ret < 0) {
            std::cout << "[ERROR]: andio avcodec_send_packet" << std::endl;
            return;
        }
        while(avcodec_receive_frame(m_auCodecCtx, frame) == 0) {
            int len = swr_convert(m_swrCtx, &aBuffer, MAX_AUDIO_FRAME_SIZE, (const uint8_t **)frame->data, frame->nb_samples);
            if (len <= 0) {
                std::cout << "swr_convert Error" << len << std::endl;
                continue;
            }
            auto dstBufsize = av_samples_get_buffer_size(0,
                              m_audioOutChannels, len, m_audioOutSampleFmt, 1);
            std::vector<char> atemp((char *)aBuffer,
                              ((char *)aBuffer) + dstBufsize);
            _sendAudio ? _sendAudio(atemp) : void();
            //std::cout << "_sendAudio" << std::endl;
        }
    }
}





void FfmpegDecoder::ThreadQuit()
{
    m_running = false;
    if(m_decodeThread) {
        m_decodeThread->joinable() ? m_decodeThread->join() : void();
    }
    while(m_AVPacketQueue.size() > 0) {
        AVPacket packet = m_AVPacketQueue.front();
        m_AVPacketQueue.pop();
        av_packet_unref(&packet);
    }
}


void FfmpegDecoder::GetAVPacket(AVPacket *data)
{
    if(!m_running) {
        return;
    }
    AVPacket *packet = av_packet_alloc();
    av_packet_ref(packet, data);
    m_getAVPacketMutex.lock();
    m_AVPacketQueue.push(*packet);
    m_getAVPacketMutex.unlock();
    // 防止文件读取速度过快，解码速度跟不上，导致内存持续增加，对此进行延时
    if(m_streamType == FfmpegGetFrames::FILE) {
        while(m_AVPacketQueue.size() > 30) {
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }
    }
}





