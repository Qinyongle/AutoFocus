#include "testprogram.h"
#include <QDebug>
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QTextCodec>
#include <QPixmap>
#include <QPainter>
#include <QPen>
#include <QFont>
#include <QDateTime>

TestProgram::TestProgram(QObject *parent)
{
    InitThread();
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
}

TestProgram::~TestProgram()
{
    m_thread->quit();
    m_thread->wait();
    m_gpsGetThread ? m_gpsGetThread->quit(), m_gpsGetThread->wait() : bool();
}

void TestProgram::GetPicPath(QString path)
{
    m_picPath = path;
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    qDebug() << "send ";
    //我的图片文件
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    // pic
    QImage image = QPixmap(m_picPath).toImage();
    QPainter painter(&image);
    QPen pen = painter.pen();
    pen.setColor(Qt::white);
    QFont font = painter.font();
    font.setBold(true);
    font.setPixelSize(image.height() / 45);
    painter.setPen(pen);
    painter.setFont(font);
    auto currentTime = QDateTime::currentDateTime()
            .toString("yyyy-MM-dd hh:mm:ss.zz");
    auto lonStr = QString::number(m_gpsData.lng, 'f', 8);
    auto latStr = QString::number(m_gpsData.lat, 'f', 8);
    auto heightStr = QString::number(m_gpsData.alt, 'f', 2);
    auto angleStr = QString::number(m_gpsData.heading, 'f', 2);
    auto text = currentTime + "   " + lonStr + "   " + latStr
            + "   H:" + heightStr + "M    DEG:" + angleStr;
    painter.drawText(QPoint(10,image.height() - 20), text);
    image.save(m_picPath);
    auto file = new QFile(m_picPath);
    if(!file->open(QIODevice::ReadOnly)) {
        qDebug() << "[ERROR]: file open failed" << m_picPath;
    }
    //imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
    QHttpPart imagePart;
    QString filename = m_picPath;
    QFileInfo fileInfo(m_picPath);
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/jpeg"));
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(QString("form-data; name=\"%1\"; filename=\"%2\";").arg("file").arg(filename)));
    imagePart.setBodyDevice(file);
    file->setParent(multiPart);
    multiPart->append(imagePart);

    // lon
    QHttpPart lonPart;
    lonPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"lon\""));
    lonPart.setBody(lonStr.toUtf8());
    multiPart->append(lonPart);
    // lat
    QHttpPart latPart;
    latPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"lat\""));
    latPart.setBody(latStr.toUtf8());
    multiPart->append(latPart);
    // time
    QHttpPart timePart;
    timePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"time\""));
    timePart.setBody(currentTime.toUtf8());
    multiPart->append(timePart);
    // height
    QHttpPart heightPart;
    heightPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"height\""));
    heightPart.setBody(heightStr.toUtf8());
    multiPart->append(heightPart);
    // rotationAngle
    QHttpPart rotationAnglePart;
    rotationAnglePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"rotationAngle\""));
    rotationAnglePart.setBody(angleStr.toUtf8());
    multiPart->append(rotationAnglePart);

    //此处设置上传的url
    QNetworkRequest request(QUrl(m_serviceUrl.toUtf8()));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data; boundary=" + multiPart->boundary());
    auto reply = manager->post(request, multiPart);
    QEventLoop eventLoop;
    connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();
}

void TestProgram::SendPicTimer()
{
    emit GetPicCmd();
}

void TestProgram::LoadThread()
{
    if(m_sendTimeInterval == 0) {
        return;
    }
    emit SetSavePathNameCmd(m_savePath, m_saveName);
    if(m_decodeType == "qsv") {
        emit RunStartCmd(m_filePath, TestFfmpegDecoder::QSV);
    } else {
        emit RunStartCmd(m_filePath, TestFfmpegDecoder::CUDA);
    }

    emit StartTimer(m_sendTimeInterval);

    if(!m_tcpIp.isEmpty()) {
        emit ConnectGps(m_tcpIp, m_tcpPort);
    }
    emit GetPicCmd();
}

void TestProgram::SetSendTimeInterval(int interval)
{
    m_sendTimeInterval = interval;
}

void TestProgram::SetDecodeType(QString type)
{
    m_decodeType = type;
}

void TestProgram::SetServiceUrl(QString url)
{
    m_serviceUrl = url;
}

void TestProgram::SetFilePath(QString path)
{
    m_filePath = path;
    auto pathSplit = path.split("/");
    foreach(auto i , pathSplit) {
        if(i.contains(".")) {
            m_cameraIp = i;
            break;
        }
    }
}

void TestProgram::SetGpsTcp(QString ip, int port)
{
    m_tcpIp = ip;
    m_tcpPort = port;
}

void TestProgram::SetSavePathName(QString path, QString name)
{
    if(path.back() != "/") {
        m_savePath = path.append("/");
    }

    m_saveName = name;
}

bool TestProgram::Set8KCamera()
{
    if(m_cameraIp.isEmpty()) return false;
    if(!RetWebApiState(WebApi::Get(m_cameraIp + ENCODETYPE + "h265"))) return false;
    if(!RetWebApiState(WebApi::Get(m_cameraIp + BITWIDTH + "8"))) return false;
    if(!RetWebApiState(WebApi::Get(m_cameraIp + RESOLUTION7680))) return false;
    if(!RetWebApiState(WebApi::Get(m_cameraIp + BITRATE + "100000000"))) return false;
    return true;
}

void TestProgram::replyFinished(QNetworkReply * reply)
{
    variant = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if (variant.isValid()) {
        qDebug() << variant.toInt();
    }
    //2、获取响应头
    variant = reply->header(QNetworkRequest::ContentTypeHeader);
    if (variant.isValid())
        qDebug() << variant.toString();

    //3、获取响应信息
    QByteArray bytes = reply->readAll();
    qDebug() << "bytes = " << bytes ;
//    ui->textBrowser->append(bytes);//GET /ctrl/af?action=update_roi_center&x=500&y=500

    // 对焦
    static int i = 0;
    if(i++ >= 6) {
        i = 0;
        //WebApi::Get(m_cameraIp + FOCUSCENTER);
    }

}

void TestProgram::httpUploadFinished()
{
    if(m_reply->error() == QNetworkReply::NoError) {
     qDebug()<<"upload file finished";
     m_file->flush();
     m_file->close();
     m_reply->deleteLater();
     m_reply = NULL;
     delete m_file;
     m_file = NULL;
    } else {
         qDebug()<<"error string:"<<m_reply->errorString();
         m_reply->deleteLater();
    }
}

void TestProgram::InitThread()
{
    m_ffmpeg = new VideoPicGet(nullptr);
    m_thread = new QThread();
    m_ffmpeg->moveToThread(m_thread);
    connect(m_thread, &QThread::finished, m_thread, &QObject::deleteLater);
    connect(this, SIGNAL(RunStartCmd(QString, TestFfmpegDecoder::DECODE_TYPE)),
            m_ffmpeg, SLOT(RunStart(QString, TestFfmpegDecoder::DECODE_TYPE)));
    connect(this, SIGNAL(RunStopCmd()),
            m_ffmpeg, SLOT(RunStop()), Qt::DirectConnection);
    connect(this, SIGNAL(GetPicCmd()),
            m_ffmpeg, SLOT(GetPicCmd()), Qt::DirectConnection);
    connect(this, SIGNAL(SetSavePathNameCmd(QString, QString)),
            m_ffmpeg, SLOT(SetSavePath(QString, QString)), Qt::DirectConnection);
    connect(m_ffmpeg->GetFrameSignalPoint(), SIGNAL(SendPicPath(QString)),
            this, SLOT(GetPicPath(QString)));
    m_thread->start();


    m_sendTimer = new QTimer();
    m_sendTimerThread = new QThread();
    m_sendTimer->moveToThread(m_sendTimerThread);
    connect(m_sendTimerThread, &QThread::finished, m_sendTimer, &QObject::deleteLater);
    connect(this, SIGNAL(StartTimer(int)), m_sendTimer, SLOT(start(int)));
    connect(this, SIGNAL(StopTimer()), m_sendTimer, SLOT(stop()));
    connect(m_sendTimer, SIGNAL(timeout()), this, SLOT(SendPicTimer()));
    m_sendTimerThread->start();

    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(replyFinished(QNetworkReply *)));
//    connect(uploadFileReply,SIGNAL(finished()),this,SLOT(httpUploadFinished()));
    //connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(replyFinished(QNetworkReply *)));

    // gps
    m_gpsGet = new GpsGetThread;
    m_gpsGetThread = new QThread;
    m_gpsGet->moveToThread(m_gpsGetThread);
    connect(m_gpsGetThread, &QThread::finished, m_gpsGet, &QObject::deleteLater);
    connect(this, SIGNAL(ConnectGps(QString, int)),
            m_gpsGet, SLOT(Connect(QString, int)));
    connect(m_gpsGet, &GpsGetThread::GpsPotData,
            this, [=] (GpsData data) {
        m_gpsData = data;
    });
    m_gpsGetThread->start();
}

bool TestProgram::RetWebApiState(QByteArray data)
{
    QJsonDocument document = QJsonDocument::fromJson(data);
    QJsonObject obj = document.object();
    if(obj.value("code").toInt() < 0) {
        return false;
    } else {
        return true;
    }
}
