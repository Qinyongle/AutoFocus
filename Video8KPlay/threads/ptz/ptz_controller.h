#ifndef PTZ_CONTROLLER_H
#define PTZ_CONTROLLER_H

#include <QObject>
#include <threads/ptz/ptz_opencv.h>
#include <QDebug>
#include <QQueue>

typedef enum
{
    INCRESE=0,
    DECLINE
}TREND;
typedef enum{
    StepGetSpeedM=0,        //获取速度的返回信息
    StepSendPoAngle=1,      //驱动电机正转
    StepGetAngleM=2,        //获取电机的返回信息
    StepCheckOut=3,         //检验是否到达最大清晰度值
    StepSendReAngle=4,      //驱动电机反转
    StepSendSpeed=5,        //更改电机速度
}STEPXXX;

enum{
    CheckFinished=0,    //检验完成，表示获取到了最大清晰度值，并从头开始一步步对焦到最大清晰度
    Check=1,            //检验，当为检验状态的时候需要发送电机指令驱动电机
    NoAct=2,            //无反应
};
typedef struct PID{
        float kp;
        float ki;
        float kd;
        float kp_out;
        float ki_out;
        float kd_out;
        float Cur_Error;
        float	last_Error;
    }PIDx;
class ptz_controller : public QObject
{
    Q_OBJECT
public:
    explicit ptz_controller(QObject *parent = nullptr);
    float pid(PIDx *pid,float Cur,float Tar);
    bool Reverseflag,Once_flag;
    void SendSpeedMax(int data);
    void SendAnglePo(double data);
    void SendAngleRe(double data);
    void SendSpeedMin(int data);
    void SendStop();
    void RunOnce(void (*fun)());
    double Average(double);
    void SendStep(uint8_t *flag);
    PIDx *focusPid=nullptr;
    QByteArray ReceiveData;
    #define MistakeJude     20         //误判宏定义
    #define MinGap          0.8f       //最小误差（实践结果）
    #define QueueLength     5
    #define MinSpeed        5000    //用于检验最大清晰度
    #define MaxSpeed        200     //用于回到初始位置
    #define MediumSpeed     2000     //用于对焦至最大清晰度
    #define RangeAngle      210     //电机走完一个范围的角度
    QQueue<double>DataQueue;
    STEPXXX Step =StepSendSpeed;    //初始化第一步为更改速度
    uint8_t CheckAngle=NoAct;
signals:
    void CtlSend(QByteArray);
    void signalControll();
    void CheckDone();      //检测最大值完成信号
    void StepMaxDone();    //转至清晰度最大值信号

public slots:
    void CtlMotor();
    void GetArray(QByteArray);


private:
    TREND Trend;

};

#endif // PTZ_CONTROLLER_H
