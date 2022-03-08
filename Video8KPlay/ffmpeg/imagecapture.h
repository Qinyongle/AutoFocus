#ifndef IMAGECAPTURE_H
#define IMAGECAPTURE_H
#include <iostream>
#include <QDebug>
#include <QWidget>
#include <QPixmap>
#include <QWindow>
#include <QScreen>
 extern int W_Posx,W_Posy,W_Width,W_Height;
 extern QString Ip_temp;
namespace Ui {
class ImageCapture;
}
class ImageCapture : public QWidget
{
    Q_OBJECT
public:
    explicit ImageCapture(QWidget *parent = nullptr);
    void SendCaptureSignal();

signals:
    void capture_trigg();
public slots:
    void capture();
private:
    QScreen *screen;

};

#endif // IMAGECAPTURE_H
