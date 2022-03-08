#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QStackedLayout>
#include <QLayout>
#include <QPushButton>
#include <QDesktopWidget>
#include <QTabWidget>
#include <QThread>
#include <QMenuBar>
#include <QLabel>
#include <QSlider>
#include <QLineEdit>
#include <QSpinBox>
#include <QSettings>
#include <QFileDialog>
#include "ffmpeg/videorealtimeget.h"
#include "mosaiclable.h"
#include "selectedlable.h"
#include "selectedwidegt.h"
#include "streamconfigwidget.h"
#include "cameraconfigwidget.h"
#include "mainwindowwidget.h"
#include "audioplay.h"
#include "ptzconfig.h"
#include "ptzctrl.h"
#include "mainprogram.h"
#include "realplaywidget.h"
#include <QKeyEvent>
#include "ffmpeg/imagecapture.h"

#define PICTURE_TIME 5000

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
#define SLIDER_SMOOTH_NUM 1000
#define ZONE_NUM 10
#define MAX_DELAY_SPEED 1000
#define MIN_DELAY_SPEED -1000

    typedef struct SELECTED_LABLE_STRUCT {
        SelectedLable *lable = nullptr;
        SelectedWidegt *widget = nullptr;
    } SELECTED_LABLE;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
int getWindowX();
int getWindowY();
signals:
    void SetSavePathNameCmd(QString ,QString);
//    void SetRtspCmd(QString, QtFfmpegDecoder::DECODE_TYPE);
    void RunStartCmd(QString, QtFfmpegDecoder::DECODE_TYPE);
    void RunStopCmd();
    void PlayingState(bool);
    void SetVideoFrameInt(long);
    void SetDrawZoneSize(int);
    void SetDecodeSpeedDelay(int);
    // audio
    void AudioStart(int, int, int);
    void AudioQuit(bool);
    void SetVolume(double);
    //new
    void SendPicState();

    //AUX
    void SendAuxRect(QRect);

    //new thread--save--realshow---sendpicture
    void StartCmd(QString);

    //Pos
    void GetPosStop();
    void GetPosStart(int);
    //Picture
    void BlurTest();
    void SendSwitch(QString url);
private slots:
    void OpenFile();
    void OpenStream();
    void ClosePlay();
    void PlayButtonClicked();
    void SliderValueChanged(int);
    void SliderReleased();
    void SliderPressed();
    void VolumeSliderMoved(int);
    void VolumeSliderVaalueChanged(int);
    void VolumeButtonClicked();

    void DecodeMenuTriggered(QAction *);
    void ZoneNumMenuTriggered(QAction *);
    void AddSpeedClicked();
    void ReduceSpeedClicked();
    void ResetSpeedClicked();
    void SpeedNumSpinBoxValueChanged(int);
    void CameraConfigConfig();

    // about
    void AboutActionTriggered();

    //pic
    void PictureActionTriggered();

public slots:
    void GetFrameData(unsigned long long, double);
    void GetCurrentFramTime(double);
    void LableKeyPressed();
    void GetStreamUrl(QString);
    void ConfigFocusZone(int, int, int, int);
    void GetWindowPos();
    void PicBlurTest();
protected:
    void resizeEvent(QResizeEvent* size);
    void keyPressEvent(QKeyEvent *event);

public:
    void SwitchStream1();
    void SwitchStream2();

private:
    //new
    void InitPicture();
    void InitUi();
    void InitThread();
    void InitConfig();
    void OpenFfmpeg(QString);
    void InitZoneSize();
    void AllZoneClose();
    inline void SetVolumeConfig(int data) {
        GetConfig()->setValue("SYS/Volume", data);
    }

    inline int GetVolumeConfig() {
        return GetConfig()->value("SYS/Volume").toInt();
    }

    inline void SetVolumeZeroConfig(bool state) {
        GetConfig()->setValue("SYS/VolumeZero", state);
    }

    inline bool GetVolumeZeroConfig() {
        return GetConfig()->value("SYS/VolumeZero").toBool();
    }

    inline QSettings * GetConfig() {
        QDir::setCurrent(QCoreApplication::applicationDirPath());
        QSettings *config = new QSettings("./config.ini", QSettings::IniFormat);
        return config;
    }
private:
    Ui::MainWindow *ui;

    QMenuBar *m_menuBar = nullptr;// = new QMenuBar(this);
    QMenu *m_open = nullptr;
    QAction *m_openFile = nullptr;
    QAction *m_closePlay = nullptr;
    QAction *m_openStream = nullptr;
    // 解码
    QMenu *m_decodeConfigMenu = nullptr;
    QMenu *m_decodeSelectMenu = nullptr;
    QAction *m_intelDecodeAct = nullptr;
    QAction *m_nvidiaDecodeAct = nullptr;
    // 选区
    QMenu *m_selectedWindow = nullptr;
    QMenu *m_zoneNumSet = nullptr;
    QAction *m_zoneNumAct[10];
    MainWindowWidget *m_mainWindow  = nullptr;
    MosaicLable *m_originalLable = nullptr;

    // 云台
    QMenu *m_ptzMenu = nullptr;
    QAction *m_ptzConfigAction = nullptr;
    QAction *m_ptzCtrlAction = nullptr;
    PtzConfig *m_ptzConfigWindow = nullptr;
    PtzCtrl *m_ptzCtrlWindow = nullptr;

    // ca confia
    QAction *m_cameraConfig = nullptr;

    // 所有选取置顶
    QAction *m_selectedZoneBeTop = nullptr;

    // 关于
    QAction *m_aboutAct = nullptr;
    QWidget *m_aboutWidget = nullptr;
    // 发送图片
    QAction *m_sendPic = nullptr;
    bool Pic_state = false;
    int m_time = 0;
    QString Server_Url = nullptr;
    QString Gps_Ip = nullptr;
    //new
    MainProgram *mainPro;
    QThread * m_thread;
    int Gps_Port = 0;

    //发送图片时间差
    int m_differ = 0;

    // 进度条
    QSlider *m_speedSlider = nullptr;
    QPushButton *m_playButton = nullptr;
    QLineEdit *m_numShow = nullptr;
    // 声音
    QPushButton *m_volumeButton = nullptr;
    QLineEdit *m_volumeShow = nullptr;
    QSlider *m_volumeSlider = nullptr;

    // 倍速播放
    QLabel *m_frameSpeedLable = nullptr;
    QLabel *m_speedNumLable = nullptr;
    QSpinBox *m_speedNumSpinBox = nullptr;
    QPushButton *m_addSpeed = nullptr;
    QPushButton *m_reduceSpeed = nullptr;
    QPushButton *m_resetSpeed = nullptr;

    // 串流输入配置界面
    StreamConfigWidget *m_streamConfig = nullptr;

    // 相机配置界面
    CameraConfigWidget *m_cameraConfigWidget = nullptr;

    //获取窗口的坐标以及宽度长度
    QTimer *m_getPosTimer=nullptr;
    QThread *m_getPosTimerThread=nullptr;
    //图片模糊检测
    PtzOpencv *m_hvOpencv=nullptr;
    QThread *m_hvOpencvThread = nullptr;

    ImageCapture *capture;
    QThread *m_imageCapture=nullptr;
private:
    QString m_filePath;
//    VideoRealTimeGet *m_ffmpeg = nullptr;
    RealPlayWidget * m_ffmpeg = nullptr;
    QThread *m_ffmpegThread = nullptr;
    long long m_videoMaxFramCount = 0;
    long long m_videoMaxTime = 0;

    // andio
    AudioPlay *m_audioPlay = nullptr;
    QThread *m_audioPlayThread = nullptr;
    int m_volumeNumber = 0;
    bool m_volumeZero = false;
    double m_selectedFrameTime = 0;
    int m_zoneSize = 1;
    QList<SELECTED_LABLE> m_selectedLableList;

    int m_currentSpeedValue = 0;
    QtFfmpegDecoder::DECODE_TYPE m_videoDecodeType = QtFfmpegDecoder::QSV;
    //录制
    QString m_path;
    QString m_name;

    //副显示器 坐标  --发送至选区窗口，方便移动
    QRect Aux_rect;

    QString m_url;
    QString switch_url;

};
#endif // MAINWINDOW_H
