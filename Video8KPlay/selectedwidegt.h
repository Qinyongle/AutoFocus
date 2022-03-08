#ifndef SELECTEDWIDEGT_H
#define SELECTEDWIDEGT_H

#include <QWidget>
#include <QKeyEvent>
class SelectedWidegt : public QWidget
{
    Q_OBJECT
public:
    explicit SelectedWidegt(QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent* size) override;
    void keyPressEvent(QKeyEvent *ev) override;
    void closeEvent(QCloseEvent *event) override;
    QSize sizeHint() const override {
        return QSize(1280,720); /* 在这里定义dock的初始大小 */
    }


signals:
    void ResizeSig(QResizeEvent* size);
    void SpaceKeyPress();
    void WidgetClose();

public slots:
    // widget top
    void WidgetTop();

private:
    QPoint mousePoint;
     bool mousePress = false;

};

#endif // SELECTEDWIDEGT_H
