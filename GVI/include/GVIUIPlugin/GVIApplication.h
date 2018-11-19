#ifndef _GVIAPPLICATION_H
#define _GVIAPPLICATION_H

#include <QWidget>

#include "GVIMainFrame.h"
#include "GVIBaseFrame.h"
#include "easypr.h"

class QPushButton;

class GVIApplication : public GVIBaseFrame
{
    Q_OBJECT

public:
    GVIApplication(QWidget *parent = Q_NULLPTR);

private:
    void init();
    void initControl();
    void initStyle();
    void initLayout();
    void initSlots();

protected:
    void mouseMoveEvent(QMouseEvent *event) override;

    protected slots:
    void onMiniBtnClicked();
    void onCloseBtnClicked();

private:
    QWidget *m_pLogoWidget;

    QWidget *m_pBtnWidget;
    QPushButton *m_pMinimunBtn;
    QPushButton *m_pCloseBtn;

    GVIMainFrame *m_pMainWidget;
};

#endif // _GVIAPPLICATION_H