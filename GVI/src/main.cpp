#include "GVIApplication.h"
#include "GVIConsts.h"

#include <QtWidgets/QApplication>
#include <QIcon>

/*
 *  TODO List
 *  Program optimization
 *  1.Efficiency optimization
 *  2.Algorithm reconstruction
 *  3.Data encapsulation
 *
 *  Author: rensl
 **/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GVIApplication w;

    QString strIcon = QCoreApplication::applicationDirPath() + gviimagepath::strAppIcon;
    a.setWindowIcon(QIcon(strIcon));

    w.show();
    return a.exec();
}