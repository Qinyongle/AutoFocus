#ifndef MAINPROGRAM_H
#define MAINPROGRAM_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include <QNetworkRequest>
#include <QBuffer>
#include <QNetworkAccessManager>
#include <QHttpPart>
#include <QFile>
#include <QVariant>
#include <QNetworkReply>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include "gpsgetthread.h"
#include "webapi/webapi.h"
class MainProgram : public QObject
{
    Q_OBJECT
#define GETCONFIG "/ctrl/stream_setting?index=stream1&action=query"
#define BITWIDTH "/ctrl/stream_setting?index=stream1&bitwidth="
#define BITRATE "/ctrl/stream_setting?index=stream1&bitrate="
#define ENCODETYPE "/ctrl/stream_setting?index=stream1&venc="
#define RESOLUTION7680 "/ctrl/stream_setting?index=stream1&width=7680&height=4320"
#define RESOLUTION4096 "/ctrl/stream_setting?index=stream1&width=4096&height=2160"
#define RESOLUTION1920 "/ctrl/stream_setting?index=stream1&width=1920&height=1080"
#define FOCUSCENTER "/ctrl/af?action=update_roi_center&x=500&y=500"
public:
    explicit MainProgram(QObject *parent = nullptr);
    ~MainProgram();
signals:
//    void RunStartCmd(QString, QtFfmpegDecoder::DECODE_TYPE);
    void RunStopCmd();
    void GetPicCmd();
    void SetSavePathNameCmd(QString, QString);

    // timer
    void StartTimer(int);
    void StopTimer();

    // ffmpeg

    // gps
    void ConnectGps(QString, int);

public slots:
    void GetPicPath(QString);
    void SendPicTimer();
    void LoadThread();

    // set
    void SetSendTimeInterval(int); // ms
    void SetDecodeType(QString);
    void SetServiceUrl(QString);
    void SetFilePath(QString);
    void SetGpsTcp(QString, int);
    void SetSavePathName(QString, QString);

    // reply
    void replyFinished(QNetworkReply *);
    void httpUploadFinished();

    // gps
    // 8K SET

private:
    void InitThread();
    bool RetWebApiState(QByteArray);


private:
    struct GPS_DATA{
        double lng;
        double lat;
        float nav;
        float hight;
    };
//    VideoRealTimeGet *m_ffmpeg = nullptr;
//    QThread *m_thread = nullptr;

    QTimer *m_sendTimer = nullptr;
    QThread *m_sendTimerThread = nullptr;

    QNetworkAccessManager *manager = nullptr;
    QNetworkReply *m_reply = nullptr;
    QHttpMultiPart *m_multiPart = nullptr;
    QFile *m_file;
    QVariant variant;

    QString m_picPath;
    int m_sendTimeInterval = 0;
    QString m_decodeType;
    QString m_serviceUrl;
    QString m_filePath;
    QString m_cameraIp;

    // gps
    GpsGetThread *m_gpsGet = nullptr;
    QThread *m_gpsGetThread = nullptr;
    GpsData m_gpsData;
    GPS_DATA m_gps_data;
    QString m_tcpIp;
    int m_tcpPort = 0;

    // save
    QString m_savePath;
    QString m_saveName;
};

#endif // MAINPROGRAM_H
