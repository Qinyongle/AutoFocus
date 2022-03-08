#include "mainwindow.h"

#include <QApplication>
#include <QFile>

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // 加锁
    static QMutex mutex;
    mutex.lock();

    QByteArray localMsg = msg.toLocal8Bit();

    QString strMsg("");
    switch(type)
    {
    case QtDebugMsg:
        strMsg = QString("Debug:");
        break;
    case QtWarningMsg:
        strMsg = QString("Warning:");
        break;
    case QtCriticalMsg:
        strMsg = QString("Critical:");
        break;
    case QtFatalMsg:
        strMsg = QString("Fatal:");
        break;
    }

    // 设置输出信息格式
//    QString strDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
//    QString strMessage = QString("Message:%1 File:%2  Line:%3  Function:%4  DateTime:%5")
//            .arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function).arg(strDateTime);

    QString strDateTime = QDateTime::currentDateTime().toString("yyMMdd hh:mm:ss.zzz");
    QString strMessage = QString("DateTime:%1 %2 File:%3  Line:%4 Function:%5 >> %6")
                         .arg(strDateTime).arg(strMsg).arg(context.file).arg(context.line)
                         .arg(context.function).arg(localMsg.constData());

    QString LocalPath = qApp->applicationDirPath();
    // 输出信息至文件中（读写、追加形式）
    QFile file(LocalPath.append("/log.txt"));
    file.open(QIODevice::ReadWrite | QIODevice::Append);
    QTextStream stream(&file);
    stream << strMessage << "\r\n";
    file.flush();
    file.close();

    // 解锁
    mutex.unlock();
}

int main(int argc, char *argv[])

{
    QApplication a(argc, argv);
//    qInstallMessageHandler(myMessageOutput);
    MainWindow *w = new MainWindow();
    w->show();

    return a.exec();
}
