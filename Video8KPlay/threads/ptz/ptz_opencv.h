#ifndef PTZ_OPENCV_H
#define PTZ_OPENCV_H

#include <QObject>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <QQueue>
extern  volatile double MaxBlurValue,lastBlurValue;
extern volatile double MaxValueFlag;
class PtzOpencv : public QObject
{
    Q_OBJECT
public:
    PtzOpencv();
    void SignalToOpencv();

public slots:
    void OpenCvLaplacian();
    void Average(double *data);

public:
#define BLURMAXQUEUE 10
#define AVERMAXQUEUE  5
    QQueue<double>BlurValue;//模糊值队列
    QQueue<double>AverQueue;
      uint8_t CheckDone=0;          //电机循值完毕标志
      uint8_t StepMaxDone=0;        //电机转到清晰最大值标志
signals:
    void imageReady();
    void SendAngle();
};

#endif // PTZ_OPENCV_H
