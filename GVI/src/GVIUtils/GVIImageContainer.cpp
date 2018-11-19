#include "GVIImageContainer.h"
#include "GVIConsts.h"
#include <QCoreApplication>

#include <QDir>

GVIImageContainer::GVIImageContainer(const QString &dirPath)
    : m_nCurIndex(-1)
    , m_sDirPath(dirPath)
{
    doLoadContainer();
}

void GVIImageContainer::doLoadContainer()
{
    QString strOriginalImagesPath = qApp->applicationDirPath() + m_sDirPath;
    QDir originalImagesDir(strOriginalImagesPath);
    if (!originalImagesDir.exists())
        return;

    QStringList filters;
    filters << QString("*.png") << QString("*.jpg");
    originalImagesDir.setFilter(QDir::Files | QDir::NoSymLinks);
    originalImagesDir.setNameFilters(filters);

    int filterCount = originalImagesDir.count();
    if (filterCount <= 0)
    {
        return;
    }

    QStringList oFilesList;
    for (int i = 0; i < filterCount; i++)
    {
        QImage image(strOriginalImagesPath + originalImagesDir[i]);
        m_oMapIndexToImagePath.insert(i, strOriginalImagesPath + originalImagesDir[i]);
        m_oImages.append(image);
    }
}

QImage GVIImageContainer::getImage(int nIndex)
{
    return m_oImages.at(nIndex);
}

QString GVIImageContainer::getImagePath(int nIndex)
{
    return m_oMapIndexToImagePath.value(nIndex);
}

QImage GVIImageContainer::getCurImage()
{
    if (m_oImages.isEmpty() || -1 == m_nCurIndex)
    {
        return QImage();
    }

    return m_oImages.at(m_nCurIndex);
}

QString GVIImageContainer::getCurImagePath()
{
    return m_oMapIndexToImagePath.value(m_nCurIndex);
}

QImage GVIImageContainer::getNextImage()
{
    if (m_oImages.isEmpty())
    {
        return QImage();
    }

    ++m_nCurIndex;

    if (m_nCurIndex == m_oImages.count())
    {
        m_nCurIndex = 0;
    }

    return m_oImages.at(m_nCurIndex);
}

QString GVIImageContainer::getNextImagePath()
{
    if (m_oImages.isEmpty())
    {
        return "";
    }

    ++m_nCurIndex;

    if (m_nCurIndex == m_oImages.count())
    {
        m_nCurIndex = 0;
    }

    return m_oMapIndexToImagePath.value(m_nCurIndex);
}

QImage GVIImageContainer::getPrevImage()
{
    if (m_oImages.isEmpty())
    {
        return QImage();
    }

    --m_nCurIndex;

    if (m_nCurIndex == -1 || m_nCurIndex == -2)
    {
        m_nCurIndex = m_oImages.count() - 1;
    }

    return m_oImages.at(m_nCurIndex);
}

QString GVIImageContainer::getPrevImagePath()
{
    if (m_oImages.isEmpty())
    {
        return "";
    }

    --m_nCurIndex;

    if (m_nCurIndex == -1 || m_nCurIndex == -2)
    {
        m_nCurIndex = m_oImages.count() - 1;
    }

    return m_oMapIndexToImagePath.value(m_nCurIndex);
}

int GVIImageContainer::getImagesCount()
{
    return m_oImages.count();
}

bool GVIImageContainer::isEmpty()
{
    return m_oImages.isEmpty();
}

void GVIImageContainer::setCurImage(int nIndex)
{
    m_nCurIndex = nIndex;
}

void GVIImageContainer::insertImage(const QImage &image, const QString &imagePath)
{
    m_oImages.append(image);
    m_oMapIndexToImagePath.insert(m_oMapIndexToImagePath.count(), imagePath);
}

void GVIImageContainer::removeImage(const QImage &image, int nIndex)
{
    m_oImages.removeAt(nIndex);
    m_oMapIndexToImagePath.remove(nIndex);
}

void GVIImageContainer::reLoadContainer()
{
    m_oImages.clear();
    m_oMapIndexToImagePath.clear();
    m_nCurIndex = -1;

    doLoadContainer();
}