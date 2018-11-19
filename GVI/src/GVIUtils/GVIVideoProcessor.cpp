#include "GVIVideoProcessor.h"

#include <QCoreApplication>

const int c_nPicturePixel = 465;

GVIVideoProcessor::GVIVideoProcessor()
{
    init();
}

GVIVideoProcessor::~GVIVideoProcessor()
{
}

void GVIVideoProcessor::init()
{
    m_capture.set(CV_CAP_PROP_FRAME_WIDTH, c_nPicturePixel);
    m_capture.set(CV_CAP_PROP_FRAME_HEIGHT, c_nPicturePixel);
}

bool GVIVideoProcessor::openCamera(int cameraIndex)
{
    bool bRes = false;
    m_capture.open(cameraIndex);

    if (m_capture.isOpened())
    {
        bRes = true;
    }

    return bRes;
}

bool GVIVideoProcessor::saveImage(const QString &path)
{
    bool bRes = false;

    if (path.isNull() || m_image.empty())
    {
        return bRes;
    }

    return imwrite(path.toStdString(), m_image);
}

cv::Mat GVIVideoProcessor::getImage()
{
    m_capture >> m_image;

    return m_image;
}