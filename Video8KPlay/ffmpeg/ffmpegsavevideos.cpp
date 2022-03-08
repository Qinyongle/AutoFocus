#include "ffmpegsavevideos.h"


FfmpegSaveVideos::FfmpegSaveVideos()
{

}


FfmpegSaveVideos::~FfmpegSaveVideos()
{
    RunStop();
    std::cout << "~FfmpegSaveVideos" << std::endl;
}

void FfmpegSaveVideos::RunStart()
{
    m_runState = true;
    if(m_thread) {
        if(m_reSave){
            m_reSave = false;
            m_thread->joinable() ? m_thread->detach() : void();
            m_thread = std::unique_ptr<std::thread>(new std::thread([this]{
                this->ThreadRun();
            }));
        }else{
            m_thread->joinable() ? m_thread->join() : void();
        }
    } else {
        m_thread = std::unique_ptr<std::thread>(new std::thread([this]{
            this->ThreadRun();
        }));
    }
}

void FfmpegSaveVideos::RunStop()
{
    m_runState = false;
    if(m_thread) {
        m_thread->joinable() ? m_thread->join() : void();
    }
    std::cout << "FfmpegSaveVideos::RunStop()" << std::endl;
}

void FfmpegSaveVideos::SetAVFormat(AVFormatContext *iFormat, FfmpegGetFrames::STREAM_TYPE)
{
//    std::cout << "***************************************88" << std::endl;
    m_iFormat = iFormat;
    int ret = 0;
    std::cout << "m_iFormat->nb_streams" << m_iFormat->nb_streams << std::endl;
    for (unsigned int i = 0; i < m_iFormat->nb_streams; i++) {
        if (m_iFormat->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            m_inputVideoStreamIdx = i;
        }
        else if (m_iFormat->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            m_inputAudioStreamIdx = i;
        }
    }


    //std::cout << "frameRate = " << m_iFormat->streams[input_video_stream_idx]->avg_frame_rate.num << m_iFormat->duration << std::endl;

    /* allocate the output media context */
    m_output_file_name = m_filebasePath + "video-1.ts";
    avformat_alloc_output_context2(&m_oFormat, nullptr, nullptr, m_output_file_name.data());
    if (!m_oFormat) {
        std::cout << "ERROR:avformat_alloc_output_context2" << std::endl;
        return;
    }

    m_outStream = AddOutStream();
    if(!m_outStream) {
        std::cout << "m_outStream is nullptr" << std::endl;
    }
    /* open the output file, if needed */
    av_dump_format(m_oFormat, 0, m_output_file_name.data(), 1);
    if ((m_oFormat->oformat->flags & AVFMT_NOFILE) == 0) {
        ret = avio_open(&m_oFormat->pb, m_output_file_name.data(), AVIO_FLAG_WRITE);
        if ( ret < 0) {
            std::cout << "Could not open " << m_output_file_name << ret << std::endl;
            return ;
        }
    } else {
        std::cout << "ERROR:ofmt->flags & AVFMT_NOFILE"
                  << m_oFormat->oformat->flags << std::endl;
    }
    ret = avformat_write_header(m_oFormat, nullptr);
    if (ret < 0) {
        std::cout << "ERROR: avformat_write_header" << ret << std::endl;
    } else {
        std::cout << "SUCCESS: avformat_write_header" << ret << std::endl;
    }

    if(m_filebasePath.length() == 0 || m_fileName.length() == 0) {
        return;
    }
    RunStart();
}

void FfmpegSaveVideos::ThreadRun()
{
    int dts = 0;
    int dtsLast = 0;
    double segmentTime = 0.0;
    double preSegmentTime = 0.0;
    unsigned int firstSegment = 0;     //第一个分片的标号
    unsigned int lastSegment = 0;      //最后一个分片标号
    unsigned int videoSegment = lastSegment;
    unsigned int currentSegmentDuration = 0;
    unsigned int actualSegmentDurations[1024] = {0}; //各个分片文件实际的长度

    while(m_runState) {
        if(m_avPacketQueue.size() >= 1) {
//            std::cout << "m_avPacketQueue size = " << m_avPacketQueue.size() << std::endl;
            // get packet
            m_getPacketMutex.lock();
            AVPacket packet = m_avPacketQueue.front();
            m_avPacketQueue.pop();
            m_getPacketMutex.unlock();
            if(packet.stream_index != m_inputVideoStreamIdx) {
                av_packet_unref(&packet);
                continue;
            }
            // get stream
            AVStream* in_stream;
            AVStream* out_stream;
            in_stream = m_iFormat->streams[packet.stream_index];
            if (packet.stream_index >= m_streamMappingSize ||
                m_streamMapping[packet.stream_index] < 0) {
                std::cout << "packet.stream_index >= streamMappingSize"
                          << packet.stream_index << m_streamMappingSize << std::endl;
                av_packet_unref(&packet);
                continue;
            }

            // write m3u8
            segmentTime = packet.dts * av_q2d(in_stream->time_base);
            currentSegmentDuration = (int)(segmentTime - preSegmentTime + 0.5);
            actualSegmentDurations[lastSegment] = (currentSegmentDuration > 0 ?
                                                       currentSegmentDuration : 1);
            //std::cout << "m_segmentTime = " << segmentTime - preSegmentTime << std::endl;
            if(segmentTime - preSegmentTime >= SEGMENT_DURATION) {
                WriteIndexFile(firstSegment,++lastSegment,true,actualSegmentDurations);
                int ret = av_write_trailer(m_oFormat);
                if(ret < 0) {
                    std::cout << "ERROR:av_write_trailer(m_oFormat)" << std::endl;
                    av_packet_unref(&packet);
                    continue;
                }

                avio_flush(m_oFormat->pb);
                avio_close(m_oFormat->pb);
                m_output_file_name = m_filebasePath + "video-" + std::to_string(videoSegment++) + ".ts";
                ret = avio_open(&m_oFormat->pb, m_output_file_name.data(), AVIO_FLAG_WRITE);
                if ( ret < 0) {
                    std::cout << "Could not open " << m_output_file_name << ret << std::endl;
                    av_packet_unref(&packet);
                    return ;
                }

                // Write a new header at the start of each file
                ret = avformat_write_header(m_oFormat, nullptr);
                if (ret != 0) {
                    std::cout <<"Could not write mpegts header to first output file %d \n" << ret;
                    av_packet_unref(&packet);
                    return;
                }
                preSegmentTime = segmentTime;
            }

            packet.stream_index = m_streamMapping[packet.stream_index];
            //std::cout << "packet.stream_index = " << packet.stream_index << std::endl;
            out_stream = m_oFormat->streams[packet.stream_index];
            /* copy packet */
            packet.pts = av_rescale_q_rnd(packet.pts, in_stream->time_base, out_stream->time_base, (AV_ROUND_NEAR_INF));
            packet.dts = av_rescale_q_rnd(packet.dts, in_stream->time_base, out_stream->time_base, (AV_ROUND_NEAR_INF));
            packet.duration = av_rescale_q(packet.duration, in_stream->time_base, out_stream->time_base);
            packet.pos = -1;

            dts = (int)packet.pts;
            if (dts < dtsLast) {
                av_packet_unref(&packet);
                continue;
            }
            dtsLast = dts;
            //std::cout << "hello" << packet.pts << " " << packet.dts << std::endl;
            if (av_write_frame(m_oFormat, &packet) < 0) {
                std::cout << "[ERROR]: av_interleaved_write_frame" << std::endl;
            }
            av_packet_unref(&packet);
        } else {
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }
    }
    av_write_trailer(m_oFormat);
    WriteIndexFile(firstSegment,++lastSegment,true,actualSegmentDurations);
    std::cout << "FfmpegSaveVideos::ThreadRun() quit" << std::endl;
}

AVStream * FfmpegSaveVideos::AddOutStream()
{
    AVStream* outStream = nullptr;
    int streamIndex = 0;
    m_streamMappingSize = m_iFormat->nb_streams;
    m_streamMapping = (int *)av_mallocz_array(m_streamMappingSize, sizeof(int));
    if(!m_streamMapping) {
        std::cout << "streamMapping is nullptr" << std::endl;
    }

    for (unsigned int i = 0; i < m_iFormat->nb_streams; i++) {
        AVStream* inStream = m_iFormat->streams[i];
        AVCodecParameters* inCodecpar = inStream->codecpar;

        if (inCodecpar->codec_type != AVMEDIA_TYPE_AUDIO &&
            inCodecpar->codec_type != AVMEDIA_TYPE_VIDEO &&
            inCodecpar->codec_type != AVMEDIA_TYPE_SUBTITLE) {
            m_streamMapping[i] = -1;
            continue;
        }
        m_streamMapping[i] = streamIndex++;
        outStream = avformat_new_stream(m_oFormat, nullptr);
        if (outStream == nullptr) {
            std::cout << "avformat_new_stream null" << std::endl;
            return nullptr;
        }
        if (avcodec_parameters_copy(outStream->codecpar, inCodecpar) < 0) {
            return nullptr;
        }
        outStream->codecpar->codec_tag = 0;
    }
    return outStream;
}

int FfmpegSaveVideos::WriteIndexFile(const unsigned int firstSegment,
                   const unsigned int lastSegment,
                   const bool end,
                   const unsigned int actualSegmentDurations[])
{
    m_m3u8FileName = m_filebasePath + m_fileName + ".m3u8";
//    qDebug()<<m_m3u8FileName.data();
    std::ofstream file(m_m3u8FileName);
    if(file.is_open()) {
        std::string writeStr;
        writeStr = "#EXTM3U\n#EXT-X-TARGETDURATION:" + std::to_string(SEGMENT_DURATION)
                + "\n#EXT-X-MEDIA-SEQUENCE:" + std::to_string(firstSegment) + "\n";
        file.write(writeStr.data(), writeStr.size());
        for(unsigned int i = firstSegment; i <= lastSegment; i++) {
            writeStr = "#EXTINF:" + std::to_string(actualSegmentDurations[i])
                     + ",\n" + "./video-" + std::to_string(i) + ".ts\n";
            //std::cout << "writeStr = " << writeStr.data() << std::endl;;
            file << writeStr.data();
        }
        if(end) {
            writeStr = "#EXT-X-ENDLIST\n";
            file.write(writeStr.data(), writeStr.size());
        }
    } else {
        return -1;
    }
    file.close();
}


void FfmpegSaveVideos::GetAVPacket(AVPacket * packet)
{
    if(m_filebasePath.length() == 0 || m_fileName.length() == 0) {
        return;
    }
    AVPacket *dataTemp = av_packet_alloc();
    av_packet_ref(dataTemp, packet);
    m_getPacketMutex.lock();
    m_avPacketQueue.push(*dataTemp);
    m_getPacketMutex.unlock();
}

void FfmpegSaveVideos::SetFilePath(std::string path, std::string name)
{
    std::cout << "save path = " << path
              << "save name = " << name;
    m_filebasePath = path;
    m_fileName = name;
}

void FfmpegSaveVideos::SetReSaveSate(bool state)
{
    m_reSave = state;
}

void FfmpegSaveVideos::AddKeyPoint(unsigned long point)
{
    auto path = m_filebasePath + m_fileName + ".pos";
    std::ofstream file(path, std::ofstream::app);
    file << point << std::endl;
    file.close();
}

