#include "GVIImageConverter.h"

#include <QImage>
#include <QString>
#include <QSize>

using namespace cv;

GVIImageConverter::GVIImageConverter()
    : m_srcImageSize(cvSize(0, 0))
{
}

GVIImageConverter::~GVIImageConverter()
{
}

QImage GVIImageConverter::converterQImage(const QString &fileName, const QSize &wndSize)
{
    if (!importImage(fileName))
    {
        return QImage();
    }

    return doConverterQImage(wndSize);
}

QImage GVIImageConverter::converterQImage(const cv::Mat &inputImage, const QSize &wndSize)
{
    if (!importImage(inputImage))
    {
        return QImage();
    }

    return doConverterQImage(wndSize);
}

bool GVIImageConverter::importImage(const QString &fileName)
{
    if (fileName.isNull())
    {
        return false;
    }

    m_srcImage = imread(fileName.toStdString(), 1);
    m_srcImageSize = m_srcImage.size();

    return true;
}

bool GVIImageConverter::importImage(const cv::Mat &inputImage)
{
    if (inputImage.empty())
    {
        return false;
    }

    m_srcImage = inputImage;
    m_srcImageSize = m_srcImage.size();

    return true;
}

QImage GVIImageConverter::doConverterQImage(const QSize &windowSize)
{
    CvSize m_wndSize;
    m_wndSize.width = windowSize.width();
    m_wndSize.height = windowSize.height();

    m_convertImage = Mat(m_wndSize.width, m_wndSize.height, CV_8UC1);

    resize(m_srcImage, m_convertImage, m_wndSize);

    if (m_srcImage.channels() == 1)
    {
        return QImage((const unsigned char*)(m_convertImage.data),
            m_convertImage.cols,
            m_convertImage.rows,
            m_convertImage.cols * m_convertImage.channels(),
            QImage::Format_Indexed8);
    }
    else
    {
        cvtColor(m_convertImage, m_convertImage, CV_BGR2RGB);

        return QImage((const unsigned char*)(m_convertImage.data),
            m_convertImage.cols,
            m_convertImage.rows,
            m_convertImage.cols * m_convertImage.channels(),
            QImage::Format_RGB888);
    }
}