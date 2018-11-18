#include "GVIBaseFrame.h"

#include <QPainter>
#include <QtMath>

GVIBaseFrame::GVIBaseFrame(int nWidth, int nHeight, int nShadow, QWidget *parent)
    : QWidget(parent)
    , m_nWidth(nWidth)
    , m_nHeight(nHeight)
    , m_nShadow(nShadow)
    , m_bPressed(false)
    , m_point(QPoint())
{
    initFrameSize();
    initAttribute();
}

void GVIBaseFrame::initAttribute()
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    this->setAttribute(Qt::WA_TranslucentBackground);
}

void GVIBaseFrame::initFrameSize()
{
    this->setFixedSize(m_nWidth + m_nShadow * 2, m_nHeight + m_nShadow * 2);
}

void GVIBaseFrame::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRect(m_nShadow, m_nShadow, this->width() - m_nShadow * 2, this->height() - m_nShadow * 2);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillPath(path, QBrush(Qt::white));

    QColor color(140, 140, 140, 55);
    for (int i = 0; i != m_nShadow; ++i)
    {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRoundRect(m_nShadow - i, m_nShadow - i, this->width() - (m_nShadow - i) * 2, this->height() - (m_nShadow - i) * 2, 2);
        color.setAlpha(140 - qSqrt(i) * 50);
        painter.setPen(color);
        painter.drawPath(path);
    }
}

void GVIBaseFrame::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_bPressed = true;
        m_point = event->pos();
    }
}

void GVIBaseFrame::mouseMoveEvent(QMouseEvent *event)
{
    if (m_bPressed)
    {
        move(event->pos() - m_point + pos());
    }
}

void GVIBaseFrame::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_bPressed = false;
}