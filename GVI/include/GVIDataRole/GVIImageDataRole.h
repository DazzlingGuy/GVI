#ifndef _GVIIMAGEDATAROLE_H
#define _GVIIMAGEDATAROLE_H

#include "OpenCV/cv.hpp"

#include <QString>
#include <QVector>

using namespace cv;

class GVIImageProcessor;
class GVIImageContainer;
class GVIProgressDialog;

class QImage;

class GVIImageDataRole
{
public:
    GVIImageDataRole(const QString &fileName);
    ~GVIImageDataRole();

    double getFinalMaxSimilarty();
    double getFinalFakeSimilarty();

    QImage getFinalMaxSimilartyImage();
    QImage getFinalFakeSimilartyImage();

    Mat getFinalSrcProcessedImage();

    Mat getFinalSrcMarkImage();

    int getComparisonCount();

    bool isFake();
    bool isComparison();

private:
    void init();

    void initSrcData();
    void calcPlateSimilarty();
    void calcMaskSimilarty();

    bool canShowIdentifyFrame();

    void createProgressDialog();
    void updateProgressDialog(int nIndex);
    void freeProgressDialog();

private:
    QString m_sFileName;

    GVIImageProcessor *m_pSrcImageProcessor;
    GVIImageProcessor *m_pDstImageProcessor;

    GVIProgressDialog *m_pProgressDialog;

    GVIImageContainer *m_pCarOriginalImageContainer;

    QVector<int> m_oLikeFakeImageIndex;

    bool m_bIsFake;
    bool m_bIsComparison;

    double m_dFinalMaxSimilarty;
    double m_dFinalFakeSimilarty;

    int m_nFinalMaxSimilartyIndex;
    int m_nFinalFakeSimilartyIndex;
};

#endif // _GVIIMAGEDATAROLE_H