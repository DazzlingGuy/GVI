#include "GVIImageProcessor.h"
#include "config.h"

#include <QCoreApplication>
#include <QString>

const double c_dPlateRegion = 1.8;

GVIImageProcessor::GVIImageProcessor()
{
    init();
}

GVIImageProcessor::~GVIImageProcessor()
{

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

void GVIImageProcessor::readImage(const QString &path)
{
    initMatData();

    if (path.isNull())
    {
        return;
    }

    m_srcImage = imread(path.toStdString()).clone();

    if (m_srcImage.empty())
    {
        return;
    }

    startProcess();
}

void GVIImageProcessor::readImage(const Mat &image)
{
    initMatData();

    m_srcImage = image.clone();

    if (m_srcImage.empty())
    {
        return;
    }

    startProcess();
}

Mat GVIImageProcessor::getSrcImage()
{
    return m_srcImage;
}

Mat GVIImageProcessor::getProcessedImage()
{
    return m_srcProcessedImage;
}

Rect GVIImageProcessor::getProcessedRegion()
{
    return m_srcProcessedRegion;
}

Mat GVIImageProcessor::getMaskImage()
{
    return m_srcMaskImage;
}

Mat GVIImageProcessor::getMarkImage()
{
    return m_srcMarkImage;
}

QString GVIImageProcessor::getLicense()
{
    return m_sLicense;
}

void GVIImageProcessor::initMatData()
{
    m_srcImage.release();
    m_srcProcessedImage.release();
    m_srcMaskImage.release();

    m_sLicense.clear();

    m_oPlate.clear();
}

bool GVIImageProcessor::startProcess()
{
    if (doProcess())
    {
        return false;
    }

    if (m_oPlate.empty())
    {
        return false;
    }

    m_srcMaskImage = doGetMaskImage();
    m_srcMarkImage = doGetMarkImage();
    m_srcProcessedRegion = doGetProcessedRegion(c_dPlateRegion);
    m_srcProcessedImage = doGetProcessedImage(c_dPlateRegion);

    m_sLicense = QString::fromStdString(m_oPlate.at(0).getPlateStr());

    return true;
}

bool GVIImageProcessor::doProcess()
{
    return m_PlateRecognize.plateRecognize(m_srcImage, m_oPlate);
}

Mat GVIImageProcessor::doGetProcessedImage(double scalingFactor)
{
    Rect plateRect = doGetRealPlateRegion();

    int nVertexX = (plateRect.tl().x + plateRect.width / 2) - scalingFactor * plateRect.width / 2;
    int nVertexY = (plateRect.tl().y + plateRect.height / 2) - scalingFactor * plateRect.height / 2;

    int nWidth = scalingFactor * plateRect.width;
    int nHeight = scalingFactor * plateRect.height;

    Rect srcImageRect = Rect(0, 0, m_srcImage.cols, m_srcImage.rows);
    Rect scalingPlateRect = srcImageRect & Rect(nVertexX, nVertexY, nWidth, nHeight);

    cv::Mat returnMat = m_srcImage.clone();

    return returnMat(scalingPlateRect);
}

Rect GVIImageProcessor::doGetProcessedRegion(double scalingFactor)
{
    cv::Mat result;

    Rect plateRect = doGetRealPlateRegion();

    int nVertexX = (plateRect.tl().x + plateRect.width / 2) - scalingFactor * plateRect.width / 2;
    int nVertexY = (plateRect.tl().y + plateRect.height / 2) - scalingFactor * plateRect.height / 2;

    int nWidth = scalingFactor * plateRect.width;
    int nHeight = scalingFactor * plateRect.height;

    Rect srcImageRect = Rect(0, 0, m_srcImage.cols, m_srcImage.rows);

    return srcImageRect & Rect(nVertexX, nVertexY, nWidth, nHeight);
}

Mat GVIImageProcessor::doGetMaskImage()
{
    Rect plateRect = doGetRealPlateRegion();

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

cv::Mat GVIImageProcessor::doGetMarkImage()
{
    cv::Mat markMat = m_srcImage.clone();

    Rect plateRegion = doGetProcessedRegion(c_dPlateRegion);
    rectangle(markMat, plateRegion, Scalar(0, 0, 255), 3);

    return markMat;
}

Rect GVIImageProcessor::doGetRealPlateRegion()
{
    return m_oPlate.at(0).getPlatePos().boundingRect();
}