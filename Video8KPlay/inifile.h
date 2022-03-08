#ifndef INIFILE_H
#define INIFILE_H

#include<QString>
#include<QVariant>
#include <QMutex>
#include <QVector>
#include <QSettings>

class IniFile
{
public:

    void WriteIniFile();
    void ReadIniFile();
    void WriteKeyValue(QString Key, int KeyValue);
    void WriteKeyValue(QString Key, double KeyValue);
    void WriteKeyValue(QString Key, QVariant KeyValue);
    QString ReadKeyValue(QString Key);

    static IniFile* getConfig()
    {
        static QMutex logmutex;
        if(m_pConfig == nullptr)
        {
            QMutexLocker locker(&logmutex);
            if(m_pConfig == nullptr)
            {
                m_pConfig = new IniFile();
            }
        }
        return m_pConfig;
    }
    static void Release()                    //必须，否则会导致内存泄露
    {
        if(nullptr != m_pConfig)
        {
            delete m_pConfig;
            m_pConfig = nullptr;
        }
    }
    void shellcmd();
    QString DecodeType() {return m_decodeType;}
    QString SaveVideoPath() {return m_path;}


    void DecodeType(QString type) {m_decodeType = type;}
    void SaveVideoPath(QString path) {m_path = path;}



private:
    QSettings *m_pMainConfig;
    QString m_strFileState;
    // decode type
    QString m_decodeType;
    QString m_path;

private:
    IniFile();
    ~IniFile();
    static IniFile* m_pConfig;
};
#endif // INIFILE_H
