#include "ffmpeggetframes.h"
#include <fstream>

FfmpegGetFrames::FfmpegGetFrames()
{

}


FfmpegGetFrames::~FfmpegGetFrames()
{
    RunStop();
    std::cout << "~FfmpegGetFrames" << std::endl;
}

void FfmpegGetFrames::InitThread()
{

}

void FfmpegGetFrames::InitConfig()
{

}


void FfmpegGetFrames::GetFrame()
{

    AVPacket *packet;
    int openTimes = 5;
    int timeTotal = 0;
    m_setFrameTime = -1;

    std::cout << "ffmpeg url = " << m_url << std::endl;

    //采用tcp传输
    AVDictionary *optionsDict = nullptr;
    av_dict_set(&optionsDict, "rtsp_transport", "tcp", 0);
    av_dict_set(&optionsDict, "stimeout", "500000", 0);//5000000
OPEN_AGAIN:
    m_running = true;
    timeTotal = 0;
    //初始化编解码库
    avformat_network_init();
    m_pFormatCtx = avformat_alloc_context();
    m_pFormatCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER | AV_CODEC_FLAG_LOW_DELAY;

    //初始化pFormatCtx结构
    if (avformat_open_input(&m_pFormatCtx, m_url.data(), nullptr, &optionsDict) != 0){
        qDebug()<< "Couldn't open input stream" <<m_url.data();

        if(/*openTimes-- && */m_running) {
            QThread::msleep(500);
            qDebug()<<"this is running..Reconnecting....";
            goto OPEN_AGAIN;
        }
    }



    //获取音视频流数据信息
    if (avformat_find_stream_info(m_pFormatCtx, nullptr) < 0) {
        qDebug()<< "Couldn't find stream information";
        if(/*openTimes-- && */m_running) {
            QThread::msleep(500);
            qDebug()<<"this is running..Reconnecting....";
            goto OPEN_AGAIN;
        }
    }

    packet = av_packet_alloc();




    if(_bindAVFormatSend != nullptr) {
//        AVFormatContext formatCtx = *m_pFormatCtx;
        if(m_setFrameTime < 0) {
            _bindAVFormatSend(m_pFormatCtx, m_streamType);
        }
    }
    m_audioindex = -1;
    m_videoindex = -1;
    for (unsigned int i = 0; i < m_pFormatCtx->nb_streams; i++) {
        if (m_pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO){
            m_videoindex = i;
        } else if(m_pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            m_audioindex = i;
        }
    }
    m_frameRate = m_pFormatCtx->streams[m_videoindex]->avg_frame_rate.num /
                    (m_pFormatCtx->streams[m_videoindex]->avg_frame_rate.den * 1.0);


    //帧率
    qDebug()<<"frameRate:"<<m_frameRate;
    _frameRateSend ? _frameRateSend(m_frameRate) : void();

    if(m_audioindex > 0) {
        m_audioFrameRate = m_pFormatCtx->streams[m_audioindex]->avg_frame_rate.num /
                (m_pFormatCtx->streams[m_audioindex]->avg_frame_rate.den * 1.0);
    }
    if(m_streamType == FILE) {
        std::cout << "file rate =========== " << m_frameRate
                  << m_pFormatCtx->streams[m_videoindex]->avg_frame_rate.num
                  << " " << m_pFormatCtx->streams[m_videoindex]->avg_frame_rate.den
                  << " " << m_pFormatCtx->duration
                  << std::endl;
        if(m_url.find(".m3u8") != std::string::npos) {
            m_fileType = M3U8;
            char ch[512] = {""};
            std::ifstream file(m_url);
            std::vector<std::string> strVector;
            while(file.getline(ch, sizeof(ch))) {
                std::string str = ch;
                str.find("#EXTINF") != std::string::npos ? strVector.push_back(str) : void();
            }
            file.close();
            for(auto i : strVector) {
                auto mPos = i.find(":");
                auto dPos = i.find(",");
                if(mPos + 1 < dPos) {
                    timeTotal += std::stoll(i.substr(mPos + 1, dPos - mPos));
                }
            }
            _fileTotalFrameSend ? _fileTotalFrameSend(timeTotal) : void();

            std::cout << "m_url = " << m_url << std::endl;
            std::cout << "file total frame = "
                      << timeTotal << " " <<  m_frameRate << std::endl;
            _frameMasageSend ? _frameMasageSend(timeTotal, m_frameRate) : void();
        } else {
            m_fileType = MP4;
            timeTotal = m_pFormatCtx->duration / 1000000;
            _fileTotalFrameSend ? _fileTotalFrameSend(m_frameTotal) : void();
            std::cout << "m_url = " << m_url << std::endl;
            std::cout << "file total frame = "
                      << m_frameTotal << " " <<  m_frameRate << std::endl;
            _frameMasageSend ? _frameMasageSend(timeTotal, m_frameRate) : void();
        }
//        if(m_fileSendTimer) {
//            m_fileSendTimerSwitch = false;
//            m_fileSendTimer->joinable() ? m_fileSendTimer->join() : void();
//            m_fileSendTimer.reset((new std::thread([this] {
//                this->FileSendTimerThread();
//            })));
//        } else {
//            m_fileSendTimer = std::unique_ptr<std::thread>(new std::thread([this] {
//                this->FileSendTimerThread();
//            }));
//        }
    }
    std::cout <<"--------------- Begain get frame ----------------" << std::endl;
    if(m_replaystate){
        m_replaystate = false;
    }
MP4_OPEN_AGAIN:
    while(av_read_frame(m_pFormatCtx, packet) >= 0 && m_running) {

        if(m_streamType == FILE) {
            if(packet->dts < 0) {
                av_packet_unref(packet);
                continue;
            }
            if(packet->stream_index == m_videoindex) {
                double packetTimes = packet->pts * av_q2d(
                            m_pFormatCtx->streams[m_videoindex]->time_base);
                //std::cout << "packetTimes = " << packetTimes << std::endl;

                if(m_playTimeSlot.able) {
                    if(packetTimes < m_playTimeSlot.begain) {
                        av_packet_unref(packet);
                        return;
                    } else if(packetTimes >= m_playTimeSlot.end) {
                        av_packet_unref(packet);
                        //goto OPEN_AGAIN;
                    }
                }
                if(m_setFrameTime >= 0) {
                    if(!m_rePlayed) {
                        m_rePlayed = true;
                        av_packet_unref(packet);
                        if(m_fileType == MP4) {
                            av_seek_frame(m_pFormatCtx, m_videoindex,
                            0, AVSEEK_FLAG_BACKWARD);
                            av_packet_unref(packet);
                            packetTimes = 0;
                            continue;
                        } else if(m_fileType == M3U8) {
                            av_packet_unref(packet);
                            goto OPEN_AGAIN;
                        }
                    } else {
                        if(packetTimes < m_setFrameTime) {
                            av_packet_unref(packet);
                            continue;
                        } else {
                            m_setFrameTime = -1;
                            m_rePlayed = false;
                        }
                    }
                }
            } else if(m_setFrameTime >= 0) {
                av_packet_unref(packet);
                continue;
            }
        }

        PacketSend(packet);
    }
    if(m_running) {
        if(m_fileType == MP4) {
            av_seek_frame(m_pFormatCtx, m_videoindex,
            0, AVSEEK_FLAG_BACKWARD);
            goto MP4_OPEN_AGAIN;
        }
        //重连
        if(m_streamType == STREAM){
            qDebug()<<"视频连接视频,进行重连.."<<m_replaystate;
//            av_packet_free(&packet);
//            avformat_free_context(m_pFormatCtx);
//            av_packet_unref(packet);
            m_replaystate = true;
//            av_packet_free(&packet);
            _replaystate(m_replaystate);
        }
    }
    av_packet_free(&packet);
    m_running = false;
//    std::cout << "GetFrame quit" << std::endl;


}

void FfmpegGetFrames::PacketSend(AVPacket *packet)
{
    double packetTimes = packet->pts * av_q2d(
                m_pFormatCtx->streams[m_videoindex]->time_base);
    if(m_streamType == FILE) {
        if(packet->dts < 0) {
            av_packet_unref(packet);
            return;
        }
        _framePositionSend ? _framePositionSend(packetTimes) : void();
        FileSendFrame(packet);
        av_packet_unref(packet);

        while(!m_playState) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    } else {
        if(_bindAVPacketSendPtr) {
            if(packet->dts < 0 || !m_playState) {
                av_packet_unref(packet);
                return;
            } else {
                _bindAVPacketSendPtr(packet);
                av_packet_unref(packet);
            }
        }
    }
}

void FfmpegGetFrames::RunStop()
{
    m_running = false;
    m_playState = true;
    m_palyDelayTime = 0;
    m_fileSendTimerSwitch = false;

    m_fileSendSwitch = false;
    if(m_getFrameThread) {
        if(m_getFrameThread->joinable()){
            m_getFrameThread.release();
            m_getFrameThread = nullptr;
        }
    }

    m_audioindex = -1;
    m_videoindex = -1;

    std::cout << "FfmpegGetFrames::RunStop()" << std::endl;
}

void FfmpegGetFrames::RunReStop()
{
    m_running = false;
    m_playState = true;
    m_palyDelayTime = 0;
    m_fileSendTimerSwitch = false;
    avformat_close_input(&m_pFormatCtx);
    m_fileSendSwitch = false;

    if(m_getFrameThread) {
        if(m_getFrameThread->joinable()){
            m_getFrameThread.release();
            m_getFrameThread = nullptr;
        }
    }
    m_audioindex = -1;
    m_videoindex = -1;

    std::cout << "FfmpegGetFrames::RunReStop()" << std::endl;
}

void FfmpegGetFrames::RunStart(std::string ip, int port, std::string user, std::string pwd, std::string url)
{
    if(m_running) {
        RunStop();
    }
    m_playState = true;
    m_palyDelayTime = 0;
    m_url = url.empty() ? "rtsp://" + user + ":"
                          + pwd + "@" + ip + ":" + std::to_string(port)
                        : url;

    m_streamType = m_url.find("://") == std::string::npos ? FILE : STREAM;
    qDebug()<<"Run_Start:"<<m_streamType<<m_url.data();

    if(m_getFrameThread) {
        m_getFrameThread->joinable() ? m_getFrameThread->join() : void();
        m_getFrameThread.reset((new std::thread([this] {
            this->GetFrame();
         })));
    } else {
        m_getFrameThread = std::unique_ptr<std::thread>(new std::thread([this] {
            this->GetFrame();
         }));
    }
}

void FfmpegGetFrames::FileSendFrame(AVPacket * packet)
{
    if(_bindAVPacketSendPtr) {
        if(packet->dts < 0) {
            av_packet_unref(packet);
            return;
        }
        if(packet->stream_index == m_videoindex) {
            while(!m_fileSendSwitch) {
                std::this_thread::sleep_for(std::chrono::microseconds(1));
                if(!m_running) {
                    break;
                }
            }
            m_fileSendSwitch = false;
        }
        _bindAVPacketSendPtr(packet);
//        std::cout << "m_audioFrameRate = " << m_audioFrameRate;
    }
}


void FfmpegGetFrames::SetFrameTime(long time)
{
    std::cout << "SetFrameTime = " << time << std::endl;
    m_setFrameTime = time;
    m_playState = true;
}

void FfmpegGetFrames::SetPlayState(bool state)
{
    m_playState = state;
}


void FfmpegGetFrames::SetPlayDelayTime(int time)
{
    m_palyDelayTime = time;
}

void FfmpegGetFrames::SetPlayTimeSlot(unsigned long begain, unsigned long end)
{
    m_playTimeSlot.begain = begain;
    m_playTimeSlot.end = end;
    m_playTimeSlot.able = true;
    std::cout << "SetPlayTimeSlot = " << begain << " " << end << std::endl;
}

void FfmpegGetFrames::SetPlayTimeUnabled()
{
    m_playTimeSlot.clear();
}

void FfmpegGetFrames::FileSendTimerThread()
{
    m_fileSendTimerSwitch = true;
    while(m_fileSendTimerSwitch) {
        std::this_thread::sleep_for(std::chrono::microseconds((int)(1000.0 / (m_frameRate + 1) * 1000.0) + (m_palyDelayTime * 1000)));
        m_fileSendSwitch = true;
        if(!m_running) {
            break;
        }
    }
}


