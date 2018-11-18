#ifndef _GVIIMAGESIMILARITYCALCULATOR_H
#define _GVIIMAGESIMILARITYCALCULATOR_H

#include "opencv2/opencv.hpp"

class GVIImageSimilarityCalculator
{
public:
    GVIImageSimilarityCalculator();
    ~GVIImageSimilarityCalculator();

    double pixel_sim(cv::Mat srcImage, cv::Mat dstImage);
    double orb_sim(cv::Mat srcImage, cv::Mat dstImage);
};

#endif // _GVIIMAGESIMILARITYCALCULATOR_H