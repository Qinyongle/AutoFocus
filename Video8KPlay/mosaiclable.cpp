#include "mosaiclable.h"
#include <QDebug>
#include <QtMath>
#include <qmath.h>
#include <QPixmap>
#include <QPainter>
#include <QMenu>
#include <QMatrix4x4>
#include <QThread>
#define ATTRIB_VERTEX 0
#define ATTRIB_TEXTURE 1

#define VERTEXIN 0
#define TEXTUREIN 1
#define MY_PI 3.14159265358979323846
MosaicLable::MosaicLable(QWidget *parent) : QOpenGLWidget(parent)
{
    qRegisterMetaType<MosaicLable::MOUSE_CHOICE_ZONE>("MosaicLable::MOUSE_CHOICE_ZONE");
    m_qimageSize.setWidth(0);
    m_qimageSize.setHeight(0);
    InitRightMouceKeyMenu();
    GetDrawZoneSize(1);
    //connect(this, SIGNAL(ImageUpdate()), this, SLOT(update()), Qt::QueuedConnection);
    //InitBackgroundImage();
    //this->setStyleSheet("border-image:url(./pic/mainLogo.png)" );
}

MosaicLable::~MosaicLable()
{

}

void MosaicLable::InitBackgroundImage()
{
    QPainter *painter = new QPainter(this);
    QPixmap pix;
    pix.load("./pic/mainLogo.png");
    painter->drawPixmap(0,0,100,33,pix);
}

void MosaicLable::InitRightMouceKeyMenu()
{
    m_rightKeyMenu = new QMenu(this);
    m_addZoneAction = new QAction(QString::fromLocal8Bit("添加选区"));
    connect(m_addZoneAction, SIGNAL(triggered()), this, SLOT(AddZoneClicked()));
    m_reviseZoneAction = new QAction(QString::fromLocal8Bit("修改该选区"));
    connect(m_reviseZoneAction, SIGNAL(triggered()), this, SLOT(ReviseZoneClicked()));
    m_deleteZoneAction = new QAction(QString::fromLocal8Bit("删除该选区"));
    connect(m_deleteZoneAction, SIGNAL(triggered()), this, SLOT(DeleteZoneClicked()));

    m_rightKeyMenu->addAction(m_addZoneAction);
    m_rightKeyMenu->addAction(m_reviseZoneAction);
    m_rightKeyMenu->addAction(m_deleteZoneAction);
}



void MosaicLable::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) {
        m_leftButtonPressState = true;
        m_begainPoint = event->pos();
        m_endPoint = m_begainPoint;
    } else if(event->button() == Qt::RightButton) {
        m_rightKeyPressState = true;
        m_focusPointBegain = event->pos();
        m_focusPointEnd = m_focusPointBegain;
    }
}

void MosaicLable::mouseMoveEvent(QMouseEvent *event)
{
    if (m_leftButtonPressState) {
        m_endPoint = event->pos();
        AdjustPoint();
        if(abs(m_ltPoint.x() - m_tempPoint.x()) < 10 && abs(m_ltPoint.y() - m_tempPoint.y()) < 10) {
            return;
        }
        m_drawZoneList[m_currentDrawZoneNum].drawed = true;
        m_drawZoneList[m_currentDrawZoneNum].ltPoint = m_ltPoint;
        m_drawZoneList[m_currentDrawZoneNum].rdPoint = m_tempPoint;
        update();
    } else if(m_rightKeyPressState) {
        m_focusPointEnd = event->pos();
        update();
    }
}

void MosaicLable::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) {
        m_leftButtonPressState = false;
        m_endPoint = event->pos();
        AdjustPoint();
        if((abs(m_ltPoint.x() - m_tempPoint.x()) >= 10 || abs(m_ltPoint.y() - m_tempPoint.y()) >= 10)
            && (m_videoPlaying)) {
            MOUSE_CHOICE_ZONE data;
            data.begainX = m_ltPoint.x();
            data.begainY = m_ltPoint.y();
            data.endX = m_tempPoint.x();
            data.endY = m_tempPoint.y();
            data.souceSize = this->size();
            m_oldZone = data;
            m_drawZoneList[m_currentDrawZoneNum].drawed = true;
            m_drawZoneList[m_currentDrawZoneNum].ltPoint = m_ltPoint;
            m_drawZoneList[m_currentDrawZoneNum].rdPoint = m_tempPoint;
            m_drawZoneList[m_currentDrawZoneNum].oldZone = data;
            emit SendSelectedZone(m_currentDrawZoneNum, data);
            this->lower();
        }
    } else if(event->button() == Qt::RightButton) {
        if(abs(m_focusPointBegain.x() - m_focusPointEnd.x()) > 10 ||
                abs(m_focusPointBegain.y() - m_focusPointEnd.y()) > 10) {
            QPoint begainPoint;// = m_focusPointBegain;
            begainPoint.setX(m_focusPointBegain.x() <= m_focusPointEnd.x() ? m_focusPointBegain.x(): m_focusPointEnd.x());
            begainPoint.setY(m_focusPointBegain.y() <= m_focusPointEnd.y() ? m_focusPointBegain.y(): m_focusPointEnd.y());

            int x = begainPoint.x();
            int y = begainPoint.y();
            int w = abs(m_focusPointBegain.x() - m_focusPointEnd.x());
            int h = abs(m_focusPointBegain.y() - m_focusPointEnd.y());
            double dX =  (this->width() * 1.0);
            double dY =  (this->height() * 1.0);
            double dd = 1000.0;
            emit FocusZone(x / dX * dd, y / dY * dd, w / dX * dd, h / dY * dd);
            qDebug() << "x = " << x / dX * dd << y / dY * dd << w * dX << h * dY << this->width() << this->height();
        } else {
            if((m_currentSelectedZoneId = GetZoneName(event->pos())) == -1) {
                m_deleteZoneAction->setEnabled(false);
                m_reviseZoneAction->setEnabled(false);
            } else {
                m_deleteZoneAction->setEnabled(true);
                m_reviseZoneAction->setEnabled(true);
            }
            m_rightKeyMenu->exec(QCursor::pos());
        }
        m_focusPointBegain.setX(0);
        m_focusPointBegain.setY(0);
        m_focusPointEnd.setX(0);
        m_focusPointEnd.setY(0);
    }
}

void MosaicLable::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Space) {
        emit SpaceKeyPress();
    }
}

void MosaicLable::SelectedZoneMove(int id, QSize size, QPoint begain, QPoint end, bool state)
{
    MOUSE_CHOICE_ZONE oldZone = m_drawZoneList.at(id).oldZone;
    MOUSE_CHOICE_ZONE newZone = oldZone;
    double d = (oldZone.endX - oldZone.begainX) / (size.width() * 1.0);
    double dx = (end.x() - begain.x()) * d;
    double dy = (end.y() - begain.y()) * d;
    newZone.begainX -= dx;
    newZone.begainY -= dy;
    newZone.endX -= dx;
    newZone.endY -= dy;
    if(newZone.begainX <= 0 || newZone.begainY <= 0) {
        return;
    }
    if(newZone.endX >= this->size().width() || newZone.endY >= this->size().height()) {
        return;
    }
    emit SendSelectedZone(id, newZone);
    m_drawZoneList[id].ltPoint.setX(newZone.begainX);
    m_drawZoneList[id].ltPoint.setY(newZone.begainY);
    m_drawZoneList[id].rdPoint.setX(newZone.endX);
    m_drawZoneList[id].rdPoint.setY(newZone.endY);
    if(state) {
        m_drawZoneList[id].oldZone = newZone;
    }

    update();
}

void MosaicLable::SelectedZoneWheel(int id, QSize size, QPoint point, bool state)
{
    MOUSE_CHOICE_ZONE oldZone = m_drawZoneList.at(id).oldZone;
    MOUSE_CHOICE_ZONE newZone = oldZone;
    int dWidth = (newZone.endX - newZone.begainX);
    int dHeight = (newZone.endY - newZone.begainY);

    if(state) {
        newZone.begainX -= dWidth * 0.1;
        newZone.begainY -= dHeight * 0.1;
        newZone.endX += dWidth * 0.1;
        newZone.endY += dHeight * 0.1;
    } else {
        newZone.begainX += dWidth * 0.1;
        newZone.begainY += dHeight * 0.1;
        newZone.endX -= dWidth * 0.1;
        newZone.endY -= dHeight * 0.1;
    }
    QPoint selectedLableCenter;
    selectedLableCenter.setX(size.width() / 2);
    selectedLableCenter.setY(size.height() / 2);
    double d = dWidth / (size.width() * 1.0);
    double dx = point.x() - selectedLableCenter.x();
    double dy = point.y() - selectedLableCenter.y();
    newZone.begainX += dx * d;
    newZone.begainY += dy * d;
    newZone.endX += dx * d;
    newZone.endY += dy * d;
    if(newZone.begainX <= 0 || newZone.begainY <= 0) {
        return;
    }
    if(newZone.endX >= this->size().width() || newZone.endY >= this->size().height()) {
        return;
    }
    emit SendSelectedZone(id, newZone);
    m_ltPoint.setX(newZone.begainX);
    m_ltPoint.setY(newZone.begainY);
    m_tempPoint.setX(newZone.endX);
    m_tempPoint.setY(newZone.endY);
    m_drawZoneList[id].ltPoint.setX(newZone.begainX);
    m_drawZoneList[id].ltPoint.setY(newZone.begainY);
    m_drawZoneList[id].rdPoint.setX(newZone.endX);
    m_drawZoneList[id].rdPoint.setY(newZone.endY);
    m_drawZoneList[id].oldZone = newZone;
    update();
}


void MosaicLable::initializeGL()
{
    initialize();
}


void MosaicLable::initialize()
{
    qDebug() << "initializeGL";

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

void MosaicLable::paintGL()
{
    if(!m_nv12Ptr)return;
    DrawFocusRectangle();
    DrawRectangleList();
    render(m_nv12Ptr, m_qimageSize.width(), m_qimageSize.height());
}

void MosaicLable::render(uchar* ptr,int width,int height)
{
    if(m_videoPlaying) {
        glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
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

void MosaicLable::DrawFocusRectangle()
{
    if(abs(m_focusPointBegain.x() - m_tempPoint.x()) < 10 && abs(m_focusPointBegain.y() - m_tempPoint.y()) < 10) {
        return;
    }
    QPoint begainPoint;// = m_focusPointBegain;
    QPoint endPoint;// = m_focusPointEnd;
    begainPoint.setX(m_focusPointBegain.x() <= m_focusPointEnd.x() ? m_focusPointBegain.x(): m_focusPointEnd.x());
    begainPoint.setY(m_focusPointBegain.y() <= m_focusPointEnd.y() ? m_focusPointBegain.y(): m_focusPointEnd.y());
    endPoint.setX(m_focusPointBegain.x() > m_focusPointEnd.x() ? m_focusPointBegain.x(): m_focusPointEnd.x());
    endPoint.setY(m_focusPointBegain.y() > m_focusPointEnd.y() ? m_focusPointBegain.y(): m_focusPointEnd.y());

    // 纹理坐标要按照 顶点坐标的顺序结构来写
    GLfloat x1, y1, x2, y2, x3, y3, x4, y4;
    int x1R = (begainPoint.x() - this->size().width() / 2);
    int y1R = -(begainPoint.y() - this->size().height() / 2);

    int widthR = (endPoint.x() - begainPoint.x());
    int heightR = endPoint.y() - begainPoint.y();// 这边是减

    GLfloat widthO = widthR / (this->size().width() / 2.0);
    GLfloat HeightO = heightR / (this->size().height() / 2.0);

    x1 = x1R / (this->size().width() / 2.0);
    y1 = y1R / (this->size().height() / 2.0);

    x2 = x1 + widthO;
    y2 = y1;

    x3 = x1;
    y3 = y1 - HeightO;

    x4 = x2;
    y4 = y3;

    //qDebug() << x1 << y1 << widthO << HeightO;


    glPushMatrix();
    glLineWidth(1);
    glBegin(GL_LINES);//线
        // top
        glVertex3f(x1,y1,0);
        glVertex3f(x2,y2,0);
        // down
        glVertex3f(x3,y3,0);
        glVertex3f(x4,y4,0);
        // left
        glVertex3f(x1,y1,0);
        glVertex3f(x3,y3,0);
        // right
        glVertex3f(x2,y2,0);
        glVertex3f(x4,y4,0);
    glEnd();
    glPopMatrix();
}



void MosaicLable::DrawRectangleBase(QPoint ltPoint, QPoint tempPoint, int id)
{
    QString text = QString::number(id);
    if(abs(ltPoint.x() - tempPoint.x()) < 10 && abs(ltPoint.y() - tempPoint.y()) < 10) {
        return;
    }

    // 纹理坐标要按照 顶点坐标的顺序结构来写
    GLfloat x1, y1, x2, y2, x3, y3, x4, y4;
    int x1R = (ltPoint.x() - this->size().width() / 2);
    int y1R = -(ltPoint.y() - this->size().height() / 2);

    int widthR = (tempPoint.x() - ltPoint.x());
    int heightR = tempPoint.y() - ltPoint.y();// 这边是减

    GLfloat widthO = widthR / (this->size().width() / 2.0);
    GLfloat HeightO = heightR / (this->size().height() / 2.0);
    GLfloat textLong = 10.0 / (this->size().width() / 2.0);

    x1 = x1R / (this->size().width() / 2.0);
    y1 = y1R / (this->size().height() / 2.0);

    x2 = x1 + widthO;
    y2 = y1;

    x3 = x1;
    y3 = y1 - HeightO;

    x4 = x2;
    y4 = y3;

    glPushMatrix();
    glLineWidth(3);
    glBegin(GL_LINES);//线
        // top
        glVertex3f(x1,y1,0);
        glVertex3f(x2,y2,0);
        // down
        glVertex3f(x3,y3,0);
        glVertex3f(x4,y4,0);
        // left
        glVertex3f(x1,y1,0);
        glVertex3f(x3,y3,0);
        // right
        glVertex3f(x2,y2,0);
        glVertex3f(x4,y4,0);
    glEnd();
        glRasterPos2f(x4 + textLong, y4);
        static bool fi = true;
        if(fi) {
            fi = false;
            wglUseFontBitmaps(wglGetCurrentDC(), 1, 128, glGenLists(128));
        }
        glCallLists(text.toStdString().size(), GL_BYTE, text.data());
    glPopMatrix();
}

void MosaicLable::DrawRectangleList()
{
    for(int i = 0; i < m_drawZoneList.size(); i++) {
        if(m_drawZoneList.at(i).drawed) {
            DrawRectangleBase(m_drawZoneList.at(i).ltPoint, m_drawZoneList.at(i).rdPoint, m_drawZoneList.at(i).id);
        }
    }
}

void MosaicLable::AdjustPoint()
{
    m_ltPoint.setX(m_begainPoint.x() <= m_endPoint.x() ? m_begainPoint.x(): m_endPoint.x());
    m_ltPoint.setY(m_begainPoint.y() <= m_endPoint.y() ? m_begainPoint.y(): m_endPoint.y());
    m_tempPoint.setX(m_begainPoint.x() > m_endPoint.x() ? m_begainPoint.x(): m_endPoint.x());
    m_tempPoint.setY(m_begainPoint.y() > m_endPoint.y() ? m_begainPoint.y(): m_endPoint.y());
}

void MosaicLable::resizeGL(int , int )
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    for(int i = 0; i < m_drawZoneList.size(); i++) {
        if(m_drawZoneList[i].drawed) {
            m_drawZoneList[i].oldZone.souceSize = this->size();
            emit SendSelectedZone(i, m_drawZoneList[i].oldZone);
        }
    }

}

void MosaicLable::slotShowYuv(unsigned char *ptr,unsigned int width,unsigned int height)
{
    if(ptr == nullptr)
        return;

    double dx = this->size().width() / (width * 1.0);
    m_videoPlaying = true;

    if(m_qimageSize.width() != width || m_qimageSize.height() != height) {
        m_qimageSize.setWidth(width);
        m_qimageSize.setHeight(height);
        m_firstGetImage = false;
    }
    m_nv12Ptr = ptr;

    if(!m_firstGetImage) {
        m_firstGetImage = true;
        this->resize(this->size().width(), m_qimageSize.height() * dx);
    }
    // 更新画面（调用paingl）
    update();
    emit ImageUpdate();

    this->show();
}


void MosaicLable::GetDrawZoneSize(int size)
{
    m_drawZoneSize = size;
    for(int i = 0; i < m_drawZoneList.size(); i++) {
        m_drawZoneList[i].Reset();
    }
    m_drawZoneList.clear();
    m_currentSelectedZoneId = 0;
    for(int i = 0; i < m_drawZoneSize; i++) {
        DRAW_ZONE data;
        data.id = i;
        m_drawZoneList.append(data);
    }
}

void MosaicLable::AddZoneClicked()
{
    qDebug() << "AddZoneClicked" << m_drawZoneList.size();
    for(int i = 0; i < m_drawZoneList.size(); i++) {
        if(!m_drawZoneList.at(i).drawed) {
            m_currentDrawZoneNum = m_drawZoneList.at(i).id;
            break;
        }
    }
}

void MosaicLable::ReviseZoneClicked()
{
    for(int i = 0; i < m_drawZoneList.size(); i++) {
        if(m_drawZoneList.at(i).id == m_currentSelectedZoneId) {
            m_drawZoneList[i].Reset();
            m_currentDrawZoneNum = m_drawZoneList.at(i).id;
            update();
            return;
        }
    }
}

void MosaicLable::DeleteZoneClicked()
{
    qDebug() << "DeleteZoneClicked" << m_currentSelectedZoneId;
    for(int i = 0; i < m_drawZoneList.size(); i++) {
        if(m_drawZoneList.at(i).id == m_currentSelectedZoneId) {
            m_drawZoneList[i].Reset();
            CloseSelectedZone(m_currentSelectedZoneId);
            update();
            return;
        }
    }
}

int MosaicLable::GetZoneName(QPoint point)
{
    for(int i = m_drawZoneList.size() - 1; i >= 0; i--) {
        if(m_drawZoneList.at(i).drawed) {
            QPoint minPoint = m_drawZoneList.at(i).ltPoint;
            QPoint maxPoint = m_drawZoneList.at(i).rdPoint;
            if(point.x() >= minPoint.x() && point.x() <= maxPoint.x()) {
                if(point.y() >= minPoint.y() && point.y() <= maxPoint.y()) {
                    return m_drawZoneList.at(i).id;
                }
            }
        }
    }
    return -1;
}

void MosaicLable::LableClose(int id)
{
    if(m_drawZoneList.size() > id) {
        m_drawZoneList[id].Reset();
    }
    update();
}





