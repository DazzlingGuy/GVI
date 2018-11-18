#include "GVIApplication.h"
#include "GVIConsts.h"

#include <QCoreApplication>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QMovie>

const int c_nMainWindownShadowWidth = 8;
const int c_nMainWindownWidth = 960;
const int c_nMainWindownHeight = 670;

const int c_nToolBtnHeight = 30;
const int c_nToolBtnWidth = 40;

GVIApplication::GVIApplication(QWidget *parent)
    : GVIBaseFrame(c_nMainWindownWidth, c_nMainWindownHeight, c_nMainWindownShadowWidth, parent)
    , m_pMinimunBtn(nullptr)
    , m_pCloseBtn(nullptr)
    , m_pBtnWidget(nullptr)
    , m_pLogoWidget(nullptr)
    , m_pMainWidget(nullptr)
{
    init();
}

void GVIApplication::init()
{
    initControl();
    initLayout();
    initStyle();
    initSlots();
}

void GVIApplication::initControl()
{
    m_pLogoWidget = new QWidget(this);

    QLabel *pGlodonLabel = new QLabel(m_pLogoWidget);
    pGlodonLabel->setFixedSize(124, 56);
    pGlodonLabel->setScaledContents(true);
    pGlodonLabel->setPixmap(QPixmap(qApp->applicationDirPath() + gviimagepath::strTeamLoge));

    QMovie *pLogoGif = new QMovie(m_pLogoWidget);
    pLogoGif->setFileName(qApp->applicationDirPath() + gviimagepath::strMovieLoge);

    QLabel *pLogoGifLabel = new QLabel(m_pLogoWidget);
    pLogoGifLabel->setMovie(pLogoGif);

    QLabel *pLogoTitle = new QLabel("Global Vehicle Identification");
    pLogoTitle->setStyleSheet(gviglobalstyle::strGlobalTitleStyle);

    QHBoxLayout *pLogoLayout = new QHBoxLayout(this);
    pLogoLayout->addSpacing(30);
    pLogoLayout->addWidget(pLogoGifLabel);
    pLogoLayout->addSpacing(30);
    pLogoLayout->addWidget(pGlodonLabel);
    pLogoLayout->addSpacing(30);
    pLogoLayout->addWidget(pLogoTitle);

    m_pLogoWidget->setLayout(pLogoLayout);

    pLogoGif->start();

    m_pBtnWidget = new QWidget(this);

    m_pMinimunBtn = new QPushButton(this);
    m_pMinimunBtn->setFixedSize(c_nToolBtnWidth, c_nToolBtnHeight);
    m_pMinimunBtn->setIcon(QIcon(qApp->applicationDirPath() + gviimagepath::strMinDefault));

    m_pCloseBtn = new QPushButton(this);
    m_pCloseBtn->setFixedSize(c_nToolBtnWidth, c_nToolBtnHeight);
    m_pCloseBtn->setIcon(QIcon(qApp->applicationDirPath() + gviimagepath::strCloseDefault));

    QHBoxLayout *pBtnLayout = new QHBoxLayout(this);
    pBtnLayout->setAlignment(Qt::AlignTop);
    pBtnLayout->setContentsMargins(0, 0, 0, 25);
    pBtnLayout->setSpacing(0);

    pBtnLayout->addStretch();
    pBtnLayout->addWidget(m_pMinimunBtn);
    pBtnLayout->addWidget(m_pCloseBtn);

    m_pBtnWidget->setLayout(pBtnLayout);

    m_pMainWidget = new GVIMainFrame(this);
}

void GVIApplication::initStyle()
{
    QString minStyleSheet = QString("QPushButton{border:none; background:url(%1);}"
        "QPushButton:hover{background:url(%2);}"
        "QPushButton:pressed{background:url(%3);}").arg(qApp->applicationDirPath() + gviimagepath::strMinDefault).arg(qApp->applicationDirPath() + gviimagepath::strMinHover).arg(qApp->applicationDirPath() + gviimagepath::strMinPressed);
    m_pMinimunBtn->setStyleSheet(minStyleSheet);

    QString closeStyleSheet = QString("QPushButton{border:none; background:url(%1);}"
        "QPushButton:hover{background:url(%2);}"
        "QPushButton:pressed{background:url(%3);}").arg(qApp->applicationDirPath() + gviimagepath::strCloseDefault).arg(qApp->applicationDirPath() + gviimagepath::strCloseHover).arg(qApp->applicationDirPath() + gviimagepath::strClosePressed);
    m_pCloseBtn->setStyleSheet(closeStyleSheet);
}

void GVIApplication::initLayout()
{
    QHBoxLayout *pTopLayout = new QHBoxLayout;
    pTopLayout->addWidget(m_pLogoWidget);
    pTopLayout->addWidget(m_pBtnWidget);

    QVBoxLayout *pMainLayout = new QVBoxLayout(this);
    pMainLayout->addSpacing(5);
    pMainLayout->addLayout(pTopLayout);
    pMainLayout->addWidget(m_pMainWidget);

    setLayout(pMainLayout);
}

void GVIApplication::initSlots()
{
    connect(m_pMinimunBtn, SIGNAL(clicked()), this, SLOT(onMiniBtnClicked()));
    connect(m_pCloseBtn, SIGNAL(clicked()), this, SLOT(onCloseBtnClicked()));
}

void GVIApplication::mouseMoveEvent(QMouseEvent *event)
{
    m_pMainWidget->refreshIdentifyFramePosition();

    GVIBaseFrame::mouseMoveEvent(event);
}

void GVIApplication::onMiniBtnClicked()
{
    this->showMinimized();
}

void GVIApplication::onCloseBtnClicked()
{
    qApp->quit();
}