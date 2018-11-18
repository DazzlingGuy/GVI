#include "GVIIdentifyResultFrame.h"
#include "GVIConsts.h"

#include <QLabel>
#include <QVBoxLayout>

const int c_nMainWindownShadowWidth = 8;
const int c_nMainWindownWidth = 480;
const int c_nMainWindownHeight = 670;

const int c_nMatchImageWidth = 448;
const int c_nMatchImageHeight = 340;

const int c_nDstImageWidth = 448;
const int c_nDstImageHeight = 240;

const int c_nTipsWidth = 448;
const int c_nTipsHeight = 30;

GVIIdentifyResultFrame::GVIIdentifyResultFrame(QWidget *parent)
    : GVIBaseFrame(c_nMainWindownWidth, c_nMainWindownHeight, c_nMainWindownShadowWidth, parent)
    , m_dSimliar(0.0)
{
    init();
}

GVIIdentifyResultFrame::~GVIIdentifyResultFrame()
{
}

void GVIIdentifyResultFrame::init()
{
    initFrame();
}

void GVIIdentifyResultFrame::initFrame()
{
    m_pTips = new QLabel(this);
    m_pTips->setFixedSize(c_nTipsWidth, c_nTipsHeight);
    m_pTips->setStyleSheet(gviglobalstyle::strGlobalTitleStyle);

    m_pDstImageLabel = new QLabel(this);
    m_pDstImageLabel->setScaledContents(true);
    m_pDstImageLabel->setFixedSize(c_nDstImageWidth, c_nDstImageHeight);

    m_pMatchImageLabel = new QLabel(this);
    m_pMatchImageLabel->setScaledContents(true);
    m_pMatchImageLabel->setFixedSize(c_nMatchImageWidth, c_nMatchImageHeight);

    QVBoxLayout *pMainLayout = new QVBoxLayout(this);
    pMainLayout->setAlignment(Qt::AlignCenter);
    pMainLayout->addWidget(m_pTips);
    pMainLayout->addSpacing(10);
    pMainLayout->addWidget(m_pDstImageLabel);
    pMainLayout->addSpacing(10);
    pMainLayout->addWidget(m_pMatchImageLabel);
}

void GVIIdentifyResultFrame::setMatchImage(const QImage &image)
{
    m_pMatchImageLabel->setPixmap(QPixmap::fromImage(image));
}

void GVIIdentifyResultFrame::setDstImage(const QImage &image)
{
    m_pDstImageLabel->setPixmap(QPixmap::fromImage(image));
}

void GVIIdentifyResultFrame::setTips(const QString &strTips)
{
    m_pTips->setText(strTips);
}