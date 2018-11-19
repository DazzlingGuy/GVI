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
class GVIImageDataRole;

class GVIMainFrame : public QWidget
{
    Q_OBJECT

public:
    explicit GVIMainFrame(QWidget *parent = nullptr);
    virtual ~GVIMainFrame();

    // TODO: using connect signal slot
    void refreshIdentifyFramePosition();

protected:
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
    void init();
    void initStyle();
    void initControl();
    void initLayout();
    void initSlots();
    void initInageLabel();

    void beginImageSimilartyMatch();

    bool canShowIdentifyFrame();

    void beginShowIdentifyFrame();
    void beginUpdateIdentifyFrame();
    void beginShowIdentifyMsgBox();

    void showIdentifyFrame();
    void hideIdentifyFrame();
    void moveIdentifyFrame();

    void createProgressDialog();
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

    GVIImageDataRole *m_pImageDataRole;

    GVIImageContainer *m_pCameraImageContainer;

    GVIVideoProcessor *m_pVideoProcessor;

    GVIDateTime *m_pDateTime;

    QTimer *m_qTimer;

    GVIIdentifyResultFrame *m_pIdentifyFrame;
    GVIProgressDialog *m_pProgressDialog;

    bool m_bIsPlay;
};

#endif // _GVIMAINFRAME_H