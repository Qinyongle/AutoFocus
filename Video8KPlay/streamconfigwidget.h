#ifndef STREAMCONFIGWIDGET_H
#define STREAMCONFIGWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QLayout>
#include <QPushButton>

class StreamConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StreamConfigWidget(QWidget *parent = nullptr);

signals:
    void SendStreamUrl(QString);
private slots:
    void OkButClicked();
    void CancelButClicked();
private:
    void InitUi();
    void InitConfig();

private:
    QLineEdit *m_urlLine = nullptr; //你
    QPushButton *m_okBut = nullptr;
    QPushButton *m_cancelBut = nullptr;

};

#endif // STREAMCONFIGWIDGET_H
