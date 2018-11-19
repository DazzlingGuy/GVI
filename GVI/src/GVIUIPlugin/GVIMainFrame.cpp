#include "GVIMainFrame.h"
#include "GVIPushButton.h"
#include "GVIImageContainer.h"
#include "GVIConsts.h"
#include "GVIMacro.h"
#include "GVIImageProcessor.h"
#include "GVIImageConverter.h"
#include "GVIIdentifyResultFrame.h"
#include "GVIVideoProcessor.h"
#include "GVIDateTime.h"
#include "GVIImageSimilarityCalculator.h"
#include "GVIProgressDialog.h"

#include <QTimer>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCoreApplication>
#include <QMessageBox>
#include <QFileDialog>
#include <QMessageBox>

const int c_nImageHeight = 465;
const int c_nImageWidth = 465;

const double c_dPlateRegion = 1.8;

const double c_dFakeThreshold = 0.7;

const QString s_szIdentifyTips = "Identify...";

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
    , m_pProgressDialog(nullptr)
    , m_pVideoProcessor(new GVIVideoProcessor())
    , m_pImageProcessor(new GVIImageProcessor())
    , m_pCameraImageContainer(new GVIImageContainer(gviimagepath::strCameraImages))
    , m_pCarOriginalImageContainer(new GVIImageContainer(gviimagepath::strCarOriginalImages))
    , m_qTimer(new QTimer())
    , m_bIsPlay(false)
    , m_bIsFake(false)
    , m_dFinalMaxSimilarty(0)
    , m_dFinalFakeSimilarty(0)
    , m_nFinalMaxSimilartyIndex(-1)
    , m_nFinalFakeSimilartyIndex(-1)
{
    init();
}

GVIMainFrame::~GVIMainFrame()
{
    FREEANDNIL(m_pCameraImageContainer);
    FREEANDNIL(m_pVideoProcessor);
    FREEANDNIL(m_pImageProcessor);
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
    initInageLabel();
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

void GVIMainFrame::initInageLabel()
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

void GVIMainFrame::canShowIdentifyFrame()
{
    return !m_pCameraImageContainer->isEmpty() && !m_pCarOriginalImageContainer->isEmpty();
}

void GVIMainFrame::freeProcessDialog()
{
    FREEANDNIL(m_pProgressDialog);
}

void GVIMainFrame::onRefreshVideoImage()
{
    GVIImageConverter converter;

    m_pVideoImageLabel->setPixmap(QPixmap::fromImage(converter.converterQImage(m_pVideoProcessor->getImage(), m_pVideoImageLabel->size())));
}

void GVIMainFrame::onIdentifyClicked()
{
    reLoad();

    createProgressDialog();

    doSimilartyMatch();

    doShowMatchResult();

    freeProcessDialog();
}

void GVIMainFrame::reLoad()
{
    reLoadData();
    reLoadImageContainer();
}

void GVIMainFrame::reLoadData()
{
    m_oFakeImageIndex.clear();

    m_nFinalMaxSimilartyIndex = -1;
    m_dFinalMaxSimilarty = 0;

    m_nFinalFakeSimilartyIndex = -1;
    m_dFinalFakeSimilarty = 0;

    m_bIsFake = false;
}

void GVIMainFrame::reLoadImageContainer()
{
    m_pCameraImageContainer->reLoadContainer();
    m_pCameraImageContainer->setCurImage(-1);
    loadNextImage();

    m_pCarOriginalImageContainer->reLoadContainer();
    m_pCarOriginalImageContainer->setCurImage(0);
}

void GVIMainFrame::createProgressDialog()
{
    if (!canShowIdentifyFrame())
    {
        return;
    }

    QCoreApplication::processEvents();
    m_pProgressDialog = new GVIProgressDialog(s_szIdentifyTips, m_pCarOriginalImageContainer->getImagesCount(), this);
    m_pProgressDialog->updateProgress(0);
    m_pProgressDialog->show();
}

void GVIMainFrame::doSimilartyMatch()
{
    processImage();

    calcSimilarty();

    dealMaskImage();
}

void GVIMainFrame::processImage()
{
    m_finalProcessedSrcImage = getProcessImageByPath(m_pCameraImageContainer->getCurImagePath());
    if (m_finalProcessedSrcImage.empty())
    {
        return;
    }

    markSrcImage(m_pImageProcessor->getSrcImage());
}

void GVIMainFrame::markSrcImage(cv::Mat srcImage)
{
    Rect plateRegion = m_pImageProcessor->getProcessedRegion(c_dPlateRegion);
    rectangle(srcImage, plateRegion, Scalar(0, 0, 255), 3);

    GVIImageConverter converter;
    m_pMatchImageLabel->setPixmap(QPixmap::fromImage(converter.converterQImage(srcImage, m_pVideoImageLabel->size())));
}

void GVIMainFrame::calcSimilarty()
{
    for (size_t i = 0; i < m_pCarOriginalImageContainer->getImagesCount(); i++)
    {
        cv::Mat originalProcessedImage = getProcessImageByIndex(i);

        double similartyValue = GVIImageSimilarityCalculator::orb_sim(m_finalProcessedSrcImage, originalProcessedImage);

        if (m_dFinalMaxSimilarty < similartyValue && c_dFakeThreshold < similartyValue)
        {
            m_dFinalMaxSimilarty = similartyValue;
            m_nFinalMaxSimilartyIndex = i;
        }
        else if (c_dFakeThreshold > similartyValue)
        {
            m_oFakeImageIndex.append(i);
        }

        QCoreApplication::processEvents();
        m_pProgressDialog->updateProgress(i + 1);
    }
}

void GVIMainFrame::dealMaskImage()
{
    cv::Mat finalMaskSrcImage = getMaskImageByPath(m_pCameraImageContainer->getCurImagePath());
    if (finalMaskSrcImage.empty())
    {
        return;
    }

    if (-1 != m_nFinalMaxSimilartyIndex)
    {
        cv::Mat finalMaskDstImage = getMaskImageByIndex(m_nFinalMaxSimilartyIndex);
        if (finalMaskDstImage.empty())
        {
            return;
        }

        double dSimilarty = GVIImageSimilarityCalculator::orb_sim(finalMaskSrcImage, finalMaskDstImage);

        if (dSimilarty < c_dFakeThreshold)
        {
            m_dFinalFakeSimilarty = m_dFinalMaxSimilarty;
            m_nFinalFakeSimilartyIndex = m_nFinalMaxSimilartyIndex;
            m_bIsFake = true;
        }
    }
    else
    {
        foreach(auto nIndex, m_oFakeImageIndex)
        {
            cv::Mat finalMaskDstImage = getMaskImageByIndex(nIndex);
            if (finalMaskDstImage.empty())
            {
                return;
            }

            cv::Mat finalMaskSrcImage = getMaskImageByPath(m_pCameraImageContainer->getCurImagePath());
            if (finalMaskSrcImage.empty())
            {
                return;
            }

            double dSimilarty = GVIImageSimilarityCalculator::orb_sim(finalMaskSrcImage, finalMaskDstImage);

            if (dSimilarty > m_dFinalFakeSimilarty && dSimilarty > c_dFakeThreshold)
            {
                m_dFinalFakeSimilarty = dSimilarty;
                m_nFinalFakeSimilartyIndex = nIndex;

                m_bIsFake = true;
            }
            else if (dSimilarty > m_dFinalFakeSimilarty && dSimilarty < c_dFakeThreshold)
            {
                m_dFinalMaxSimilarty = dSimilarty;
                m_nFinalMaxSimilartyIndex = nIndex;
            }
        }
    }
}

cv::Mat GVIMainFrame::getMaskImageByIndex(int nIndex)
{
    cv::Mat originalImage = imread(m_pCarOriginalImageContainer->getImagePath(nIndex).toStdString());
    if (originalImage.empty())
    {
        return Mat();
    }

    m_pImageProcessor->readImage(originalImage);
    if (!m_pImageProcessor->startProcess())
    {
        return Mat();
    }

    cv::Mat originalMaskImage = m_pImageProcessor->getMaskImage();
    if (originalMaskImage.empty())
    {
        return Mat();
    }

    return originalMaskImage;
}

cv::Mat GVIMainFrame::getMaskImageByPath(const QString &path)
{
    cv::Mat originalImage = imread(path.toStdString());
    if (originalImage.empty())
    {
        return Mat();
    }

    m_pImageProcessor->readImage(originalImage);
    if (!m_pImageProcessor->startProcess())
    {
        return Mat();
    }

    cv::Mat originalMaskImage = m_pImageProcessor->getMaskImage();
    if (originalMaskImage.empty())
    {
        return Mat();
    }

    return originalMaskImage;
}

cv::Mat GVIMainFrame::getProcessImageByIndex(int nIndex)
{
    cv::Mat originalImage = imread(m_pCarOriginalImageContainer->getImagePath(nIndex).toStdString());
    if (originalImage.empty())
    {
        return Mat();
    }

    m_pImageProcessor->readImage(originalImage);
    if (!m_pImageProcessor->startProcess())
    {
        return Mat();
    }

    cv::Mat originalProcessedImage = m_pImageProcessor->getProcessedImage(c_dPlateRegion);
    if (originalProcessedImage.empty())
    {
        return Mat();
    }

    return originalProcessedImage;
}

cv::Mat GVIMainFrame::getProcessImageByPath(const QString &path)
{
    cv::Mat originalImage = imread(path.toStdString());
    if (originalImage.empty())
    {
        return Mat();
    }

    m_pImageProcessor->readImage(originalImage);
    if (!m_pImageProcessor->startProcess())
    {
        return Mat();
    }

    cv::Mat originalProcessedImage = m_pImageProcessor->getProcessedImage(c_dPlateRegion);
    if (originalProcessedImage.empty())
    {
        return Mat();
    }

    return originalProcessedImage;
}

void GVIMainFrame::doShowMatchResult()
{
    updateIdentifyFrameInfo();

    showIdentifyFrame();

    showMessageBox();
}

void GVIMainFrame::updateIdentifyFrameInfo()
{
    if (!canShowIdentifyFrame())
    {
        return;
    }

    GVIImageConverter converter;
    m_pIdentifyFrame->setDstImage(converter.converterQImage(m_finalProcessedSrcImage, m_pVideoImageLabel->size()));

    if (m_bIsFake)
    {
        m_pIdentifyFrame->setTips(QString("The Similarity is %1%.").arg(QString::number((m_dFinalFakeSimilarty * 100), 'f', 2)));
        m_pIdentifyFrame->setMatchImage(m_pCarOriginalImageContainer->getImage(m_nFinalFakeSimilartyIndex));
    }
    else
    {
        m_pIdentifyFrame->setTips(QString("The Similarity is %1%.").arg(QString::number((m_dFinalMaxSimilarty * 100), 'f', 2)));
        m_pIdentifyFrame->setMatchImage(m_pCarOriginalImageContainer->getImage(m_nFinalMaxSimilartyIndex));
    }
}

void GVIMainFrame::showMessageBox()
{
    if (!canShowIdentifyFrame())
    {
        return;
    }

    if (m_bIsFake)
    {
        if (m_dFinalFakeSimilarty > c_dFakeThreshold)
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