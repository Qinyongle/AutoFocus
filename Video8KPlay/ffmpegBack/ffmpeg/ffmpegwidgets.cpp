#include "ffmpegwidgets.h"
#include <QDebug>
#include <QThread>
FFmpegWidgets::FFmpegWidgets(QString name, QObject *parent) : QObject(parent)
{
    m_running = false;
    m_name = name;
    qRegisterMetaType<QImage>("QImage");
    qRegisterMetaType<FFmpegWidgets::IMAGE_TYPE>("FFmpegWidgets::IMAGE_TYPE");

}

FFmpegWidgets::~FFmpegWidgets()
{
    RunStop();
    qDebug() << "ffmpeg RunStop";
}


void FFmpegWidgets::InitThread()
{
//    av_register_all();	// 初始化ffmpeg, 这是一个过时的函数, 刚开始学用这个会比较上手.
//    qDebug() <<"avcodec_version(): " <<avcodec_version();
}

void FFmpegWidgets::RunStart(FFmpegWidgets::IMAGE_TYPE type)
{
    if(type == RGB32) {
        AVFormatContext    *pFormatCtx;
        int                i, videoindex;
        AVCodecContext    *pCodecCtx;
        AVCodec            *pCodec;
        AVFrame    *pFrame, *pFrameRGB;
        unsigned char *out_buffer;
        AVPacket *packet;
        int ret, got_picture;
        struct SwsContext *img_convert_ctx;
        QStringList urlList = m_url.split("://");
        QString url;
        if(urlList.size() == 2) { // "rtsp://admin:admin12345@192.168.10.67:554"
            url = urlList.at(0) + "://" + m_user + ":" + m_passwd + "@" + urlList.at(1);
        } else {
            url = m_url;
        }
        qDebug() << "ffmpeg url = " << url;

        //初始化编解码库
        av_register_all();//创建AVFormatContext对象，与码流相关的结构。
        pFormatCtx = avformat_alloc_context();
        AVDictionary *optionsDict = NULL;
        //采用tcp传输
        av_dict_set(&optionsDict, "rtsp_transport", "tcp", 0);
        //如果没有设置stimeout，那么把ipc网线拔掉，av_read_frame会阻塞（时间单位是微妙）
        av_dict_set(&optionsDict, "stimeout", "5000000", 0);

        //初始化pFormatCtx结构
        if (avformat_open_input(&pFormatCtx, url.toLatin1().data(), NULL, &optionsDict) != 0){
            printf("Couldn't open input stream.\n");
            return ;
        }
        //获取音视频流数据信息
        if (avformat_find_stream_info(pFormatCtx, NULL) < 0){
            qDebug() << "Couldn't find stream information.\n";
            return ;
        }
        videoindex = -1;
        //nb_streams视音频流的个数，这里当查找到视频流时就中断了。
        for (i = 0; i < pFormatCtx->nb_streams; i++)
            if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO){
                videoindex = i;
                break;
        }
        if (videoindex == -1){
            qDebug() <<"Didn't find a video stream.\n";
            return ;
        }

        // 软件解码
        //获取视频流编码结构
        AVCodecParameters* codecpar = pFormatCtx->streams[videoindex]->codecpar;
        //查找解码器
        pCodec = avcodec_find_decoder(codecpar->codec_id);
        if (pCodec == NULL){
            qDebug() <<"Codec not found.\n";
            return ;
        }
        pCodecCtx = avcodec_alloc_context3(pCodec);
        if(!pCodecCtx) {
            qDebug() <<"Codec not found.\n";
            return ;
        }
        qDebug() << pCodec->id;
        pCodecCtx->pix_fmt = AVPixelFormat(codecpar->format);
        pCodecCtx->height = codecpar->height;
        pCodecCtx->width = codecpar->width;
        pCodecCtx->thread_count = 16;
        //用于初始化pCodecCtx结构
        if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0){
            qDebug() <<"Could not open codec.\n";
            return ;
        }
        //创建帧结构，此函数仅分配基本结构空间，图像数据空间需通过av_malloc分配
        pFrame = av_frame_alloc();
        pFrameRGB = av_frame_alloc();
        //创建动态内存,创建存储图像数据的空间
        //av_image_get_buffer_size获取一帧图像需要的大小
        out_buffer = (unsigned char *)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_RGB32, pCodecCtx->width, pCodecCtx->height, 1));
        av_image_fill_arrays(pFrameRGB->data, pFrameRGB->linesize, out_buffer,
            AV_PIX_FMT_RGB32, pCodecCtx->width, pCodecCtx->height, 1);

        packet = (AVPacket *)av_malloc(sizeof(AVPacket));
        //Output Info-----------------------------
        qDebug() <<"--------------- File Information ----------------\n";

        //初始化img_convert_ctx结构
        img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
            pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_RGB32, SWS_BICUBIC, NULL, NULL, NULL);
        //av_read_frame读取一帧未解码的数据
        m_running = true;
        while (av_read_frame(pFormatCtx, packet) >= 0 && m_running){
            //如果是视频数据
            if (packet->stream_index == videoindex){
                //解码一帧视频数据
                ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
                if (ret < 0){
                    qDebug() <<"Decode Error.\n";
                    return ;
                }
                if(got_picture){
                    sws_scale(img_convert_ctx, (const unsigned char* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height,
                        pFrameRGB->data, pFrameRGB->linesize);
                    QImage img((uchar*)pFrameRGB->data[0],pCodecCtx->width,pCodecCtx->height,QImage::Format_RGB32);
    //                    ui->label->setPixmap(QPixmap::fromImage(img));
    //                    Delay(40);
                    if(m_name.isEmpty())
                        emit SendImg(img);
                    else
                        emit SendImg(m_name, img);
                }
            }
            av_free_packet(packet);
            QThread::usleep(1);
        }

        sws_freeContext(img_convert_ctx);
        av_frame_free(&pFrameRGB);
        av_frame_free(&pFrame);
        avcodec_close(pCodecCtx);
        avformat_close_input(&pFormatCtx);
    } else if(type == NV12) {

        AVFormatContext    *pFormatCtx;
        int                i, videoindex;
        AVCodecContext    *pCodecCtx;
        AVCodec            *pCodec;
        AVFrame    *pFrame, *pFrameYUV;
        unsigned char *out_buffer;
        AVPacket *packet;
        int ret, got_picture;
        QStringList urlList = m_url.split("://");
        QString url = m_url;

        qDebug() << "ffmpeg url = " << url;

        //初始化编解码库
        av_register_all();//创建AVFormatContext对象，与码流相关的结构。
        pFormatCtx = avformat_alloc_context();
        //采用tcp传输
        AVDictionary *optionsDict = NULL;
        av_dict_set(&optionsDict, "rtsp_transport", "tcp", 0);
        av_dict_set(&optionsDict, "buffer_size", "204800000", 0);
        av_dict_set(&optionsDict, "stimeout", "5000000", 0);
        pFormatCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER | AV_CODEC_FLAG_LOW_DELAY;

RUN_AGAIN:
        m_playingState = true;
        //初始化pFormatCtx结构
        if (avformat_open_input(&pFormatCtx, url.toLatin1().data(), NULL, NULL) != 0){
            printf("Couldn't open input stream.\n");
            return ;
        }
        //获取音视频流数据信息
        if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
            qDebug() << "Couldn't find stream information.\n";
            return ;
        }
        //nb_streams视音频流的个数，这里当查找到视频流时就中断了。
        videoindex = -1;
//        for (i = 0; i < pFormatCtx->nb_streams; i++) {
//            if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO){
//                videoindex = i;
//                break;
//            }
//        }
//        if (videoindex == -1) {
//            qDebug() <<"Didn't find a video stream.\n";
//            return ;
//        }
        videoindex = av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_VIDEO, -1, -1, &pCodec, 0);
        // 获取视频流编码结构
        AVCodecParameters* codecpar = pFormatCtx->streams[videoindex]->codecpar;
        qDebug() << "code id = " << codecpar->codec_id;
        // 硬件解码
        //查找解码器
//        if(codecpar->codec_id == AV_CODEC_ID_H264) {
//            pCodec = avcodec_find_decoder_by_name("h264_qsv");
//        } else if(codecpar->codec_id == AV_CODEC_ID_HEVC){
//            pCodec = avcodec_find_decoder_by_name("hevc_qsv");
//        }

//        if (!pCodec) {
//            qDebug()<<"not find dcoder";
//            return ;
//        }


        // 使用从AVFormatContext得到的CodecContext
        pCodecCtx = avcodec_alloc_context3(pCodec);
        av_hwdevice_ctx_create(&pCodecCtx->hw_device_ctx, AV_HWDEVICE_TYPE_QSV, NULL, NULL, 0);
        avcodec_parameters_to_context(pCodecCtx, codecpar);
        pCodecCtx->thread_count = 6;
        qDebug() <<"codecpar->format = " << AVPixelFormat(codecpar->format) << AV_PIX_FMT_NV12;

        //用于初始化pCodecCtx结构
        if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0){
            qDebug() <<"Could not open codec.\n";
            return ;
        }


        //创建帧结构，此函数仅分配基本结构空间，图像数据空间需通过av_malloc分配
        pFrame = av_frame_alloc();
        pFrameYUV = av_frame_alloc();
        //创建动态内存,创建存储图像数据的空间
        //av_image_get_buffer_size获取一帧图像需要的大小 AV_PIX_FMT_YUV420P10LE AV_PIX_FMT_YUV420P
        out_buffer = (unsigned char *)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_YUV420P10LE, pCodecCtx->width, pCodecCtx->height, 1));
        packet = (AVPacket *)av_malloc(sizeof(AVPacket));
        qDebug() << "image size = " << pCodecCtx->width << pCodecCtx->height;
        int frameRate = pFormatCtx->streams[videoindex]->nb_frames / (pFormatCtx->duration / 1000000);
        emit SendFrameData(pFormatCtx->streams[videoindex]->nb_frames,
                           pFormatCtx->duration);
        qDebug() << "frameRate = " << frameRate;
        qDebug() << "timeBase = " << pFormatCtx->streams[videoindex]->time_base.num;
        qDebug() <<"--------------- Begain Decode ----------------\n";
        int videoWidth = pCodecCtx->width;
        int videoHeight = pCodecCtx->height;
        //av_read_frame读取一帧未解码的数据
        m_running = true;
        while (av_read_frame(pFormatCtx, packet) >= 0 && m_running){
            //如果是视频数据
            mutex.lock();
            if (packet->stream_index == videoindex){
                //解码一帧视频数据
                ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
                if (ret < 0){
                    qDebug() <<"Decode Error" << ret;
                    QThread::sleep(1);
                    goto decodeError;
                }
                if (got_picture){
//                    int bytes = 0;
                    int i, j ,k ;
                    for(i = 0; i < videoHeight; i++){ //将y分量拷贝
                        ::memcpy(out_buffer + videoWidth * i,pFrame->data[0] + pFrame->linesize[0] * i,videoWidth);
                    }
                    //int uv = videoHeight >> 1;
                    for(j = 0; j < videoHeight / 2; j++){ //将u分量拷贝
                        ::memcpy(out_buffer + videoWidth * i + videoWidth / 2 * j,
                                 pFrame->data[1] + pFrame->linesize[1] * j,videoWidth / 2);
                    }
                    for(k = 0; k < videoHeight / 2; k++){ //将v分量拷贝
                        ::memcpy(out_buffer + videoWidth * i + videoWidth / 2 * j + videoWidth / 2 * k,
                                 pFrame->data[2] + pFrame->linesize[2] * k,videoWidth / 2);
                    }
                    //qDebug() << "timeBase = " << packet->pts * av_q2d(pCodecCtx->time_base);
                    emit SendCurrentFramTime(packet->pts * av_q2d(pCodecCtx->time_base));
                    emit SendImg(out_buffer,pCodecCtx->width,pCodecCtx->height);
                }
            }
decodeError:
            av_free_packet(packet);
            mutex.unlock();
            //QThread::msleep(1000 / frameRate);
            while(!m_playingState) {
                QThread::msleep(100);
            }
            if(m_videoFrameInt != 0) {
                m_videoFrameInt = 0;
                if(m_videoFrameInt < pFormatCtx->duration / 1000000) {
                    av_seek_frame(pFormatCtx, videoindex, 15, AVSEEK_FLAG_BACKWARD | AVSEEK_FLAG_FRAME);
                    qDebug() << "seek over";
                }
            }
            //int pos = 20 * r2d(pFormatCtx->streams[videoindex]->time_base);

        }
        av_frame_free(&pFrameYUV);
        av_frame_free(&pFrame);
        avcodec_close(pCodecCtx);
        avformat_close_input(&pFormatCtx);
        if(m_running) {
            goto RUN_AGAIN;
        }
    }
}




void FFmpegWidgets::RunStop()
{
    m_running = false;
}

void FFmpegWidgets::SetRtsp(QString url, QString user, QString passwd)
{
    m_url = url;
    m_user = user;
    m_passwd = passwd;
    qDebug() << "url = " << url << user << passwd;
}

void FFmpegWidgets::PlayingState(bool state)
{
    m_playingState = state;
}

void FFmpegWidgets::GetVideoFrameInt(long num)
{
    m_videoFrameInt = num;
}









