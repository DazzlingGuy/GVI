#ifndef _GVIIMAGEPROCESSOR_H
#define _GVIIMAGEPROCESSOR_H

#include <opencv2\opencv.hpp>
#include "easypr.h"

#include <QString>

class GVIImageProcessor
{
public:
    GVIImageProcessor();
    ~GVIImageProcessor();

    void readImage(const QString &path);
    void readImage(const Mat &Image);

    Mat getSrcImage();

    Mat getProcessedImage();
    Rect getProcessedRegion();

    Mat getMaskImage();
    Mat getMarkImage();

    QString getLicense();

private:
    void init();
    void initMatData();

    bool startProcess();
    bool doProcess();

    Mat doGetProcessedImage(double scalingFactor);
    Rect doGetProcessedRegion(double scalingFactor);

    Mat doGetMaskImage();

    Mat doGetMarkImage();

    Rect doGetRealPlateRegion();

private:
    Mat m_srcImage;
    Mat m_srcMaskImage;
    Mat m_srcProcessedImage;
    Mat m_srcMarkImage;

    Rect m_srcProcessedRegion;

    easypr::CPlateRecognize m_PlateRecognize;

    vector<easypr::CPlate> m_oPlate;

    QString m_sLicense;
};

#endif // _GVIIMAGEPROCESSOR_H