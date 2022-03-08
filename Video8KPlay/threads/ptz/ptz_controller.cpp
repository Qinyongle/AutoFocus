#include "ptz_controller.h"
#include "QString"
#include "ptz_opencv.h"

ptz_controller::ptz_controller(QObject *parent) : QObject(parent)
{
    focusPid = new PIDx;
    focusPid->kp=1;
    focusPid->ki=0;
    focusPid->kd=0;
    Reverseflag=true;
    Once_flag=false;
}

float ptz_controller::pid(PIDx *pid, float Cur, float Tar)
{
    float out;
    pid->Cur_Error=fabs(Tar-Cur);
    //qDebug()<<"Error="<<pid->Cur_Error;
    pid->kp_out = pid->kp*pid->Cur_Error;
    pid->kd_out = pid->kd*(pid->Cur_Error-pid->last_Error);
    if(fabs(pid->Cur_Error)<1)
    {
        pid->ki_out += pid->ki*pid->Cur_Error;
    }
    else
    {
        pid->ki_out=0;
    }
    pid->last_Error=pid->Cur_Error;
    out=pid->kp_out+pid->ki_out+pid->kd_out;
    return out;
}

void ptz_controller::SendSpeedMax(int data)
{
    QString SpeedMax=QString("Speed%1").arg(data);                //设置为最快的速度
    QByteArray SpeedArray = SpeedMax.toUtf8();
    emit CtlSend(SpeedArray);
}

void ptz_controller::SendAngleRe(double data)                      //反向
{
    QString BackZero=QString("A%1B").arg(data*-1);
    QByteArray BackArray=BackZero.toUtf8();
    emit CtlSend(BackArray);
}

void ptz_controller::SendAnglePo(double data)                      //正向
{
    QString BackZero=QString("A%1B").arg(data);
    QByteArray BackArray=BackZero.toUtf8();
    emit CtlSend(BackArray);
}

void ptz_controller::SendSpeedMin(int data)
{
    QString SpeedMin=QString("Speed5000");                //设置为最慢的速度
    QByteArray SpeedArray = SpeedMin.toUtf8();
    emit CtlSend(SpeedArray);
}

void ptz_controller::SendStop()
{
    QString SpeedMin=QString("Stop");                //设置为最慢的速度
    QByteArray SpeedArray = SpeedMin.toUtf8();
    emit CtlSend(SpeedArray);
}

double ptz_controller::Average(double data)
{
    double sum=0;
    if(DataQueue.size()==3)
    {
        DataQueue.dequeue();
    }
    DataQueue.enqueue(data);
    for(int i=0;i<DataQueue.size();i++)
    {
        sum=sum+DataQueue[i];
    }
    return sum/static_cast<double>(DataQueue.size());
}

void ptz_controller::CtlMotor()
{
   static uint8_t Reverse_Flag=false;
   static double current1=0,current2=0;
   static double stepangle=5;
   static uint32_t Mintimes=0;
   static uint32_t times=0;
   QString str;
   QByteArray angleArray;
   SendStep(&CheckAngle);  //发送步长检测
   if(CheckAngle==CheckFinished)       //checkangle==0表示检测完成或者没有正在检测状态
   {
       switch(Step)
       {
       case StepGetSpeedM:if(ReceiveData=="Done\r\n"){current1=lastBlurValue;Step=StepSendPoAngle;/*qDebug()<<"getcurrent1"<<current1;*/ReceiveData.clear();}break;
       case StepSendPoAngle:SendAnglePo(stepangle);Step=StepGetAngleM;qDebug()<<"SendPoAngle";Reverse_Flag=false;break;
       case StepGetAngleM:
       {
           if(ReceiveData=="PTZ GOTO FINISH")
           {
               current2=lastBlurValue;Step=StepCheckOut;ReceiveData.clear();/*qDebug()<<"getcurrent2"<<current2;*/
           }
           else if(!ReceiveData.isEmpty())
           {
              qDebug()<<"Getdata"<< ReceiveData;
           }
       }break;
       case StepCheckOut:
       {
           if(fabs(MaxBlurValue-lastBlurValue)<=MinGap)         //如果当前图片的清晰值与清晰最大值差别小于最小误差则发送停止转动指令
           {
               Mintimes++;
               if(Mintimes>0)
               {
                   if(Once_flag==false)
                   {
                       SendStop();
                       emit StepMaxDone();
                       Once_flag=true;
                       qDebug()<<"Stop";
                       CheckAngle=NoAct;
                       stepangle=5;
                       Step=StepSendSpeed;
                       qDebug()<<MaxBlurValue<<" "<<lastBlurValue;
                   }
               }
           }
           else
           {
               qDebug()<<current1<<current2<<MaxBlurValue;
               Mintimes=0;
               if(MaxBlurValue-current2<(MaxBlurValue/2))
               {
                   stepangle=stepangle>2?(stepangle/2):stepangle;
                   if(MaxBlurValue-current2<3)
                   {
                       stepangle=1;
                   }
               }
               Step=StepSendPoAngle;
//               if(current1<=current2)
//               {
//                   if(Reverse_Flag==true)
//                   {
//                       Step=StepSendReAngle;
//                   }
//                   if(Reverse_Flag==false)
//                   {
//                       if(MaxBlurValue-current2<(MaxBlurValue/2))
//                       {
//                           stepangle=stepangle>2?(stepangle/2):stepangle;
//                           if(MaxBlurValue-current2<3)
//                           {
//                               stepangle=1;
//                           }
//                       }

//                       Step=StepSendPoAngle;
//                   }
//                   current1=current2;
//               }
//               else if(current1==0.0&&current2==0.0)
//               {
//                   if(Reverse_Flag==true)
//                   {
//                       SendAngleRe(stepangle);
//                   }
//                   else if(Reverse_Flag==false)
//                   {
//                       SendAnglePo(stepangle);
//                   }
//                   Step=StepGetAngleM;
//               }
//               else if(current2<current1)
//               {
//                   if(Reverse_Flag==false)
//                   {
//                       Step=StepSendReAngle;
//                       Reverse_Flag=true;
//                   }
//                   else if(Reverse_Flag==true)
//                   {
//                       Step=StepSendPoAngle;
//                       Reverse_Flag=false;
//                   }
//                   current1=current2;
//               }
//               if((fabs(MaxBlurValue-current2)>2)&&(fabs(MaxBlurValue-current1)>2)&&           //由于环境因素导致计算失误则重新定义计算最大值
//                       (fabs(current1-current2)<0.5)&&(current1>0)&&(current2>0))
//               {
//                   times++;
//                   if(times>MistakeJude)
//                   {
//                       times=0;
//                       MaxBlurValue=(current2+current1)/2;
//                   }
//               }
//               else
//               {
//                   times=0;
//               }
           }
       }break;
       case StepSendReAngle:
       {
           SendAngleRe(stepangle);
           qDebug()<<"SendReAngle";
           Step=StepGetAngleM;
       }break;
       case StepSendSpeed:
       {
           SendSpeedMax(MediumSpeed);
           Step=StepGetSpeedM;
       }break;
       default:break;
       }
    }
}

void ptz_controller::GetArray(QByteArray data)
{
    ReceiveData=data;
}

void ptz_controller::SendStep(uint8_t *flag)         //发送一个步长
{
    static uint8_t step=0;
    if(*flag==Check)
    {
        switch(step)
        {
        case 0:SendSpeedMax(MaxSpeed);step++;qDebug()<<"CheckMax";break; //先发送最大的速度
        case 1:if(ReceiveData=="Done\r\n"){step++;ReceiveData.clear();}break;
        case 2:SendAnglePo(200);step++;MaxBlurValue=0;break;      //快速回到起始点,清晰度最大值初始化为0
        case 3:if(ReceiveData=="PTZ GOTO FINISH"){step++;ReceiveData.clear();}break;  //收到返回值进行下一步
        case 4:SendSpeedMin(MinSpeed);step++;break;   //调为最慢速度
        case 5:if(ReceiveData=="Done\r\n"){step++;ReceiveData.clear();}break;
        case 6:SendAngleRe(200);step++;break;  //反向转动
        case 7:if(ReceiveData=="PTZ GOTO FINISH"){emit CheckDone();step++;ReceiveData.clear();step=0;*flag=CheckFinished;Once_flag=false;}break;    //转动完成初始化参数，以便下次循环
        default:break;
        }
    }
}

