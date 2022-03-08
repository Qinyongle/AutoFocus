#include "mainwindowwidget.h"
#include <QPalette>
#include <QPainter>
#include <QPixmap>

MainWindowWidget::MainWindowWidget(QWidget *parent) : QWidget(parent)
{
    InitUi();
}

void MainWindowWidget::paintEvent(QPaintEvent *)
{
    if(m_backGroudSwitch) {
        QPainter painter(this);
        QPixmap pixmap("./pic/mainLogo.png");
        painter.drawPixmap(QRect(this->size().width() / 2 - (this->size().height() - 100) / 2,
                                 0,
                                 this->size().height() - 100,
                                 this->size().height() - 100), pixmap);
    }

}

void MainWindowWidget::InitUi()
{
    QPalette pal(this->palette());

    //设置背景黑色
    pal.setColor(QPalette::Background, QColor(10,10,10));
    this->setAutoFillBackground(true);
    this->setPalette(pal);
}
