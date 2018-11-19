#ifndef _GVICONSTS_H
#define _GVICONSTS_H

#include <QString>

namespace gviimagepath
{
    const QString strAppIcon = "/icons/app-logo.png";
    const QString strTeamLoge = "/icons/team-logo.png";
    const QString strMovieLoge = "/icons/movie-logo.gif";

    const QString strMinDefault = "/icons/min-default.png";
    const QString strMinHover = "/icons/min-hover.png";
    const QString strMinPressed = "/icons/min-pressed.png";

    const QString strCloseDefault = "/icons/close-default.png";
    const QString strCloseHover = "/icons/close-hover.png";
    const QString strClosePressed = "/icons/close-pressed.png";

    const QString strOldImages = "/images/oldImages/";
    const QString strNewImages = "/images/newImages/";

    const QString strCameraImages = "/cameraImages/";
    const QString strCarOriginalImages = "/carOriginalImages/";
}

namespace gviglobalstyle
{
    const QString strGlobalBtnStyle = "QPushButton{font-family:Arial; font-size:16px; color: #FFFFFF;"
        "background-color: #2A85DF;border-radius: 5px}"
        "QPushButton:hover{font-family:Arial; font-size:16px; color: #FFFFFF;"
        "background-color: #529ce5;border-radius: 5px}"
        "QPushButton:pressed{font-family:Arial; font-size:16px; color: #FFFFFF;"
        "background-color: #2A85DF;border-radius: 5px}";

    const QString strGlobalTitleStyle = "QLabel{font-family:Arial; font-size:24px; color: #949494; background-color: #FFFFFF}";

    const QString strGlobalImageLabelStyle = "QLabel{background-color: #C8C8C8}";

    const QString strGlobalProgressBarStyle = "QProgressBar{border: 1px solid grey; border-radius: 5px; background-color: #FFFFFF;}"
        "QProgressBar::chunk{border: 1px solid grey; border-radius: 5px; background-color: #2A85DF;}";
}

#endif // _GVICONSTS_H