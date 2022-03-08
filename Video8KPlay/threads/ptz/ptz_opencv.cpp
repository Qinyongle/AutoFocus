#include "ptz_opencv.h"
#include <stdint.h>
#include <QDebug>
volatile double MaxBlurValue=0,lastBlurValue=0;
volatile double MaxValueFlag=0;
PtzOpencv::PtzOpencv()
{

}

void PtzOpencv::SignalToOpencv()
{
    emit imageReady();
}

void PtzOpencv::OpenCvLaplacian()
{
static uint32_t times=0;
static uint8_t CheckStep=0;
static double Temp_max=0;
    cv::Mat imageSource = cv::imread("./capture.jpg");
    if(imageSource.data==0)
    {
       qDebug("损坏\r\n");
    }
    else
    {
        cv::Mat imageGrey;
        cv::cvtColor(imageSource,imageGrey,CV_BGR2GRAY);    //#include <opencv2\imgproc\types_c.h>解决找不到CV_RGB2GRAY的问题
        cv::Mat imageSobel;
        cv::Mat  cany;
        cv::Canny(imageGrey,imageSobel,50,200);
//        cv::imshow("canny",imageSobel);
//        cv::waitKey(0);
//        cv::destroyAllWindows();
       // cv::Sobel(imageGrey,imageSobel,CV_16U,1,11);
        //cv::Laplacian(imageGrey,imageSobel,CV_8UC1,5,1.0,0.0,cv::BORDER_REFLECT101);
        lastBlurValue=mean(imageSobel)[0];
        BlurValue.enqueue((const double)lastBlurValue);   //值入队
        qDebug()<<MaxBlurValue<<lastBlurValue;

        if(fabs(MaxBlurValue-lastBlurValue)>5||MaxBlurValue==0.0)           //当检测到最大清晰度值与当前检测的清晰值差为5以上表明更换了拍摄场景
        {
            if(times>20)            //添加判断次数，防止因为短暂的噪声干扰导致误动作
            {
                switch(CheckStep)
                {
                case 0:emit SendAngle();CheckStep++;qDebug()<<"sendAngleSignal";break;  //发送执行一个步长的命令
                case 1:
                {
                    if(lastBlurValue>Temp_max)
                    {
                        AverQueue.enqueue(BlurValue.last());    //最大值入栈
                        Average(&Temp_max);                 //计算均值
                    }
                    if(CheckDone==1)
                    {
                        qDebug()<<"CheckDone";
                        CheckStep=2;
                        MaxBlurValue=Temp_max;
                        qDebug()<<"Max="<<MaxBlurValue;
                        CheckDone=0;
                        Temp_max=0;
                    }
                }break;
                case 2:
                {
                    if(StepMaxDone==1)
                    {
                        CheckStep=0;
                        StepMaxDone=0;
                        times=0;
                        qDebug()<<"StepMaxDone";
                    }
                }break;
                default:break;
                }
            }
            else
            {
                times++;
            }
        }
        else
        {
            times=0;
        }
          if(BlurValue.size()>=BLURMAXQUEUE)
            {
                BlurValue.clear();      //当队列超过一定数目，清空队列
            }
    }
}



void PtzOpencv::Average(double *data)
{
    double sum=0;
    if(AverQueue.size()==AVERMAXQUEUE)
    {
        AverQueue.dequeue();
    }
    for(int i=0;i<AverQueue.size();i++)
    {
        sum=sum+AverQueue[i];
    }
       *data = sum/static_cast<double>(AverQueue.size());
}
