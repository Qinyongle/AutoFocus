#ifndef WEBAPI_H
#define WEBAPI_H

#include <QObject>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>

class WebApi : public QObject
{
    Q_OBJECT
public:
    explicit WebApi(QObject *parent = nullptr);
    static QByteArray Get(const QString &strUrl);
    static QByteArray Post(const QString &strUrl, QByteArray data);
signals:

private:
    //static QNetworkAccessManager m_qnam;
};

#endif // WEBAPI_H
