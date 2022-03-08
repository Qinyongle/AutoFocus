#ifndef MOSAICLABLE_H
#define MOSAICLABLE_H

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
#include <QAction>
//#include <gl/GLU.h>

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)
QT_FORWARD_DECLARE_CLASS(QOpenGLTexture)


QT_FORWARD_DECLARE_CLASS(Nv12Render)
QT_FORWARD_DECLARE_CLASS(VideoData)

class MosaicLable : public QOpenGLWidget,protected QOpenGLFunctions
{
    Q_OBJECT
public:
    #define WIDGET_FRAME 1
    typedef struct MOUSE_CHOICE_ZONE_STRUCT {
        MOUSE_CHOICE_ZONE_STRUCT(){
            reset();
        }
        int begainX;
        int begainY;
        int endX;
        int endY;
        int reco;
        QSize souceSize;
        void reset() {
            begainX = 0;
            begainY = 0;
            endX = 0;
            endY = 0;
            reco = 0;
        }
    } MOUSE_CHOICE_ZONE;

    typedef struct DRAW_ZONE_STRUCT {
        int id;
        QPoint ltPoint;
        QPoint rdPoint;
        MOUSE_CHOICE_ZONE oldZone;
        MOUSE_CHOICE_ZONE newZone;
        bool drawed = false;
        void Reset() {
            ltPoint.setX(0);
            ltPoint.setY(0);
            rdPoint.setX(0);
            rdPoint.setY(0);
            drawed = false;
            oldZone.reset();
            newZone.reset();
        }
    }DRAW_ZONE;

    explicit MosaicLable(QWidget *parent = nullptr);
    ~MosaicLable();
    QSize m_qimageSize;

signals:
    void SendSelectedZone(int, MosaicLable::MOUSE_CHOICE_ZONE);
    void CloseSelectedZone(int);
    void SpaceKeyPress();
    void FocusZone(int, int, int, int);
    void ImageUpdate();
protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent *ev) override;
    // opengl
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

private:
    void InitRightMouceKeyMenu();
    void DrawFocusRectangle();
    void DrawRectangleBase(QPoint, QPoint, int);
    void DrawRectangleList();
    void AdjustPoint();
    int GetZoneName(QPoint);
    void InitBackgroundImage();
public:
    // new
    void initialize();
    void render(uchar* ptr,int width,int height);

public slots:
    // opengl
    void slotShowYuv(unsigned char *,unsigned int,unsigned int); //显示一帧Yuv图像
//    void MainWinSizeChange(QSize);
    void SelectedZoneMove(int, QSize, QPoint, QPoint, bool);
    void SelectedZoneWheel(int, QSize, QPoint, bool);

    // draw zone size
    void GetDrawZoneSize(int);

    void LableClose(int);

private slots:
    void AddZoneClicked();
    void ReviseZoneClicked();
    void DeleteZoneClicked();


private:
    bool m_leftButtonPressState = false;




    // opengl
    // nv12
    //shader程序
    QOpenGLShaderProgram m_program;
    GLuint m_idY,m_idUV;
    QOpenGLBuffer m_vbo;
    unsigned char *m_nv12Ptr;
    unsigned char *m_nv12PtrLast;

    bool m_firstGetImage = false;


    // 未处理坐标点
    QPoint m_begainPoint;
    QPoint m_endPoint;
    // 选区的坐标
    QPoint    m_ltPoint;                //左上角的坐标
    QPoint    m_tempPoint;              //临时右下角坐标

    bool m_videoPlaying = false;

    // 接收选框回值的坐标点
    MOUSE_CHOICE_ZONE m_newZone;
    MOUSE_CHOICE_ZONE m_oldZone;

    // 选区的坐标数组
    QList<DRAW_ZONE> m_drawZoneList;
    int m_drawZoneSize = 1;
    int m_currentDrawZoneNum = 0;

    // 右键菜单
    QMenu *m_rightKeyMenu = nullptr;
    QAction *m_addZoneAction = nullptr;
    QAction *m_deleteZoneAction = nullptr;
    QAction *m_reviseZoneAction = nullptr;

    // 删除选区数据暂存
    int m_currentSelectedZoneId = -1;

    // 右键选取焦点
    bool m_rightKeyPressState = false;
    QPoint m_focusPointBegain;
    QPoint m_focusPointEnd;


};

#endif // MOSAICLABLE_H
