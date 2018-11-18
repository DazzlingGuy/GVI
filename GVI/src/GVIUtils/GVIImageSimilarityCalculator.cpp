#include "GVIImageSimilarityCalculator.h"

#include <iostream>

#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

GVIImageSimilarityCalculator::GVIImageSimilarityCalculator()
{
}

GVIImageSimilarityCalculator::~GVIImageSimilarityCalculator()
{
}

double GVIImageSimilarityCalculator::pixel_sim(Mat srcImage, Mat dstImage)
{
    cvtColor(srcImage, srcImage, CV_BGR2BGRA);
    cvtColor(dstImage, dstImage, CV_BGR2BGRA);

    Mat resultImage;

    srcImage.resize(dstImage.rows, dstImage.cols);
    absdiff(srcImage, dstImage, resultImage);
    threshold(resultImage, resultImage, 5, 255.0, CV_THRESH_BINARY);

    double counter = 0;
    for (int i = 0; i < resultImage.rows; i++)
    {
        auto *data = resultImage.ptr<uchar>(i);

        for (int j = 0; j < resultImage.cols; j++)
        {
            if (data[j] == 255)
            {
                counter++;
            }
        }
    }

    return (1 - counter * 10 / (srcImage.rows*srcImage.cols));
}

double GVIImageSimilarityCalculator::orb_sim(Mat srcImage, Mat dstImage)
{
    Ptr<ORB> orb = ORB::create();

    vector<KeyPoint> srcKeyPoint, dstKeyPoint;
    orb->detect(srcImage, srcKeyPoint);
    orb->detect(dstImage, dstKeyPoint);

    Mat srcComputedImage, dstComputedImage;
    orb->compute(srcImage, srcKeyPoint, srcComputedImage);
    orb->compute(dstImage, dstKeyPoint, dstComputedImage);

    if (srcKeyPoint.empty() || dstKeyPoint.empty())
    {
        return 0.0;
    }

    vector<DMatch> oMatchList;
    BFMatcher matcher(NORM_HAMMING);
    matcher.match(srcComputedImage, dstComputedImage, oMatchList);

    double nMinDistance = 0, nMaxDistance = 0;
    for (int i = 0; i < srcComputedImage.rows; ++i)
    {
        double nDistance = oMatchList[i].distance;
        if (nDistance < nMinDistance) nMinDistance = nDistance;
        if (nDistance > nMaxDistance) nMaxDistance = nDistance;
    }

    vector<DMatch> oSimilarMatchList;
    double dDefaultStandard = (nMinDistance + nMaxDistance) / 3;
    for (int j = 0; j < srcComputedImage.rows; ++j)
    {
        if (oMatchList[j].distance <= max(2 * dDefaultStandard, 30.0))
        {
            oSimilarMatchList.push_back(oMatchList[j]);
        }
    }

    return (double)oSimilarMatchList.size() / (double)oMatchList.size();
}