#include "ptzconfig.h"
#include <QSettings>
#include <QMessageBox>
PtzConfig::PtzConfig(QWidget *parent) : QWidget(parent)
{

    InitUi();
}


void PtzConfig::InitUi()
{
    this->setWindowTitle(QString::fromLocal8Bit("云台配置"));
    auto rlIpLable = new QLabel(QString::fromLocal8Bit("云台IP："));
    m_rlIpEdit = new QLineEdit;
    auto rlPortLable = new QLabel(QString::fromLocal8Bit("云台端口："));
    m_rlPortEdit = new QLineEdit;

    auto ptzHLayout = new QHBoxLayout;
    ptzHLayout->addWidget(rlIpLable);
    ptzHLayout->addWidget(m_rlIpEdit);
    ptzHLayout->addWidget(rlPortLable);
    ptzHLayout->addWidget(m_rlPortEdit);


    auto zoomIpLable = new QLabel(QString::fromLocal8Bit("缩放IP"));
    m_zoomIpEdit = new QLineEdit;
    auto zoomPortLable = new QLabel(QString::fromLocal8Bit("缩放端口"));
    m_zoomPortEdit = new QLineEdit;

    auto zoomHLayout = new QHBoxLayout;
    zoomHLayout->addWidget(zoomIpLable);
    zoomHLayout->addWidget(m_zoomIpEdit);
    zoomHLayout->addWidget(zoomPortLable);
    zoomHLayout->addWidget(m_zoomPortEdit);

    m_overButton = new QPushButton(QString::fromLocal8Bit("确定"));
    connect(m_overButton, SIGNAL(clicked()), this, SLOT(OverButtonClicked()));
    m_cancelButton = new QPushButton(QString::fromLocal8Bit("取消"));
    connect(m_cancelButton, &QPushButton::clicked, this, [=] {
        this->hide();
    });
    auto buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(m_overButton);
    buttonLayout->addWidget(m_cancelButton);


    auto mainLayout = new QVBoxLayout;
    mainLayout->addLayout(ptzHLayout);
//    mainLayout->addLayout(zoomHLayout);
    mainLayout->addLayout(buttonLayout);

    this->setLayout(mainLayout);
}


void PtzConfig::InitConfig()
{
    auto config = new QSettings("./config.ini", QSettings::IniFormat);
    m_rlIpEdit->setText(config->value("PTZ/PtzIp").toString());
    m_rlPortEdit->setText(config->value("PTZ/PtzPort").toString());
    m_zoomIpEdit->setText(config->value("PTZ/ZoomIp").toString());
    m_zoomPortEdit->setText(config->value("PTZ/ZoomPort").toString());

}

void PtzConfig::showEvent(QShowEvent *event)
{
    InitConfig();
    QWidget::showEvent(event);
}

void PtzConfig::OverButtonClicked()
{
    auto config = new QSettings("./config.ini", QSettings::IniFormat);
    config->setValue("PTZ/PtzIp", m_rlIpEdit->text());
    config->setValue("PTZ/PtzPort", m_rlPortEdit->text());
    config->setValue("PTZ/ZoomIp", m_zoomIpEdit->text());
    config->setValue("PTZ/ZoomPort", m_zoomPortEdit->text());
    InitConfig();
    QMessageBox::information(nullptr, QString::fromLocal8Bit("成功"),QString::fromLocal8Bit("配置成功"));
}
