#include "inifile.h"
#include <QSettings>
#include <QCoreApplication>
#include <QProcess>
#include <QDir>
#include <QDebug>
#include <QProcess>
#include <QFile>
#include <QTextStream>

IniFile* IniFile::m_pConfig = nullptr;
IniFile::IniFile()
{
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    bool bConfigOk = false;
    QFile file("config.ini");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        shellcmd();
    }
    else
    {

        while(!file.atEnd())
        {
            QByteArray line = file.readLine();
            if(line.contains("configState=ok"))
            {
                bConfigOk = true;
                break;
            }
        }
        file.close();
    }

    if(!bConfigOk)
    {
        shellcmd();
    }
    //前面配置文件没有配置configState=ok---运行了shellcmd()--删除了配置文件
    m_pMainConfig = new QSettings("config.ini", QSettings::IniFormat); //这里是读取配置文件
    ReadIniFile();
}
void IniFile::shellcmd()
{
    //just for linux
//#ifdef _LINUX
    {
        QProcess::execute("rm -rf config.ini");
//        QProcess::execute("cp .config.ini config.ini"); //复制指令.在这里没必要，前面删除了无法复制
    }
//#endif
}

IniFile::~IniFile()
{
    WriteIniFile();
    delete m_pMainConfig;
}

void IniFile::WriteIniFile()
{
    WriteKeyValue("Decode/Type",m_decodeType);
    WriteKeyValue("Decode/savePath",m_path);
}

void IniFile::ReadIniFile()
{
    m_decodeType = ReadKeyValue("Decode/Type");
    m_path = ReadKeyValue("Decode/savePath");}

void IniFile::WriteKeyValue(QString Key, int KeyValue)
{
    m_pMainConfig->setValue(Key, KeyValue);
}
void IniFile::WriteKeyValue(QString Key, double KeyValue)
{
    m_pMainConfig->setValue(Key, KeyValue);
}
void IniFile::WriteKeyValue(QString Key, QVariant KeyValue)
{
    m_pMainConfig->setValue(Key, KeyValue);
}

QString IniFile::ReadKeyValue(QString Key)
{
    QString val = m_pMainConfig->value(Key).toString();
    if(val.isEmpty())
    {
        WriteKeyValue(Key,"");
    }
    return val;
}
