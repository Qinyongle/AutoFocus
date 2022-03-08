#include "streamconfigwidget.h"
#include <QDir>
#include <QSettings>
#include <QCoreApplication>
StreamConfigWidget::StreamConfigWidget(QWidget *parent) : QWidget(parent)
{
    InitUi();
    InitConfig();
}


void StreamConfigWidget::InitUi()
{
    QLabel *urlLable = new QLabel(QString::fromLocal8Bit("URL"));
    m_urlLine = new QLineEdit();
    m_urlLine->setText("rtsp://");
    QHBoxLayout *urlLayout = new QHBoxLayout();
    urlLayout->addWidget(urlLable);
    urlLayout->addWidget(m_urlLine);
    urlLayout->setStretchFactor(urlLable, 1);
    urlLayout->setStretchFactor(m_urlLine, 3);

    m_okBut = new QPushButton(QString::fromLocal8Bit("确定"));
    connect(m_okBut, SIGNAL(clicked()), this, SLOT(OkButClicked()));
    m_cancelBut = new QPushButton(QString::fromLocal8Bit("取消"));
    connect(m_cancelBut, SIGNAL(clicked()), this, SLOT(CancelButClicked()));
    QHBoxLayout *butLayout = new QHBoxLayout();
    butLayout->addStretch();
    butLayout->addWidget(m_okBut);
    butLayout->addStretch();
    butLayout->addWidget(m_cancelBut);
    butLayout->addStretch();
    butLayout->setStretchFactor(m_okBut, 1);
    butLayout->setStretchFactor(m_cancelBut, 1);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(urlLayout);
    mainLayout->addLayout(butLayout);

    this->setLayout(mainLayout);


}

void StreamConfigWidget::InitConfig()
{
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    QSettings *config = new QSettings("./config.ini", QSettings::IniFormat);
    QString url = config->value("SYS/Url").toString();
    if(url.isEmpty()) {
        config->setValue("SYS/Url", "");
    } else {
        m_urlLine->setText(url);
    }
}

void StreamConfigWidget::OkButClicked()
{
    QString url = m_urlLine->text();
    emit SendStreamUrl(url);
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    QSettings *config = new QSettings("./config.ini", QSettings::IniFormat);
    config->setValue("SYS/Url", url);

    this->hide();
}

void StreamConfigWidget::CancelButClicked()
{
    this->hide();
}
