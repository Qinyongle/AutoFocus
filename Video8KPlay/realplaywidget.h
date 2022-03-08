#ifndef REALPLAYWIDGET_H
#define REALPLAYWIDGET_H

#include <QObject>
#include <QWidget>
#include <QMainWindow>
#include "ffmpeg/ffmpeggetframes.h"
#include "ffmpeg/ffmpegsavevideos.h"
#include "ffmpeg/ffmpegdecoder.h"
#include "cpptransqt.h"
#include <QSettings>
#include <QFile>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QCoreApplication>
#include <QThread>
#include <QDateTime>


class RealPlayWidget: public QObject
{
    Q_OBJECT
public:
    explicit RealPlayWidget(QWidget *p, QObject *parent = nullptr);
    ~RealPlayWidget();
signals:
    void SendPicPath(QString);
    void SendImg(unsigned char *,unsigned int,unsigned int);
public slots:
    void RunStart(QString url);
    void RunStop();
    // pic
    void GetPicCmd();

    void SwtichStream(QString _url);


private:
    void InitThread();


private:
    std::unique_ptr<FfmpegGetFrames> m_getFrames = nullptr;
    CppTransQt *m_cppTransQt = nullptr;
    QThread *m_cppTransQtThread = nullptr;
    std::unique_ptr<FfmpegSaveVideos> m_saveVideos = nullptr;
    QWidget *m_homePagePlayLable = nullptr;

    QString m_decodeType;
    QString m_savePath;
    QString m_saveName;
    QString m_url;
    bool resavestate = false;
};

#endif // REALPLAYWIDGET_H
