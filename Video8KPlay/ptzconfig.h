#ifndef PTZCONFIG_H
#define PTZCONFIG_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>

class PtzConfig : public QWidget
{
    Q_OBJECT
public:
    explicit PtzConfig(QWidget *parent = nullptr);

signals:


public slots:
    void OverButtonClicked();


protected:
    void showEvent(QShowEvent *event);

private:
    void InitUi();
    void InitConfig();






private:
    QPushButton *m_overButton = nullptr;
    QPushButton *m_cancelButton = nullptr;

    QLineEdit *m_rlIpEdit = nullptr;
    QLineEdit *m_rlPortEdit = nullptr;

    QLineEdit *m_zoomIpEdit = nullptr;
    QLineEdit *m_zoomPortEdit = nullptr;
};

#endif // PTZCONFIG_H
