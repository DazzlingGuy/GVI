#ifndef _GVIMAINFRAME_H
#define _GVIMAINFRAME_H

#include <QWidget>

#include "OpenCV/cv.hpp"

class QLabel;
class QHBoxLayout;
class QVBoxLayout;

class GVIPushButton;
class GVIImageContainer;
class GVIIdentifyResultFrame;
class GVIVideoProcessor;
class GVIDateTime;
class GVIImageProcessor;
class GVIProgressDialog;

class GVIMainFrame : public QWidget
{
    Q_OBJECT

public:
    explicit GVIMainFrame(QWidget *parent = nullptr);
    virtual ~GVIMainFrame();

    // TODO: using connect signal slot
    void refreshIdentifyFramePosition();

protected:
    void init();

    void loadNextImage();
    void loadPrevImage();

    protected slots:
    void onStartClicked();
    void onEndClicked();
    void onPhotoClicked();

    void onIdentifyClicked();
    void onPrevClicked();
    void onNextClicked();

    void onRefreshVideoImage();

private:
    void initStyle();
    void initControl();
    void initLayout();
    void initSlots();
    void initImageContainer();

    bool isNeedTakePhoto();

    cv::Mat getMaskImageByIndex(int nIndex);
    cv::Mat getMaskImageByPath(const QString &path);
    cv::Mat getProcessImageByIndex(int nIndex);
    cv::Mat getProcessImageByPath(const QString &path);

    void reLoad();
    void reLoadData();
    void reLoadImageContainer();

    void createProgressDialog();

    void doSimilartyMatch();
    void processImage();
    void markSrcImage(cv::Mat srcImage);
    void calcSimilarty();
    void dealMaskImage();

    void doShowMatchResult();
    void updateIdentifyFrameInfo();
    void showMessageBox();

    void showIdentifyFrame();
    void hideIdentifyFrame();
    void moveIdentifyFrame();

    void freeProcessDialog();

    inline QHBoxLayout* createCommonHBoxLayout(QWidget *parent = nullptr);
    inline QVBoxLayout* createCommonVBoxLayout(QWidget *parent = nullptr);

private:
    QLabel *m_pVideoImageLabel;
    QLabel *m_pMatchImageLabel;

    GVIPushButton *m_pStartBtn;
    GVIPushButton *m_pEndBtn;
    GVIPushButton *m_pPhotoBtn;

    GVIPushButton *m_pIdentifyBtn;
    GVIPushButton *m_pPrevBtn;
    GVIPushButton *m_pNextBtn;

    GVIImageContainer *m_pCameraImageContainer;
    GVIImageContainer *m_pCarOriginalImageContainer;

    GVIVideoProcessor *m_pVideoProcessor;
    GVIImageProcessor *m_pImageProcessor;

    GVIDateTime *m_pDateTime;

    QTimer *m_qTimer;

    GVIIdentifyResultFrame *m_pIdentifyFrame;
    GVIProgressDialog *m_pProgressDialog;

    bool m_bIsNeedPhoto;
    bool m_bIsPlay;

    bool m_bIsFake;

    cv::Mat m_finalProcessedSrcImage;

    QVector<int> m_oFakeImageIndex;

    int m_nFinalMaxSimilartyIndex;
    double m_dFinalMaxSimilarty;

    int m_nFinalFakeSimilartyIndex;
    double m_dFinalFakeSimilarty;
};

#endif // _GVIMAINFRAME_H