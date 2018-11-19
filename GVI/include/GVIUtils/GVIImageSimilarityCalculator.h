#ifndef _GVIIMAGESIMILARITYCALCULATOR_H
#define _GVIIMAGESIMILARITYCALCULATOR_H

#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

class GVIImageSimilarityCalculator
{
public:
    GVIImageSimilarityCalculator();
    ~GVIImageSimilarityCalculator();

    static double pixel_sim(const Mat &srcImageInput, const Mat &dstImageInput);
    static double orb_sim(const Mat &srcImage, const Mat &dstImage);
};

#endif // _GVIIMAGESIMILARITYCALCULATOR_H