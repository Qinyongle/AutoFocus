#ifndef SELECTEDLABLE_H
#define SELECTEDLABLE_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QApplication>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QApplication>
#include <QMenu>
#include "mosaiclable.h"
QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)
QT_FORWARD_DECLARE_CLASS(QOpenGLTexture)


QT_FORWARD_DECLARE_CLASS(Nv12Render)
QT_FORWARD_DECLARE_CLASS(VideoData)

class SelectedLable : public QOpenGLWidget,protected QOpenGLFunctions
{
    Q_OBJECT
#define WIDGET_FRAME 1
public:
    typedef struct LABLE_CUT_DATA_STRUCT{
        LABLE_CUT_DATA_STRUCT() {
            reset();
        }
        int x, y, height, width;
        double contrastValue;
        void reset() {
            x = 0;
            y = 0;
            height = 0;
            width = 0;
            contrastValue = 0.0;
        }
    }LABLE_CUT_DATA;


    explicit SelectedLable(QWidget *parent = nullptr);
    void SetId(int id) {m_id = id;}
    ~SelectedLable();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *ev) override;

    // opengl
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

private:
    // new
    void initialize();
    void render(uchar* ptr,int width,int height);

signals:
    void SelectedZoneMove(int, QSize, QPoint, QPoint, bool);
    void SelectedZoneWheel(int, QSize, QPoint, bool);
    void SpaceKeyPress();
    void SelectedLableHide(int);
public slots:
    void GetMouseChoiceZone(int, MosaicLable::MOUSE_CHOICE_ZONE);
    void SelectedWidgetSizeChange(QResizeEvent *);
    void CloseThisZone(int);
    // opengl
    void slotShowYuv(unsigned char *,unsigned int,unsigned int); //显示一帧Yuv图像

    // widget close
    void WidgetClose();

    //set AUX
    void SetAuxrect(QRect rect);



private:
    QWidget *m_parent = nullptr;
    MosaicLable::MOUSE_CHOICE_ZONE m_mouseChoiceZone;
    LABLE_CUT_DATA m_realLableSize;

    QSize m_beforeCutLableSize;
    QSize m_qimageSize;

    QString m_lableName;

    // opengl
    // nv12
    QOpenGLShaderProgram m_program;
    GLuint m_idY,m_idUV;
    QOpenGLBuffer m_vbo;

    unsigned char *m_nv12Ptr;

    bool m_firstGetImage = false;
    bool m_leftButtonClicked = false;


    // 未处理坐标点
    QPoint m_begainPoint;
    QPoint m_endPoint;
    // 选区的坐标
    QPoint    m_ltPoint;                //左上角的坐标
    QPoint    m_tempPoint;              //临时右下角坐标

    bool m_videoPlaying = false;

    int m_id = 0;

};

#endif // SELECTEDLABLE_H
