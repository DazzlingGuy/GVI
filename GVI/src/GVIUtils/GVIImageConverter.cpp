#include "GVIImageConverter.h"

GVIImageConverter::GVIImageConverter()
    : m_bConverted(false)
    , m_imageSize(cvSize(0, 0))
    , m_wndSize(cvSize(0, 0))
{
}

GVIImageConverter::~GVIImageConverter()
{
}

bool GVIImageConverter::importImageByFileName(const char *filename)
{
    if (filename)
    {
        m_orignImage = imread(filename, 1);
        m_imageSize = m_orignImage.size();
        m_bConverted = false;

        return true;
    }
    else
    {
        return false;
    }
}

bool GVIImageConverter::importImageByMatImage(const cv::Mat &inputImage)
{
    m_orignImage = inputImage;
    m_imageSize = m_orignImage.size();
    m_bConverted = false;

    return true;
}

QImage* GVIImageConverter::exportImage(const QSize &windowSize)
{
    m_wndSize.width = windowSize.width();
    m_wndSize.height = windowSize.height();

    m_convertImage = Mat(m_wndSize.width, m_wndSize.height, CV_8UC1);

    resize(m_orignImage, m_convertImage, m_wndSize);

    if (m_orignImage.channels() == 1)
    {
        m_outputImage = QImage((const unsigned char*)(m_convertImage.data),
            m_convertImage.cols,
            m_convertImage.rows,
            m_convertImage.cols * m_convertImage.channels(),
            QImage::Format_Indexed8);
    }
    else
    {
        cvtColor(m_convertImage, m_convertImage, CV_BGR2RGB);

        m_outputImage = QImage((const unsigned char*)(m_convertImage.data),
            m_convertImage.cols,
            m_convertImage.rows,
            m_convertImage.cols * m_convertImage.channels(),
            QImage::Format_RGB888);
    }

    m_bConverted = true;

    return &m_outputImage;
}