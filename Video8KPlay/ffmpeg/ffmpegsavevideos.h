#ifndef FFMPEGSAVEVIDEOS_H
#define FFMPEGSAVEVIDEOS_H

#include <thread>
#include <memory>
#include <iostream>
#include <fstream>
#include "ffmpeggetframes.h"

class FfmpegSaveVideos
{

#define OUTPUT_PREFIX       "ZWGTEST"              //切割文件的前缀
#define NUM_SEGMENTS        50                      //在磁盘上一共最多存储多少个分片
#define SEGMENT_DURATION    5                      //每一片切割多少秒

public:

    explicit FfmpegSaveVideos();
    ~FfmpegSaveVideos();
    void RunStart();
    void RunStop();
    void SetAVFormat(AVFormatContext *, FfmpegGetFrames::STREAM_TYPE);
    void GetAVPacket(AVPacket *);
    void SetFilePath(std::string, std::string);
    void SetReSaveSate(bool state);
    void AddKeyPoint(unsigned long);

private:
    void ThreadRun();
    AVStream * AddOutStream();
    int WriteIndexFile(const unsigned int firstSegment,
                       const unsigned int lastSegment,
                       const bool end,
                       const unsigned int actualSegmentDurations[]);

private:
    bool m_runState = false;
    std::unique_ptr<std::thread> m_thread = nullptr;
    std::queue<AVPacket> m_avPacketQueue;
    std::string m_filebasePath;// = "./video/";
    std::string m_fileName;// = "123";
    std::string m_output_file_name = m_filebasePath + m_fileName + ".ts";
    std::string m_m3u8FileName = m_filebasePath + m_fileName + ".m3u8";

    AVFormatContext *m_iFormat;
    AVFormatContext *m_oFormat;

    int m_inputVideoStreamIdx = -1;
    int m_inputAudioStreamIdx = -1;

    int* m_streamMapping = nullptr;
    int m_streamMappingSize;

    AVStream* m_outStream;

    std::mutex m_getPacketMutex;
    bool m_reSave = false;


};

#endif // FFMPEGSAVEVIDEOS_H
