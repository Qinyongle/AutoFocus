#include "imagecapture.h"
#include <QGuiApplication>
#include <QApplication>
#include <QString>
#include <QFileDialog>
#include <webapi/webapi.h>
  int W_Posx=0,W_Posy=0,W_Width=0,W_Height=0;
  QString Ip_temp;
ImageCapture::ImageCapture(QWidget *parent) : QWidget(parent)
{

}

void ImageCapture::SendCaptureSignal()
{
    emit capture_trigg();
}

void ImageCapture::capture()
{
  //  QString filename= QApplication::applicationDirPath()+"capture"+".jpg";
   // QString fileName = QFileDialog::getSaveFileName(this, tr("保存截图"), QDir::currentPath(), tr("image(*.png)"));
    screen=QGuiApplication::primaryScreen();
    screen->grabWindow(0,W_Posx,W_Posy,W_Width,W_Height).save("capture.jpg","jpg");
//    qDebug()<<"W_Posx:"<<W_Posx<<W_Posy<<W_Width<<W_Height;
//    screen->grabWindow(0,W_Posx,0,1280,720).save("capture.jpg","jpg");
}
