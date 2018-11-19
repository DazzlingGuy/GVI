#include "GVIImageProcessor.h"
#include "config.h"

#include <QCoreApplication>

GVIImageProcessor::GVIImageProcessor()
{
    init();
}

GVIImageProcessor::~GVIImageProcessor()
{
    if (!m_oPlate.empty())
    {
        m_oPlate.clear();
    }

    if (!m_sLicense.empty())
    {
        m_sLicense.clear();
    }
}

void GVIImageProcessor::init()
{
    m_PlateRecognize.setResultShow(false);
    m_PlateRecognize.setLifemode(true);
    m_PlateRecognize.setDetectType(easypr::PR_DETECT_COLOR | easypr::PR_DETECT_CMSER);
    m_PlateRecognize.setMaxPlates(4);

    m_PlateRecognize.LoadSVM(qApp->applicationDirPath().toStdString() + easypr::kDefaultSvmPath);
    m_PlateRecognize.LoadANN(qApp->applicationDirPath().toStdString() + easypr::kDefaultAnnPath);
    m_PlateRecognize.LoadChineseANN(qApp->applicationDirPath().toStdString() + easypr::kChineseAnnPath);

    m_PlateRecognize.LoadGrayChANN(qApp->applicationDirPath().toStdString() + easypr::kGrayAnnPath);
    m_PlateRecognize.LoadChineseMapping(qApp->applicationDirPath().toStdString() + easypr::kChineseMappingPath);
}

void GVIImageProcessor::readImage(string path)
{
    m_srcImage.release();
    m_dstImage.release();

    if (path.length() > 0)
    {
        m_srcImage = imread(path).clone();
    }
}

void GVIImageProcessor::readImage(Mat image)
{
    m_srcImage.release();
    m_dstImage.release();

    m_srcImage = image.clone();
}

bool GVIImageProcessor::startProcess()
{
    int nPlate = 0;
    bool bRes = false;

    if (doProcess(nPlate))
    {
        return bRes;
    }

    if (m_oPlate.empty())
    {
        return bRes;
    }

    easypr::CPlate plate = m_oPlate.at(0);
    m_dstImage = plate.getPlateMat();

    if (m_sLicense.length() > 0)
    {
        m_sLicense.clear();
    }

    m_sLicense = plate.getPlateStr();

    return true;
}

bool GVIImageProcessor::doProcess(int &nPlate)
{
    if (!m_oPlate.empty())
    {
        m_oPlate.clear();
    }

    nPlate = m_PlateRecognize.plateRecognize(m_srcImage, m_oPlate);

    if (nPlate != 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

Mat GVIImageProcessor::getProcessedImage(double scalingFactor)
{
    Rect plateRect = getRealPlateRegion();

    int nVertexX = (plateRect.tl().x + plateRect.width / 2) - scalingFactor * plateRect.width / 2;
    int nVertexY = (plateRect.tl().y + plateRect.height / 2) - scalingFactor * plateRect.height / 2;

    int nWidth = scalingFactor * plateRect.width;
    int nHeight = scalingFactor * plateRect.height;

    Rect srcImageRect = Rect(0, 0, m_srcImage.cols, m_srcImage.rows);
    Rect scalingPlateRect = srcImageRect & Rect(nVertexX, nVertexY, nWidth, nHeight);

    cv::Mat returnMat = m_srcImage.clone();

    return returnMat(scalingPlateRect);
}

Mat GVIImageProcessor::getMaskImage()
{
    Rect plateRect = getRealPlateRegion();

    int nVertexX = (plateRect.tl().x + plateRect.width / 2) - 3 * plateRect.width / 2;
    int nVertexY = (plateRect.tl().y + plateRect.height / 2) - 3 * plateRect.height / 2;

    int nWidth = 3 * plateRect.width;
    int nHeight = 3 * plateRect.height;

    Rect srcImageRect = Rect(0, 0, m_srcImage.cols, m_srcImage.rows);
    Rect scalingPlateRect = srcImageRect & Rect(nVertexX, nVertexY, nWidth, nHeight);

    cv::Mat maskImage = m_srcImage.clone();
    cv::Mat maskRectImage = maskImage(scalingPlateRect);
    maskRectImage = { Scalar(0, 0, 0) };

    return maskImage;
}

Rect GVIImageProcessor::getProcessedRegion(double scalingFactor)
{
    cv::Mat result;

    Rect plateRect = getRealPlateRegion();

    int nVertexX = (plateRect.tl().x + plateRect.width / 2) - scalingFactor * plateRect.width / 2;
    int nVertexY = (plateRect.tl().y + plateRect.height / 2) - scalingFactor * plateRect.height / 2;

    int nWidth = scalingFactor * plateRect.width;
    int nHeight = scalingFactor * plateRect.height;

    Rect srcImageRect = Rect(0, 0, m_srcImage.cols, m_srcImage.rows);

    return srcImageRect & Rect(nVertexX, nVertexY, nWidth, nHeight);
}

Mat GVIImageProcessor::getSrcImage()
{
    return m_srcImage;
}

Mat GVIImageProcessor::getDstImage()
{
    return m_dstImage;
}

string GVIImageProcessor::getLicense()
{
    return m_sLicense;
}

Rect GVIImageProcessor::getRealPlateRegion()
{
    return m_oPlate.at(0).getPlatePos().boundingRect();
}