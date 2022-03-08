#ifndef CAMERACONFIGWIDGET_H
#define CAMERACONFIGWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QLayout>
#include <QPushButton>
#include <QComboBox>
#include "webapi/webapi.h"
class CameraConfigWidget : public QWidget
{
    Q_OBJECT
#define GETCONFIG "/ctrl/stream_setting?index=stream1&action=query"
#define BITWIDTH "/ctrl/stream_setting?index=stream1&bitwidth="
#define BITRATE "/ctrl/stream_setting?index=stream1&bitrate="
#define ENCODETYPE "/ctrl/stream_setting?index=stream1&venc="
#define RESOLUTION7680 "/ctrl/stream_setting?index=stream1&width=7680&height=4320"
#define RESOLUTION4096 "/ctrl/stream_setting?index=stream1&width=4096&height=2160"
#define RESOLUTION1920 "/ctrl/stream_setting?index=stream1&width=1920&height=1080"

public:
    explicit CameraConfigWidget(QWidget *parent = nullptr);

signals:

private slots:
    void LinkClicked();
    void ChangeClicked();


private:
    void InitUi();
    void InitConfig();
    bool RetWebApiState(QByteArray);
private:
    QLineEdit *m_ipLine = nullptr;
    QPushButton *m_LinkButton = nullptr;

    QComboBox *m_encodeType = nullptr;    // 编码格式
    QComboBox *m_resolution = nullptr;    // 分辨率
    QComboBox *m_bitPo = nullptr;         // bit位
    QLineEdit *m_bitRate = nullptr;       // 码率

    QPushButton *m_changeButton;

private:
    bool m_linkState = false;
    QString m_ipStr;
};

#endif // CAMERACONFIGWIDGET_H
