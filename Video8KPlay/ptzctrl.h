#ifndef PTZCTRL_H
#define PTZCTRL_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QThread>
#include "threads/ptzthreads.h"
#include <QRadioButton>

class PtzCtrl : public QWidget
{
    Q_OBJECT
public:
    explicit PtzCtrl(QWidget *parent = nullptr);
    ~PtzCtrl();

signals:
    void TcpRun();
    void TcpQuit();

    void RRotate();
    void LRotate();
    void HAngleSet(double);
    void URotate();
    void DRotate();
    void VAngleSet(double);
    void RotateStop();
    void ZIn();
    void ZOut();
    void ZSet(double);
    void ZStop();

    void CapturePic();
    void StopCapturePic();
    void SendAngle();
    void CheckDonefromPtzThread();
    void StepMaxDonefromPtzThread();

public slots:


protected:
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);

private:
    void InitUi();
    void InitThread();
    void ConnectStateUpdate();

private:
    QLabel *m_stateShow = nullptr;
    QPushButton *m_hLRotate = nullptr;
    QPushButton *m_hRRotate = nullptr;
    QPushButton *m_vURotate = nullptr;
    QPushButton *m_vDRotate = nullptr;
    QPushButton *m_zIRotate = nullptr;
    QPushButton *m_zORotate = nullptr;
    QRadioButton *m_Focus    = nullptr;

    QLineEdit *m_hStateShow = nullptr;
    QLineEdit *m_vStateShow = nullptr;
    QLineEdit *m_zoomStateShow = nullptr;

    QLineEdit *m_hAngleSet = nullptr;
    QLineEdit *m_vAngleSet = nullptr;
    QLineEdit *m_zAngleSet = nullptr;
    QPushButton *m_hAngleSetButton = nullptr;
    QPushButton *m_vAngleSetButton = nullptr;
    QPushButton *m_zAngleSetButton = nullptr;


private:
    bool m_hvDevConnected = false;
    bool m_zoomDevConnected = false;

    PtzThreads *m_ptz = nullptr;
    QThread *m_ptzThread = nullptr;

};

#endif // PTZCTRL_H
