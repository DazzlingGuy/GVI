#ifndef _GVIIMAGECONVERTER_H
#define _GVIIMAGECONVERTER_H

#include <opencv2\opencv.hpp>
#include <QtWidgets/qmainwindow.h>

class QImage;
class QString;
class QSize;

class GVIImageConverter
{
public:
    GVIImageConverter();
    ~GVIImageConverter();

    QImage converterQImage(const QString &fileName, const QSize &wndSize);
    QImage converterQImage(const cv::Mat &inputImage, const QSize &wndSize);

private:
    bool importImage(const QString &fileName);
    bool importImage(const cv::Mat &inputImage);

    QImage doConverterQImage(const QSize &wndSize);

private:
    cv::Mat m_srcImage;
    cv::Mat m_convertImage;

    CvSize m_srcImageSize;
};

#endif // _GVIIMAGECONVERTER_H