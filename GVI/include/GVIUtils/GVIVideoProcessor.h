#ifndef _GVIVIDEOPROCESSOR_H
#define _GVIVIDEOPROCESSOR_H

#include <opencv2/opencv.hpp>

class GVIVideoProcessor
{
public:
    GVIVideoProcessor();
    ~GVIVideoProcessor();

    void init();

    bool openCamera(int cameraIndex = 0);

    bool saveImage(std::string &path);
    cv::Mat getImage();

private:
    bool m_bShowGrab;

    cv::VideoCapture m_capture;
    cv::Mat m_image;
};

#endif // _GVIVIDEOPROCESSOR_H