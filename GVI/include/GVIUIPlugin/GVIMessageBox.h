#pragma once

#include <QMessageBox>

class GVIMessageBox : public QMessageBox
{
    Q_OBJECT

public:
    GVIMessageBox();
    ~GVIMessageBox();

private:
    void paintEvent(QPaintEvent *event) override;
};