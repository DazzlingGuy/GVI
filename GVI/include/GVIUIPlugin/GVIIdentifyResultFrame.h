#ifndef _GVIIDENTIFYRESULTFRAME_H
#define _GVIIDENTIFYRESULTFRAME_H

#include "GVIBaseFrame.h"

class QLabel;

class GVIIdentifyResultFrame : public GVIBaseFrame
{
public:
    GVIIdentifyResultFrame(QWidget *parent = Q_NULLPTR);
    ~GVIIdentifyResultFrame();

    void setMatchImage(const QImage &image);
    void setDstImage(const QImage &image);
    void setTips(const QString &strTips);

private:
    void init();
    void initFrame();

private:
    QLabel *m_pDstImageLabel;
    QLabel *m_pMatchImageLabel;

    QLabel *m_pTips;

    double m_dSimliar;
};

#endif // _GVIIDENTIFYRESULTFRAME_H