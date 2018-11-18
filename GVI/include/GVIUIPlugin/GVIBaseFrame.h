#ifndef _GVIBASEFRAME_H
#define _GVIBASEFRAME_H

#include <QWidget>
#include <QMouseEvent>

class QPushButton;

class GVIBaseFrame : public QWidget
{
    Q_OBJECT

public:
    GVIBaseFrame(int nWidth, int nHeight, int nShadow, QWidget *parent = Q_NULLPTR);

private:
    void initAttribute();
    void initFrameSize();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    int m_nWidth;
    int m_nHeight;
    int m_nShadow;

    bool m_bPressed;
    QPoint m_point;
};

#endif // _GVIBASEFRAME_H