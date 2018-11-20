#include "GVIMainFrame.h"
#include "GVIPushButton.h"
#include "GVIImageContainer.h"
#include "GVIConsts.h"
#include "GVIMacro.h"
#include "GVIImageConverter.h"
#include "GVIIdentifyResultFrame.h"
#include "GVIVideoProcessor.h"
#include "GVIDateTime.h"
#include <GVIImageDataRole.h>

#include <QTimer>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QCoreApplication>

const int c_nImageHeight = 465;
const int c_nImageWidth = 465;

const double c_dFakeThreshold = 0.7;

GVIMainFrame::GVIMainFrame(QWidget *parent)
    : QWidget(parent)
    , m_pVideoImageLabel(nullptr)
    , m_pMatchImageLabel(nullptr)
    , m_pStartBtn(nullptr)
    , m_pEndBtn(nullptr)
    , m_pPhotoBtn(nullptr)
    , m_pIdentifyBtn(nullptr)
    , m_pPrevBtn(nullptr)
    , m_pNextBtn(nullptr)
    , m_pDateTime(nullptr)
    , m_pIdentifyFrame(nullptr)
    , m_pImageDataRole(nullptr)
    , m_qTimer(new QTimer())
    , m_bIsPlay(false)
    , m_pCameraImageContainer(new GVIImageContainer(gviimagepath::strCameraImages))
    , m_pVideoProcessor(new GVIVideoProcessor())
{
    init();
}

GVIMainFrame::~GVIMainFrame()
{
    FREEANDNIL(m_pCameraImageContainer);
    FREEANDNIL(m_pVideoProcessor);
}

void GVIMainFrame::refreshIdentifyFramePosition()
{
    moveIdentifyFrame();
}

void GVIMainFrame::init()
{
    initControl();
    initLayout();
    initSlots();
    initImageLabel();
    initStyle();
}

void GVIMainFrame::initStyle()
{
    this->setStyleSheet(gviglobalstyle::strGlobalBtnStyle);

    m_pVideoImageLabel->setStyleSheet(gviglobalstyle::strGlobalImageLabelStyle);
    m_pMatchImageLabel->setStyleSheet(gviglobalstyle::strGlobalImageLabelStyle);
}

void GVIMainFrame::initControl()
{
    m_pVideoImageLabel = new QLabel();
    m_pVideoImageLabel->setFixedSize(c_nImageWidth, c_nImageHeight);
    m_pVideoImageLabel->setScaledContents(true);

    m_pStartBtn = new GVIPushButton("Start");
    m_pEndBtn = new GVIPushButton("End");
    m_pPhotoBtn = new GVIPushButton("Photo");

    m_pMatchImageLabel = new QLabel();
    m_pMatchImageLabel->setFixedSize(c_nImageWidth, c_nImageHeight);
    m_pMatchImageLabel->setScaledContents(true);

    m_pPrevBtn = new GVIPushButton("Previous");
    m_pNextBtn = new GVIPushButton("Next");
    m_pIdentifyBtn = new GVIPushButton("Identify");

    m_pIdentifyFrame = new GVIIdentifyResultFrame(this);
}

void GVIMainFrame::initLayout()
{
    auto pLeftBottomLayout = createCommonHBoxLayout();
    pLeftBottomLayout->addWidget(m_pStartBtn);
    pLeftBottomLayout->addWidget(m_pEndBtn);
    pLeftBottomLayout->addWidget(m_pPhotoBtn);

    auto pLeftLayout = createCommonVBoxLayout();
    pLeftLayout->addWidget(m_pVideoImageLabel);
    pLeftLayout->addSpacing(20);
    pLeftLayout->addLayout(pLeftBottomLayout);

    auto pRightBottomLayout = createCommonHBoxLayout();
    pRightBottomLayout->addWidget(m_pPrevBtn);
    pRightBottomLayout->addWidget(m_pNextBtn);
    pRightBottomLayout->addWidget(m_pIdentifyBtn);

    auto pRightLayout = createCommonVBoxLayout();
    pRightLayout->addWidget(m_pMatchImageLabel);
    pRightLayout->addSpacing(20);
    pRightLayout->addLayout(pRightBottomLayout);

    auto pMainLayout = createCommonHBoxLayout(this);
    pMainLayout->addLayout(pLeftLayout);
    pMainLayout->addLayout(pRightLayout);
}

void GVIMainFrame::initSlots()
{
    connect(m_pStartBtn, &QPushButton::clicked, this, &GVIMainFrame::onStartClicked);
    connect(m_pEndBtn, &QPushButton::clicked, this, &GVIMainFrame::onEndClicked);
    connect(m_pPhotoBtn, &QPushButton::clicked, this, &GVIMainFrame::onPhotoClicked);
    connect(m_pIdentifyBtn, &QPushButton::clicked, this, &GVIMainFrame::onIdentifyClicked);
    connect(m_pPrevBtn, &QPushButton::clicked, this, &GVIMainFrame::onPrevClicked);
    connect(m_pNextBtn, &QPushButton::clicked, this, &GVIMainFrame::onNextClicked);
}

void GVIMainFrame::initImageLabel()
{
    onStartClicked();

    loadNextImage();
}

void GVIMainFrame::loadNextImage()
{
    QPixmap pixmap = QPixmap::fromImage(m_pCameraImageContainer->getNextImage());
    if (pixmap.isNull())
    {
        return;
    }

    m_pMatchImageLabel->setPixmap(pixmap);
}

void GVIMainFrame::loadPrevImage()
{
    QPixmap pixmap = QPixmap::fromImage(m_pCameraImageContainer->getPrevImage());
    if (pixmap.isNull())
    {
        return;
    }

    m_pMatchImageLabel->setPixmap(pixmap);
}

void GVIMainFrame::onStartClicked()
{
    if (!m_bIsPlay && m_pVideoProcessor->openCamera())
    {
        m_bIsPlay = true;

        connect(m_qTimer, &QTimer::timeout, this, &GVIMainFrame::onRefreshVideoImage);

        m_qTimer->start(30);
    }
}

void GVIMainFrame::onEndClicked()
{
    if (m_bIsPlay)
    {
        m_bIsPlay = false;

        disconnect(m_qTimer, &QTimer::timeout, this, &GVIMainFrame::onRefreshVideoImage);

        m_qTimer->stop();
    }
}

void GVIMainFrame::onPhotoClicked()
{
    QString pictureDirPath = qApp->applicationDirPath() + gviimagepath::strCameraImages;
    QString picturePath = pictureDirPath + GVIDateTime::getCurrentTime() + ".jpg";

    if (!m_pVideoProcessor->saveImage(picturePath))
    {
        return;
    }

    m_pCameraImageContainer->insertImage(QImage(picturePath), picturePath);
}

void GVIMainFrame::showIdentifyFrame()
{
    if (!canShowIdentifyFrame())
    {
        return;
    }

    if (m_pIdentifyFrame->isHidden())
    {
        m_pIdentifyFrame->show();
    }

    moveIdentifyFrame();
}

void GVIMainFrame::hideIdentifyFrame()
{
    if (!m_pIdentifyFrame->isHidden())
    {
        m_pIdentifyFrame->hide();
    }
}

void GVIMainFrame::moveIdentifyFrame()
{
    QPoint point = mapToGlobal(QPoint(this->geometry().top(), this->geometry().left()));
    m_pIdentifyFrame->move(point.x() + 798, point.y() - 164);
}

bool GVIMainFrame::canShowIdentifyFrame()
{
    bool bRes = false;

    if (m_pImageDataRole)
    {
        bRes = m_pImageDataRole->isComparison();
    }

    return bRes;
}

void GVIMainFrame::onRefreshVideoImage()
{
    GVIImageConverter converter;

    m_pVideoImageLabel->setPixmap(QPixmap::fromImage(converter.converterQImage(m_pVideoProcessor->getImage(), m_pVideoImageLabel->size())));
}

void GVIMainFrame::onIdentifyClicked()
{
    beginImageSimilartyMatch();

    beginShowIdentifyFrame();
}

void GVIMainFrame::beginImageSimilartyMatch()
{
    m_pCameraImageContainer->reLoadContainer();
    m_pCameraImageContainer->setCurImage(-1);
    loadNextImage();

    m_pImageDataRole = new GVIImageDataRole(m_pCameraImageContainer->getCurImagePath());

    GVIImageConverter converter;
    m_pMatchImageLabel->setPixmap(QPixmap::fromImage(converter.converterQImage(m_pImageDataRole->getFinalSrcMarkImage(), m_pMatchImageLabel->size())));
}

void GVIMainFrame::beginShowIdentifyFrame()
{
    updateIdentifyFrame();

    showIdentifyFrame();

    showIdentifyMsgBox();
}

void GVIMainFrame::updateIdentifyFrame()
{
    if (!canShowIdentifyFrame())
    {
        return;
    }

    GVIImageConverter converter;
    m_pIdentifyFrame->setDstImage(converter.converterQImage(m_pImageDataRole->getFinalSrcProcessedImage(), m_pVideoImageLabel->size()));

    if (m_pImageDataRole->isFake())
    {
        m_pIdentifyFrame->setTips(QString("The Similarity is %1%.").arg(QString::number((m_pImageDataRole->getFinalFakeSimilarty() * 100), 'f', 2)));
        m_pIdentifyFrame->setMatchImage(m_pImageDataRole->getFinalFakeSimilartyImage());
    }
    else
    {
        m_pIdentifyFrame->setTips(QString("The Similarity is %1%.").arg(QString::number((m_pImageDataRole->getFinalMaxSimilarty() * 100), 'f', 2)));
        m_pIdentifyFrame->setMatchImage(m_pImageDataRole->getFinalMaxSimilartyImage());
    }
}

void GVIMainFrame::showIdentifyMsgBox()
{
    if (!canShowIdentifyFrame())
    {
        return;
    }

    if (m_pImageDataRole->isFake())
    {
        if (m_pImageDataRole->getFinalFakeSimilarty() > c_dFakeThreshold)
        {
            QMessageBox::warning(this, "Fake", "The car has a deck risk, the administrator needs to pay attention to check!");
        }
        else
        {
            QMessageBox::information(this, "Normal", "The car is in normal access!");
        }
    }
    else
    {
        QMessageBox::information(this, "Normal", "The car is in normal access!");
    }
}

void GVIMainFrame::onPrevClicked()
{
    loadPrevImage();
    hideIdentifyFrame();
}

void GVIMainFrame::onNextClicked()
{
    loadNextImage();
    hideIdentifyFrame();
}

QHBoxLayout* GVIMainFrame::createCommonHBoxLayout(QWidget *parent)
{
    auto* pLayout = new QHBoxLayout(parent);

    return pLayout;
}

QVBoxLayout* GVIMainFrame::createCommonVBoxLayout(QWidget *parent)
{
    auto* pLayout = new QVBoxLayout(parent);

    return pLayout;
}