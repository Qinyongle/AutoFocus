#include "selectedlable.h"
#include "mosaiclable.h"
#include <QDebug>
#include <QtMath>
#include <qmath.h>
#include <QPixmap>
#include <QPainter>
#include <QMenu>
#include <QMatrix4x4>

#define ATTRIB_VERTEX 0
#define ATTRIB_TEXTURE 1

#define VERTEXIN 0
#define TEXTUREIN 1
#define MY_PI 3.14159265358979323846


SelectedLable::SelectedLable(QWidget *parent) : QOpenGLWidget(parent)
{
    m_parent = parent;
    qRegisterMetaType<QSize>("QSize");
    qRegisterMetaType<QPoint>("QPoint");
    m_qimageSize.setWidth(0);
    m_qimageSize.setHeight(0);
    connect(m_parent, SIGNAL(WidgetClose()), this, SLOT(WidgetClose()));
}

SelectedLable::~SelectedLable()
{
    //hideEvent();
}

void SelectedLable::WidgetClose()
{
    emit SelectedLableHide(m_id);
}

void SelectedLable::SetAuxrect(QRect rect)
{
    //多屏联调---自动调整选区位置
    m_parent->move(rect.x(),rect.y());
}


void SelectedLable::wheelEvent(QWheelEvent *event)
{
    if(event->delta() < 0) {
        emit SelectedZoneWheel(m_id, this->size(), event->pos(), true);
    } else {
        emit SelectedZoneWheel(m_id,this->size(), event->pos(), false);
    }
    update();
}

void SelectedLable::mousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton) {
        m_leftButtonClicked = true;
        m_begainPoint = event->pos();
        m_endPoint = m_begainPoint;
    }
}

void SelectedLable::mouseReleaseEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton) {
        m_leftButtonClicked = false;
        m_endPoint = event->pos();
        emit SelectedZoneMove(m_id, this->size(), m_begainPoint, m_endPoint, true);
        update();
    }
}

void SelectedLable::mouseMoveEvent(QMouseEvent* event)
{
    if(m_leftButtonClicked) {
        m_endPoint = event->pos();
        emit SelectedZoneMove(m_id, this->size(), m_begainPoint, m_endPoint, false);
        update();
    }
}

void SelectedLable::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Space) {
        emit SpaceKeyPress();
        qDebug() << "keyPressEvent";
    }
}

void SelectedLable::GetMouseChoiceZone(int id, MosaicLable::MOUSE_CHOICE_ZONE data)
{
    m_parent->raise();
    if(id != m_id) {
        return;
    }
    //默认最大化
    m_parent->showMaximized();
//    m_parent->show();

    // 此鼠标坐标系建立在同样的widget上面
    m_mouseChoiceZone = data;
    if(data.begainX == 0 && data.begainY == 0 && data.endX == 0 && data.endY == 0) {
        if(data.reco) {
            m_realLableSize.reset();
            if(m_beforeCutLableSize.width() != 0) {
                //this->setFixedSize(m_beforeCutLableSize);
            }
            return;
        } else {
            return;
        }
    } else {
        m_beforeCutLableSize = this->size();
    }

    // 先将画框的方式统一为从左到右，从上到下，避免后面过多的判断。
    MosaicLable::MOUSE_CHOICE_ZONE dataTemp =data;
    // 原视数据lable尺寸的比值
    double lableSizeRate = (m_parent->size().width() - WIDGET_FRAME) / (data.souceSize.width() * 1.0);
    // x
    m_realLableSize.x = dataTemp.begainX * lableSizeRate;
    m_realLableSize.width = (dataTemp.endX - dataTemp.begainX) * lableSizeRate;

    // y
    m_realLableSize.y = dataTemp.begainY * lableSizeRate;
    m_realLableSize.height = (dataTemp.endY - dataTemp.begainY) * lableSizeRate;

    m_realLableSize.contrastValue = m_qimageSize.width() / ((m_parent->size().width() - WIDGET_FRAME) * 1.0);
    double d = (m_parent->size().width() - WIDGET_FRAME) / m_realLableSize.width;

    if(m_realLableSize.height * d > m_parent->height() - WIDGET_FRAME) {
        m_realLableSize.height = (m_parent->height() - WIDGET_FRAME) / d;
    }

    //this->setFixedHeight(m_realLableSize.height * d);
    this->resize(this->size().width(), m_realLableSize.height * d);
//    this->res
    //this->resize(m_realLableSize.width * d, m_realLableSize.height * d);
    //m_parent->setFixedHeight(this->size().height() + 10);


    qApp->processEvents();  // 用来更新界面
    update();
}

void SelectedLable::initializeGL()
{
    initialize();
}

void SelectedLable::initialize()
{
    //初始化opengl （QOpenGLFunctions继承）函数
    initializeOpenGLFunctions();

    //顶点shader
    const char *vsrc =
            "attribute vec4 vertexIn; \
             attribute vec2 textureIn; \
             varying vec2 textureOut;  \
             void main(void)           \
             {                         \
                 gl_Position = vertexIn; \
                 textureOut = textureIn; \
             }";

    const char *fsrc =
            "varying mediump vec2 textureOut;\n"
            "uniform sampler2D textureY;\n"
            "uniform sampler2D textureUV;\n"
            "void main(void)\n"
            "{\n"
            "vec3 yuv; \n"
            "vec3 rgb; \n"
            "float y, u, v, r, g, b, a; \n"
            "y = texture2D(textureY, textureOut.st).r - 0.0625; \n"
            "u = texture2D(textureUV, textureOut.st).r - 0.5; \n"
            "v = texture2D(textureUV, textureOut.st).g - 0.5; \n"
            "r = y + 1.5958 * v; \n"
            "g = y - 0.39173 * u - 0.81290 * v; \n"
            "b = y + 2.017 * u; \n"
            "gl_FragColor = vec4(r, g, b, 1.0); \n"
            "}\n";

    m_program.addCacheableShaderFromSourceCode(QOpenGLShader::Vertex,vsrc);
    m_program.addCacheableShaderFromSourceCode(QOpenGLShader::Fragment,fsrc);
    m_program.link();

    GLfloat points[]{
        -1.0f, 1.0f,
         1.0f, 1.0f,
         1.0f, -1.0f,
        -1.0f, -1.0f,

        0.0f,0.0f,
        1.0f,0.0f,
        1.0f,1.0f,
        0.0f,1.0f
    };

    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(points,sizeof(points));

    GLuint ids[2];
    glGenTextures(2,ids);
    m_idY = ids[0];
    m_idUV = ids[1];

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);//允许深度测试
}

void SelectedLable::paintGL()
{
    if(!m_nv12Ptr)return;

    if(m_realLableSize.x == 0 && m_realLableSize.y == 0
            && m_realLableSize.width == 0 && m_realLableSize.height == 0) {
        GLfloat points[]{
            -1.0f, 1.0f,
             1.0f, 1.0f,
             1.0f, -1.0f,
            -1.0f, -1.0f,

            0.0f,0.0f,
            1.0f,0.0f,
            1.0f,1.0f,
            0.0f,1.0f
        };

        m_vbo.create();
        m_vbo.bind();
        m_vbo.allocate(points,sizeof(points));
    } else {
        double d = m_realLableSize.contrastValue;
        // 纹理坐标要按照 顶点坐标的顺序结构来写
        GLfloat x1, y1, x2, y2, x3, y3, x4, y4;
        GLfloat x11, y11, x21, y21, x31, y31, x41, y41;
        int imageWidth = m_qimageSize.width();
        int imageHeight = m_qimageSize.height();

        // 计算纹理覆盖坐标
        x1 = m_realLableSize.x * d / (imageWidth * 1.0);
        y1 = m_realLableSize.y * d / (imageHeight * 1.0);
        x2 = x1 + (m_realLableSize.width * d) / (imageWidth * 1.0);
        y2 = y1;

        x3 = x2;
        y3 = y1 + (m_realLableSize.height * d) / (imageHeight * 1.0);
        x4 = x1;
        y4 = y3;

        x11 = x1; y11 = y1;
        x21 = x2; y21 = y2;
        x31 = x3; y31 = y3;
        x41 = x4; y41 = y4;

        GLfloat points[]{
            -1.0f, 1.0f,
             1.0f, 1.0f,
             1.0f, -1.0f,
            -1.0f, -1.0f,

            x11, y11,         // 左上角
            x21, y21,         // 右上角
            x31, y31,         // 右下角
            x41, y41          // 左下角
        };

        m_vbo.create();
        m_vbo.bind();
        m_vbo.allocate(points,sizeof(points));

    }

    render(m_nv12Ptr, m_qimageSize.width(), m_qimageSize.height());

}

void SelectedLable::render(uchar* ptr,int width,int height)
{
    if(m_videoPlaying) {
        glClearColor(0.2929f, 0.2929f, 0.2929f, 0.2929f);
    } else {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    }
    glClear(GL_COLOR_BUFFER_BIT);

    m_program.bind();
    m_vbo.bind();
    m_program.enableAttributeArray("vertexIn");
    m_program.enableAttributeArray("textureIn");
    m_program.setAttributeBuffer("vertexIn",GL_FLOAT, 0, 2, 2*sizeof(GLfloat));
    m_program.setAttributeBuffer("textureIn",GL_FLOAT,2 * 4 * sizeof(GLfloat),2,2*sizeof(GLfloat));

    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D,m_idY);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RED,width,height,0,GL_RED,GL_UNSIGNED_BYTE,ptr);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D,m_idUV);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RG,width >> 1,height >> 1,0,GL_RG,GL_UNSIGNED_BYTE,ptr + width*height);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    m_program.setUniformValue("textureUV",0);
    m_program.setUniformValue("textureY",1);
    glDrawArrays(GL_QUADS,0,4);
    m_program.disableAttributeArray("vertexIn");
    m_program.disableAttributeArray("textureIn");
    m_program.release();
}


void SelectedLable::resizeGL(int width, int height)
{
    // 没用过
//    glViewport(0, 0, width, height);
//    glMatrixMode(GL_PROJECTION);
//    float aspect = (float)width / (float)height;
//    glOrtho(-aspect, aspect, -1, 1, -1, 1);

//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
}

void SelectedLable::slotShowYuv(unsigned char *ptr,unsigned int width,unsigned int height)
{
    if(ptr == nullptr)
        return;

    m_videoPlaying = true;
    m_nv12Ptr = ptr;
    m_qimageSize.setWidth(width);
    m_qimageSize.setHeight(height);

    // 更新画面（调用paingl）
    update();

}

void SelectedLable::SelectedWidgetSizeChange(QResizeEvent * size)
{
    GetMouseChoiceZone(m_id, m_mouseChoiceZone);
}

void SelectedLable::CloseThisZone(int id)
{
    if(id == m_id) {
        m_parent->hide();
    }
}








