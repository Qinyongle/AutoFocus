#include "webapi.h"

#include <QMessageBox>

WebApi::WebApi(QObject *parent) : QObject(parent)
{

}

QByteArray WebApi::Get(const QString &strUrl)
{
    QNetworkAccessManager m_qnam;
//    assert(!strUrl.isEmpty());

    const QUrl url = QUrl::fromUserInput(strUrl);
//    assert(url.isValid());

    QNetworkRequest qnr(url);
    QNetworkReply* reply = m_qnam.get(qnr); //m_qnam是QNetworkAccessManager对象

    QEventLoop eventLoop;
    connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec(QEventLoop::ExcludeUserInputEvents);

    QByteArray replyData = reply->readAll();
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QVariant redirectAttr = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (reply->error()
        || 300 == statusCode //状态码300 Multiple Choices，既不是错误也不算重定向，应该是qt bug
        || !redirectAttr.isNull())
    {
        QString errString = reply->error() ? reply->errorString() : QString("Redirection occurred(%1),not allowd that").arg(statusCode);
        QMessageBox::critical(nullptr, "net error",
            QString("send get request error\nURL: %1\nerror masage: %2").arg(reply->request().url().toDisplayString(), errString));
        replyData.clear();
    }

    reply->deleteLater();
    reply = nullptr;

    return replyData;
}

QByteArray WebApi::Post(const QString &strUrl, QByteArray data)
{
    QNetworkAccessManager m_qnam;
//    assert(!strUrl.isEmpty());

    const QUrl url = QUrl::fromUserInput(strUrl);
//    assert(url.isValid());

    QNetworkRequest qnr(url);
    qnr.setHeader(QNetworkRequest::ContentTypeHeader, "application/json;charset=utf-8");
    //qnr.setRawHeader("Content-Type", "application/json; charset=utf-8");


    qDebug() << data;
    QNetworkReply* reply = m_qnam.post(qnr, data); //m_qnam是QNetworkAccessManager对象

    QEventLoop eventLoop;
    connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec(QEventLoop::ExcludeUserInputEvents);

    QByteArray replyData = reply->readAll();
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QVariant redirectAttr = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (reply->error()
        || 300 == statusCode //状态码300 Multiple Choices，既不是错误也不算重定向，应该是qt bug
        || !redirectAttr.isNull())
    {
        QString errString = reply->error() ? reply->errorString() : QString("Redirection occurred(%1),not allowd that").arg(statusCode);
        QMessageBox::critical(nullptr, "net error",
            QString("send get request error\nURL: %1\nerror masage: %2").arg(reply->request().url().toDisplayString(), errString));
        replyData.clear();
    }

    reply->deleteLater();
    reply = nullptr;

    return replyData;
}
