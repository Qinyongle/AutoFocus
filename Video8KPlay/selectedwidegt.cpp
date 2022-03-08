#include "selectedwidegt.h"
#include <QDebug>
SelectedWidegt::SelectedWidegt(QWidget *parent) : QWidget(parent)
{
    this->setStyleSheet("background:rgb(75,75,75);");
}


void SelectedWidegt::resizeEvent(QResizeEvent* size)
{
    emit ResizeSig(size);
}

void SelectedWidegt::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Space) {
        emit SpaceKeyPress();
        qDebug() << "keyPressEvent";
    }
}

void SelectedWidegt::closeEvent(QCloseEvent *)
{
    emit WidgetClose();
}

void SelectedWidegt::WidgetTop()
{
    this->raise();
}
