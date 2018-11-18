#include "GVIProgressDialog.h"

#include <QtMath>
#include <QPainter>
#include <QLabel>
#include <QProgressBar>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "GVIConsts.h"

static const int c_nProgressWidth = 400;
static const int c_nProgressHeight = 220;
static const int c_nProgressShadow = 1;

GVIProgressDialog::GVIProgressDialog(const QString &strTitle, int totalNum, QWidget *parent /* = nullptr */)
    : m_pProgressBar(nullptr)
    , m_pTipsLabel(nullptr)
    , m_pProgressTextLabel(nullptr)
    , m_sTitleName(strTitle)
    , m_nTotalNum(totalNum)
    , m_point(QPoint())
    , m_bPressed(false)
{
    init();
}

GVIProgressDialog::~GVIProgressDialog()
{
}

void GVIProgressDialog::init()
{
    initAttribute();

    initFrame();

    initData();
}

void GVIProgressDialog::initAttribute()
{
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::Dialog);
    setWindowModality(Qt::WindowModal);
}

void GVIProgressDialog::initFrame()
{
    setFixedSize(c_nProgressWidth, c_nProgressHeight);

    QFont fontStyle;
    fontStyle.setFamily("Arial");
    fontStyle.setPixelSize(16);
    this->setFont(fontStyle);

    m_pTipsLabel = new QLabel(this);
    m_pTipsLabel->setFont(fontStyle);
    m_pTipsLabel->setText(m_sTitleName);

    m_pProgressBar = new QProgressBar(this);
    m_pProgressBar->setObjectName("GVIProgressBar");
    m_pProgressBar->setTextVisible(false);
    m_pProgressBar->setMaximumHeight(15);
    m_pProgressBar->setValue(0);
    m_pProgressBar->setStyleSheet(gviglobalstyle::strGlobalProgressBarStyle);

    QVBoxLayout *pProgressLayout = new QVBoxLayout;
    pProgressLayout->setAlignment(Qt::AlignCenter);
    pProgressLayout->setContentsMargins(15, 0, 15, 10);
    pProgressLayout->addWidget(m_pTipsLabel);
    pProgressLayout->setSpacing(10);
    pProgressLayout->addWidget(m_pProgressBar);

    setLayout(pProgressLayout);

    m_pProgressTextLabel = new QLabel(m_pProgressBar);
    m_pProgressTextLabel->setStyleSheet("background-color:transparent;");
    m_pProgressTextLabel->setFont(fontStyle);
    m_pProgressTextLabel->setFixedSize(100, 15);
    m_pProgressTextLabel->setAlignment(Qt::AlignCenter);

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->setContentsMargins(0, 0, 0, 0);
    hLayout->setSpacing(0);
    hLayout->addStretch();
    hLayout->addWidget(m_pProgressTextLabel);
    hLayout->addStretch();

    m_pProgressBar->setLayout(hLayout);
    m_pProgressBar->show();
}

void GVIProgressDialog::initData()
{
    m_pProgressBar->setRange(0, m_nTotalNum);
}

void GVIProgressDialog::updateProgress(int value)
{
    m_pProgressTextLabel->setText(QString::number((value * 100 / m_nTotalNum)) + "%");

    m_pProgressBar->setValue(value);
    m_pProgressBar->update();
}

void GVIProgressDialog::setTotalProgress(int value)
{
    m_nTotalNum = value;
}

void GVIProgressDialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRect(c_nProgressShadow, c_nProgressShadow, this->width() - c_nProgressShadow * 2, this->height() - c_nProgressShadow * 2);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillPath(path, QBrush(Qt::white));

    QColor color(140, 140, 140, 55);
    for (int i = 0; i != c_nProgressShadow; ++i)
    {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addRoundRect(c_nProgressShadow - i, c_nProgressShadow - i, this->width() - (c_nProgressShadow - i) * 2, this->height() - (c_nProgressShadow - i) * 2, 2);
        color.setAlpha(140 - qSqrt(i) * 50);
        painter.setPen(color);
        painter.drawPath(path);
    }
}

void GVIProgressDialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_bPressed = true;
        m_point = event->pos();
    }
}

void GVIProgressDialog::mouseMoveEvent(QMouseEvent *event)
{
    if (m_bPressed)
    {
        move(event->pos() - m_point + pos());
    }
}

void GVIProgressDialog::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_bPressed = false;
}