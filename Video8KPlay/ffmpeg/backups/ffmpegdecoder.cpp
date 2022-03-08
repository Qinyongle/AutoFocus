#include "ffmpegdecoder.h"

FfmpegDecoder::FfmpegDecoder()
{
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

void FfmpegDecoder::SetAVFormat(AVFormatContext *formatCtx,
                                FfmpegGetFrames::STREAM_TYPE streamType)
{
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
    m_viCodecCtx->thread_count = 16;
    std::cout << "codecpar->format = " << AVPixelFormat(viCodecpar->format)
              << AV_PIX_FMT_NV12 << std::endl;
    if (avcodec_open2(m_viCodecCtx, m_viCodec, nullptr) < 0) {
        std::cout <<"Could not open codec" << std::endl;
        return ;
    }

    std::cout << "image size = " << m_viCodecCtx->width << m_viCodecCtx->height << std::endl;
    // 音频解码
    uint64_t out_channel_layout;
//    AVSampleFormat out_sample_fmt;
    int outSampleRate = 0;
//    int out_channels = 0;
//    SwrContext *swr_ctx = nullptr;


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
        if(m_audioDecodeThread) {
            m_running = false;
            m_audioDecodeThread->joinable() ? m_audioDecodeThread->join() : void();
            m_audioDecodeThread.reset((new std::thread([this] {
                this->AudioDecodeThread();
            })));
        } else {
            m_audioDecodeThread = std::unique_ptr<std::thread>(new std::thread([this] {
                this->AudioDecodeThread();
            }));
        }
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

    if(m_videoDecodeThread) {
        m_running = false;
        m_videoDecodeThread->joinable() ? m_videoDecodeThread->join() : void();
        m_videoDecodeThread.reset((new std::thread([this] {
            this->VideoDecodeThread();
        })));
    } else {
        m_videoDecodeThread = std::unique_ptr<std::thread>(new std::thread([this] {
            this->VideoDecodeThread();
        }));
    }


}

void FfmpegDecoder::ThreadRun()
{
//    int ret = -1;
//    AVFrame *pFrame = av_frame_alloc();
//    int videoWidth = m_viCodecCtx->width;
//    int videoHeight = m_viCodecCtx->height;
//    unsigned char *videoOutBuffer;
//    videoOutBuffer = (unsigned char *)av_malloc(av_image_get_buffer_size(
//                                                    AV_PIX_FMT_NV12, videoWidth, videoHeight, 1));
//    m_running = true;
//    while(m_running) {
//        m_getvideoPacketMutex.lock();
//        if(m_videoPacketQueue.size() == 1) {
//            std::cout << "m_videoPacketQueue.size() = " << m_videoPacketQueue.size() << std::endl;
//            AVPacket packet = m_videoPacketQueue.front();
//            m_videoPacketQueue.pop();
//            m_getvideoPacketMutex.unlock();
//            if(!m_decodeState) {
//                av_packet_unref(&packet);
//                continue;
//            }

//            //解码一帧视频数据
//            ret = avcodec_send_packet(m_viCodecCtx, &packet);
//            if(ret < 0) {
//                std::cout << "ERROR:avcodec_send_packet" << std::endl;
//                av_packet_unref(&packet);
//                continue;
//            }
//            av_packet_unref(&packet);
//            while(avcodec_receive_frame(m_viCodecCtx, pFrame) == 0) {
//                if(packet.stream_index == m_videoIndex) {
//                    int bytes = 0;
//                    for(int i = 0; i < videoHeight; i++){ //将y分量拷贝
//                        ::memcpy(videoOutBuffer + bytes,pFrame->data[0] + pFrame->linesize[0] * i,videoWidth);
//                        bytes += videoWidth;
//                    }
//                    int uv = videoHeight >> 1;
//                    for(int j = 0; j < uv; j++){ //将uv分量拷贝
//                        ::memcpy(videoOutBuffer + bytes,pFrame->data[1] + pFrame->linesize[1] * j,videoWidth);
//                        bytes += videoWidth;
//                    }
//                    _sendImg ? _sendImg(videoOutBuffer,videoWidth,videoHeight) : void();
//                    break;
//                }
//            }
//        } else {
//            m_getvideoPacketMutex.unlock();
//            std::this_thread::sleep_for(std::chrono::microseconds(1));
//        }

//    }
}

void FfmpegDecoder::VideoDecodeThread()
{
    m_running = true;
    while(m_videoPacketQueue.size() > 0) {
        AVPacket packet = m_videoPacketQueue.front();
        m_videoPacketQueue.pop();
        av_packet_unref(&packet);
    }
    auto pFrame = av_frame_alloc();
    auto videoOutBuffer = (unsigned char *)av_malloc(av_image_get_buffer_size(
                                                        AV_PIX_FMT_NV12, m_viCodecCtx->width, m_viCodecCtx->height, 1));
    while(m_running) {

        if(m_videoPacketQueue.size() > 0) {
            std::cout << "m_videoPacketQueue.size() = " << m_videoPacketQueue.size() << std::endl;
            m_getvideoPacketMutex.lock();
            AVPacket packet = m_videoPacketQueue.front();
            m_videoPacketQueue.pop();
            m_getvideoPacketMutex.unlock();
            if(!m_decodeState) {
                av_packet_unref(&packet);
                continue;
            }
            double packetTimes = packet.pts * m_timeBase;
//            std::cout << "packetTimes = " << packetTimes
//                      << " " << m_videoIndex
//                      << " "<< std::endl;
            _framePositionSend ? _framePositionSend(packetTimes) : void();
            auto sendFrameTimer = std::async(std::launch::async,
                                                          [this](unsigned char *buffer,AVFrame *pFrame, AVPacket packet) {
                                                                this->AsyncVideoDecode(buffer, pFrame, packet);
                                                            }, videoOutBuffer,pFrame, packet);
        } else {
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }
    }
    // 暂时版本不能释放，显示线程正在读取的时候，释放会卡死
//    av_free(videoOutBuffer);
//    videoOutBuffer = nullptr;
//    av_free(pFrame);
//    pFrame = nullptr;
    std::cout << "FfmpegDecoder::VideoDecodeThread quit" << std::endl;
}

void FfmpegDecoder::AudioDecodeThread()
{
    m_running = true;
    auto pFrame = av_frame_alloc();
    auto audioOutBuffer = (unsigned char *)av_malloc(MAX_AUDIO_FRAME_SIZE * 2);
    while(m_audioPacketQueue.size() > 0) {
        AVPacket packet = m_audioPacketQueue.front();
        m_audioPacketQueue.pop();
        av_packet_unref(&packet);
    }

    while(m_running) {
        if(m_audioPacketQueue.size() > 0) {
            //std::cout << "m_audioPacketQueue.size() = " << m_audioPacketQueue.size() << std::endl;
            m_getaudioPacketMutex.lock();
            AVPacket packet = m_audioPacketQueue.front();
            m_audioPacketQueue.pop();
            m_getaudioPacketMutex.unlock();
            if(!m_decodeState) {
                av_packet_unref(&packet);
                continue;
            }
            auto sendFrameTimer = std::async(std::launch::async,
                                          [this](unsigned char *buffer,AVFrame *pFrame, AVPacket packet) {
                                                this->AsyncAudioDecode(buffer, pFrame, packet);
                                            }, audioOutBuffer,pFrame, packet);

        } else {
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }
    }

    std::cout << "FfmpegDecoder::AudioDecodeThread quit" << std::endl;

}

void FfmpegDecoder::ThreadQuit()
{
    m_running = false;
    if(m_decodeThread) {
        m_decodeThread->joinable() ? m_decodeThread->join() : void();
    }

    if(m_videoDecodeThread) {
        m_videoDecodeThread->joinable() ? m_videoDecodeThread->join() : void();
    }

    if(m_audioDecodeThread) {
        m_audioDecodeThread->joinable() ? m_audioDecodeThread->join() : void();
    }
}


void FfmpegDecoder::GetVideoPacket(AVPacket *data)
{
    AVPacket *packet = av_packet_alloc();
    av_packet_ref(packet, data);
    if(data->stream_index == m_videoIndex) {
        m_getvideoPacketMutex.lock();
        m_videoPacketQueue.push(*packet);
        m_getvideoPacketMutex.unlock();
    }
    // 防止文件读取速度过快，解码速度跟不上，导致内存持续增加，对此进行延时
    if(m_streamType == FfmpegGetFrames::FILE) {
        while(m_videoPacketQueue.size() > 30) {
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }
    }
}

void FfmpegDecoder::GetAudioPacket(AVPacket * data)
{
    AVPacket *dataTemp = av_packet_alloc();
    av_packet_ref(dataTemp, data);
    if(data->stream_index == m_audioIndex) {
        m_getaudioPacketMutex.lock();
        m_audioPacketQueue.push(*dataTemp);
        m_getaudioPacketMutex.unlock();
    }
}

void FfmpegDecoder::AsyncVideoDecode(unsigned char *videoOutBuffer,
                                     AVFrame *pFrame, AVPacket packet)
{
    int videoWidth = m_viCodecCtx->width;
    int videoHeight = m_viCodecCtx->height;
    if(!m_decodeState) {
        return;
    }
    //解码一帧视频数据
    int ret = -1;
    ret = avcodec_send_packet(m_viCodecCtx, &packet);
    av_packet_unref(&packet);
    if(ret < 0) {
        std::cout << "[ERROR]: video avcodec_send_packet" << std::endl;
        return;
    }
    while(avcodec_receive_frame(m_viCodecCtx, pFrame) == 0) {
        if(packet.stream_index == m_videoIndex) {
            int bytes = 0;
            for(int i = 0; i < videoHeight; i++) { //将y分量拷贝
                ::memcpy(videoOutBuffer + bytes,pFrame->data[0] + pFrame->linesize[0] * i,videoWidth);
                bytes += videoWidth;
            }
            int uv = videoHeight >> 1;
            for(int j = 0; j < uv; j++){ //将uv分量拷贝
                ::memcpy(videoOutBuffer + bytes,pFrame->data[1] + pFrame->linesize[1] * j,videoWidth);
                bytes += videoWidth;
            }
            _sendImg ? _sendImg(videoOutBuffer,videoWidth,videoHeight) : void();
            break;
        }
    }
}

void FfmpegDecoder::AsyncAudioDecode(unsigned char *audioOutBuffer,
                              AVFrame *pFrame, AVPacket packet)
{
    if(!m_decodeState) {
        return;
    }
    if(packet.stream_index != m_audioIndex) {
        av_packet_unref(&packet);
        return;
    }
    int ret = -1;
    ret = avcodec_send_packet(m_auCodecCtx, &packet);
    av_packet_unref(&packet);
    if(ret < 0) {
        std::cout << "[ERROR]:audio avcodec_send_packet" << std::endl;
        return;
    }
    while(avcodec_receive_frame(m_auCodecCtx, pFrame) == 0) {
        int len = swr_convert(m_swrCtx, &audioOutBuffer, MAX_AUDIO_FRAME_SIZE, (const uint8_t **)pFrame->data, pFrame->nb_samples);
        if (len <= 0) {
            std::cout << "swr_convert Error" << len << std::endl;
            continue;
        }
        auto dstBufsize = av_samples_get_buffer_size(0,
                          m_audioOutChannels, len, m_audioOutSampleFmt, 1);
        std::vector<char> atemp((char *)audioOutBuffer,
                          ((char *)audioOutBuffer) + dstBufsize);
        _sendAudio ? _sendAudio(atemp) : void();
    }
}


