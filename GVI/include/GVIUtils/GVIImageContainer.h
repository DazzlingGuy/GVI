#ifndef _GVIIMAGECONTAINER_H
#define _GVIIMAGECONTAINER_H

#include <QList>
#include <QImage>
#include <QMap>

using Images = QList<QImage>;

class GVIImageContainer
{
public:
    explicit GVIImageContainer(const QString &dirPath);

    QImage getImage(int nIndex);
    QString getImagePath(int nIndex);

    QImage getCurImage();
    QString getCurImagePath();

    QImage getNextImage();
    QString getNextImagePath();

    QImage getPrevImage();
    QString getPrevImagePath();

    int getImagesCount();

    bool isEmpty();

    void setCurImage(int nIndex);

    void insertImage(const QImage &image, const QString &imagePath);
    void removeImage(const QImage &image, int nIndex);

    void reLoadImage();

private:
    void doLoadImages();

private:
    Images m_oImages;

    QMap<int, QString> m_oMapIndexToImagePath;

    int m_nCurIndex;

    QString m_sDirPath;
};

#endif  // _GVIIMAGECONTAINER_H