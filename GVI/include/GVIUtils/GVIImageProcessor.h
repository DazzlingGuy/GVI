#ifndef _GVIIMAGEPROCESSOR_H
#define _GVIIMAGEPROCESSOR_H

#include <opencv2\opencv.hpp>
#include "easypr.h"

class GVIImageProcessor
{
public:
    GVIImageProcessor();
    ~GVIImageProcessor();

    void init();

    void readImage(string _path);
    void readImage(Mat _Image);

    bool startProcess();

    Mat getProcessedImage(double scalingFactor = 2);
    Rect getProcessedRegion(double scalingFactor = 2);
    Mat getMaskImage();

    Mat getDstImage();
    Mat getSrcImage();

    string getLicense();

private:
    bool doProcess(int &numPlate);
    Rect getRealPlateRegion();

private:
    Mat m_srcImage;
    Mat m_dstImage;

    easypr::CPlateRecognize m_PlateRecognize;

    vector<easypr::CPlate> m_oPlate;

    string m_sLicense;
};

#endif // _GVIIMAGEPROCESSOR_H