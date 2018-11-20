#include "GVIImageDataRole.h"
#include "GVIImageProcessor.h"
#include "GVIImageSimilarityCalculator.h"
#include "GVIConsts.h"
#include "GVIImageContainer.h"

#include <QImage>

const double c_dFakeThreshold = 0.7;

GVIImageDataRole::GVIImageDataRole(const QString &fileName)
    : m_pCarOriginalImageContainer(new GVIImageContainer(gviimagepath::strCarOriginalImages))
    , m_pSrcImageProcessor(new GVIImageProcessor())
    , m_pDstImageProcessor(new GVIImageProcessor())
    , m_bIsFake(false)
    , m_bIsComparison(false)
    , m_dFinalMaxSimilarty(0.0)
    , m_dFinalFakeSimilarty(0.0)
    , m_nFinalMaxSimilartyIndex(-1)
    , m_nFinalFakeSimilartyIndex(-1)
    , m_sFileName(fileName)
{
    init();
}

GVIImageDataRole::~GVIImageDataRole()
{

}

double GVIImageDataRole::getFinalMaxSimilarty()
{
    return m_dFinalMaxSimilarty;
}

double GVIImageDataRole::getFinalFakeSimilarty()
{
    return m_dFinalFakeSimilarty;
}

QImage GVIImageDataRole::getFinalMaxSimilartyImage()
{
    return m_pCarOriginalImageContainer->getImage(m_nFinalMaxSimilartyIndex);
}

QImage GVIImageDataRole::getFinalFakeSimilartyImage()
{
    return m_pCarOriginalImageContainer->getImage(m_nFinalFakeSimilartyIndex);
}

cv::Mat GVIImageDataRole::getFinalSrcProcessedImage()
{
    return m_pSrcImageProcessor->getProcessedImage();
}

int GVIImageDataRole::getComparisonCount()
{
    return m_pCarOriginalImageContainer->getImagesCount();
}

bool GVIImageDataRole::isFake()
{
    return m_bIsFake;
}

bool GVIImageDataRole::isComparison()
{
    return m_bIsComparison;
}

void GVIImageDataRole::init()
{
    initSrcData();
    calcPlateSimilarty();
    calcMaskSimilarty();
}

void GVIImageDataRole::initSrcData()
{
    m_pSrcImageProcessor->readImage(m_sFileName);

    m_pCarOriginalImageContainer->reLoadContainer();

    m_oLikeFakeImageIndex.clear();

    m_bIsFake = false;
    m_bIsComparison = false;
}

void GVIImageDataRole::calcPlateSimilarty()
{
    cv::Mat finalProcessedSrcImage = m_pSrcImageProcessor->getProcessedImage();
    if (finalProcessedSrcImage.empty())
    {
        return;
    }

    for (size_t i = 0; i < m_pCarOriginalImageContainer->getImagesCount(); i++)
    {
        m_pDstImageProcessor->readImage(m_pCarOriginalImageContainer->getImagePath(i));

        cv::Mat finalProcessedDstImage = m_pDstImageProcessor->getProcessedImage();
        if (finalProcessedDstImage.empty())
        {
            continue;
        }

        double similartyValue = GVIImageSimilarityCalculator::orb_sim(finalProcessedSrcImage, finalProcessedDstImage);

        if (m_dFinalMaxSimilarty < similartyValue && c_dFakeThreshold < similartyValue)
        {
            m_dFinalMaxSimilarty = similartyValue;
            m_nFinalMaxSimilartyIndex = i;
        }
        else if (c_dFakeThreshold > similartyValue)
        {
            m_oLikeFakeImageIndex.append(i);
        }
    }
}

void GVIImageDataRole::calcMaskSimilarty()
{
    cv::Mat finalMaskSrcImage = m_pSrcImageProcessor->getMaskImage();
    if (finalMaskSrcImage.empty())
    {
        return;
    }

    if (-1 != m_nFinalMaxSimilartyIndex)
    {
        m_pDstImageProcessor->readImage(m_pCarOriginalImageContainer->getImagePath(m_nFinalMaxSimilartyIndex));

        cv::Mat finalMaskDstImage = m_pDstImageProcessor->getMaskImage();
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
        foreach(auto nIndex, m_oLikeFakeImageIndex)
        {
            m_pDstImageProcessor->readImage(m_pCarOriginalImageContainer->getImagePath(nIndex));

            cv::Mat finalMaskDstImage = m_pDstImageProcessor->getMaskImage();
            if (finalMaskDstImage.empty())
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

    m_bIsComparison = true;
}
