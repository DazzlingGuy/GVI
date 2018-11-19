#ifndef _GVIVIDEOPROCESSOR_H
#define _GVIVIDEOPROCESSOR_H

#include <opencv2/opencv.hpp>
#include <QString>

class GVIVideoProcessor
{
public:
    GVIVideoProcessor();
    ~GVIVideoProcessor();

    bool openCamera(int cameraIndex = 0);

    bool saveImage(const QString &path);

    cv::Mat getImage();

private:
    void init();

private:
    bool m_bShowGrab;

    cv::VideoCapture m_capture;

    cv::Mat m_image;
};

#endif // _GVIVIDEOPROCESSOR_H