#include "GVIPushButton.h"

const int c_nMainWindownHeight = 30;
const int c_nMainWindownWidth = 100;

GVIPushButton::GVIPushButton(QString strName)
    : QPushButton(strName)
{
    setObjectName("GVIPushButton");
    this->setMinimumSize(c_nMainWindownWidth, c_nMainWindownHeight);
}

GVIPushButton::~GVIPushButton()
{
}