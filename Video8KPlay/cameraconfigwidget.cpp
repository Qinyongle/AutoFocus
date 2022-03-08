#include "cameraconfigwidget.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QDir>
#include <QSettings>
#include <QCoreApplication>
CameraConfigWidget::CameraConfigWidget(QWidget *parent) : QWidget(parent)
{
    InitUi();
    InitConfig();
}


void CameraConfigWidget::InitUi()
{
    QLabel *warningLable = new QLabel(QString::fromLocal8Bit("配置前请关闭此IP串流，否则会配置失败"));
    warningLable->setMaximumHeight(15);
    QLabel *ipLable = new QLabel(QString::fromLocal8Bit("IP："));
    m_ipLine = new QLineEdit();
    m_LinkButton = new QPushButton(QString::fromLocal8Bit("连接"));
    connect(m_LinkButton, SIGNAL(clicked()), this, SLOT(LinkClicked()));
    QHBoxLayout *ipLayout = new QHBoxLayout();
    ipLayout->addWidget(ipLable);
    ipLayout->addWidget(m_ipLine);
    ipLayout->addStretch();
    ipLayout->addWidget(m_LinkButton);
    ipLayout->setStretchFactor(ipLable, 1);
    ipLayout->setStretchFactor(m_ipLine, 3);
    ipLayout->setStretchFactor(m_LinkButton, 2);

    QLabel *encodeLable = new QLabel(QString::fromLocal8Bit(("编码格式：")));
    m_encodeType = new QComboBox();
    m_encodeType->insertItem(0, "");
    m_encodeType->insertItem(1, "h264");
    m_encodeType->insertItem(2, "h265");
    QLabel *resoluLable = new QLabel(QString::fromLocal8Bit("分辨率："));
    m_resolution = new QComboBox();
    m_resolution->insertItem(0, "");
    m_resolution->insertItem(1, "1920 x 1080");
    m_resolution->insertItem(2, "4096 x 2160");
    m_resolution->insertItem(3, "7680 x 4320");
    QHBoxLayout *enReLayout = new QHBoxLayout();
    enReLayout->addWidget(encodeLable);
    enReLayout->addWidget(m_encodeType);
    enReLayout->addStretch();
    enReLayout->addWidget(resoluLable);
    enReLayout->addWidget(m_resolution);
    enReLayout->setStretchFactor(encodeLable, 1);
    enReLayout->setStretchFactor(m_encodeType, 2);
    enReLayout->setStretchFactor(resoluLable, 1);
    enReLayout->setStretchFactor(m_resolution, 2);

    QLabel *bitPoLable = new QLabel(QString::fromLocal8Bit("bit位宽："));
    m_bitPo = new QComboBox();
    m_bitPo->insertItem(0, "");
    m_bitPo->insertItem(1, "8");
    m_bitPo->insertItem(2, "10");
    QLabel *bitRateLable = new QLabel(QString::fromLocal8Bit("码率(最大为400 000)："));
    m_bitRate = new QLineEdit();
    QHBoxLayout *bitLayout = new QHBoxLayout();
    bitLayout->addWidget(bitPoLable);
    bitLayout->addWidget(m_bitPo);
    bitLayout->addStretch();
    bitLayout->addWidget(bitRateLable);
    bitLayout->addWidget(m_bitRate);
    bitLayout->setStretchFactor(bitPoLable, 1);
    bitLayout->setStretchFactor(m_bitPo, 2);
    bitLayout->setStretchFactor(bitRateLable, 1);
    bitLayout->setStretchFactor(m_bitRate, 2);

    m_changeButton = new QPushButton(QString::fromLocal8Bit("修改"));
    connect(m_changeButton, SIGNAL(clicked()), this, SLOT(ChangeClicked()));

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(warningLable);
    mainLayout->addLayout(ipLayout);
    mainLayout->addLayout(enReLayout);
    mainLayout->addLayout(bitLayout);
    mainLayout->addWidget(m_changeButton);
    mainLayout->setStretchFactor(warningLable, 1);
    mainLayout->setStretchFactor(ipLayout, 1);
    mainLayout->setStretchFactor(enReLayout, 1);
    mainLayout->setStretchFactor(bitLayout, 1);
    mainLayout->setStretchFactor(m_changeButton, 1);

    this->setLayout(mainLayout);
}

void CameraConfigWidget::InitConfig()
{
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    QSettings *config = new QSettings("./config.ini", QSettings::IniFormat);
    m_ipStr = config->value("SYS/CameraIp").toString();
    if(m_ipStr.isEmpty()) {
        config->setValue("SYS/CameraIp", "");
    } else {
        m_ipLine->setText(m_ipStr);
    }
}

void CameraConfigWidget::LinkClicked()
{
    m_ipStr = m_ipLine->text();
    QByteArray data = WebApi::Get(m_ipStr + GETCONFIG);
    if(data.contains("streamIndex")) {
        QJsonDocument document = QJsonDocument::fromJson(data);
        QJsonObject obj = document.object();
        m_encodeType->setCurrentIndex(obj.value("encoderType") == "h264" ? 1 : 2);
        m_bitPo->setCurrentIndex(obj.value("bitwidth") == "8bit" ? 1 : 2);
        m_bitRate->setText(QString::number(obj.value("bitrate").toInt()));
        int width = obj.value("width").toInt();
        if(width == 1920) {
            m_resolution->setCurrentIndex(1);
        } else if(width == 4096) {
            m_resolution->setCurrentIndex(2);
        } else if(width == 7680) {
            m_resolution->setCurrentIndex(3);
        }
        QDir::setCurrent(QCoreApplication::applicationDirPath());
        QSettings *config = new QSettings("./config.ini", QSettings::IniFormat);
        config->setValue("SYS/CameraIp", m_ipStr);
    } else {

    }
}

void CameraConfigWidget::ChangeClicked()
{
    QString maxBitRateStr;
    if(m_encodeType->currentIndex() == 1) {
        if(!RetWebApiState(WebApi::Get(m_ipStr + ENCODETYPE + "h264"))) goto ERRORAPI;
    } if(m_encodeType->currentIndex() == 2) {
        if(!RetWebApiState(WebApi::Get(m_ipStr + ENCODETYPE + "h265"))) goto ERRORAPI;
    }

    if(m_bitPo->currentIndex() == 1) {
        if(!RetWebApiState(WebApi::Get(m_ipStr + BITWIDTH + "8"))) goto ERRORAPI;
    } else if(m_bitPo->currentIndex() == 2) {
        if(!RetWebApiState(WebApi::Get(m_ipStr + BITWIDTH + "10"))) goto ERRORAPI;
    }

    if(m_resolution->currentIndex() == 1) {
        if(!RetWebApiState(WebApi::Get(m_ipStr + RESOLUTION1920))) goto ERRORAPI;
    } else if(m_resolution->currentIndex() == 2) {
        if(!RetWebApiState(WebApi::Get(m_ipStr + RESOLUTION4096))) goto ERRORAPI;
    } else if(m_resolution->currentIndex() == 3) {
        if(!RetWebApiState(WebApi::Get(m_ipStr + RESOLUTION7680))) goto ERRORAPI;
    }

    if(m_bitRate->text().toLongLong() > 400000) {
        maxBitRateStr = "400000000";
    } else {
        maxBitRateStr = QString::number(m_bitRate->text().toLongLong() * 1000);
    }
    if(!RetWebApiState(WebApi::Get(m_ipStr + BITRATE + maxBitRateStr)))
        goto ERRORAPI;

    LinkClicked();
    QMessageBox::information(nullptr, QString::fromLocal8Bit("成功"),QString::fromLocal8Bit("配置成功"));
    return;
    ERRORAPI:
    qDebug() << "ERRORAPI";
    QMessageBox::critical(nullptr, QString::fromLocal8Bit("异常"),QString::fromLocal8Bit("配置失败"));
    LinkClicked();
    return;
}

bool CameraConfigWidget::RetWebApiState(QByteArray data)
{
    QJsonDocument document = QJsonDocument::fromJson(data);
    QJsonObject obj = document.object();
    if(obj.value("code").toInt() < 0) {
        return false;
    } else {
        return true;
    }
}
