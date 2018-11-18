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

bool GVIVideoProcessor::saveImage(std::string &path)
{
    bool bRes = false;
    if (path.empty() || m_image.empty())
    {
        return bRes;
    }

    path = qApp->applicationDirPath().toStdString() + "/cameraImages/" + path + ".jpg";

    bRes = imwrite(path, m_image);

    return bRes;
}

cv::Mat GVIVideoProcessor::getImage()
{
    m_capture >> m_image;

    return m_image;
}