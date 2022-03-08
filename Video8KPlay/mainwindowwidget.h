#ifndef MAINWINDOWWIDGET_H
#define MAINWINDOWWIDGET_H

#include <QWidget>

class MainWindowWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindowWidget(QWidget *parent = nullptr);
    void NotBackgroud() {m_backGroudSwitch = false;}
signals:



private:
    void InitUi();
    bool m_backGroudSwitch = true;

protected:
    void paintEvent(QPaintEvent *);

};

#endif // MAINWINDOWWIDGET_H
