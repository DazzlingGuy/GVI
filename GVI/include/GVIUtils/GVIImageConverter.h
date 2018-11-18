#ifndef _GVIIMAGECONVERTER_H
#define _GVIIMAGECONVERTER_H

#include <opencv2\opencv.hpp>
#include <QtWidgets/qmainwindow.h>

using cv::Mat;
using cv::imread;
using cv::resize;

class GVIImageConverter
{
public:
    GVIImageConverter();
    ~GVIImageConverter();

    bool importImageByFileName(const char *filename);
    bool importImageByMatImage(const cv::Mat &inputImage);

    QImage* exportImage(const QSize &wndSize);

private:
    Mat m_orignImage;
    Mat m_convertImage;

    QImage m_outputImage;

    CvSize m_imageSize;
    CvSize m_wndSize;

    bool m_bConverted;
};

#endif // _GVIIMAGECONVERTER_H