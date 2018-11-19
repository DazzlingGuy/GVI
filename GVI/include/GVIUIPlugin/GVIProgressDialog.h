#ifndef _GVIPROGRESSDIALOG_H
#define _GVIPROGRESSDIALOG_H

#include <QWidget>
#include <QMouseEvent>

class QLabel;
class QProgressBar;
class GVIPushButton;

class GVIProgressDialog : public QWidget
{
    Q_OBJECT

public:
    GVIProgressDialog(const QString &strTitle, int totalNum, QWidget *parent = nullptr);
    virtual ~GVIProgressDialog();

    void updateProgress(int value);
    void setTotalProgress(int value);

private:
    void init();
    void initAttribute();
    void initFrame();
    void initData();

protected:
    void paintEvent(QPaintEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QProgressBar *m_pProgressBar;

    QLabel *m_pTipsLabel;
    QLabel *m_pProgressTextLabel;

    QString m_sTitleName;

    int m_nTotalNum;

    QPoint m_point;

    bool m_bPressed;
};

#endif // _GVIPROGRESSDIALOG_H