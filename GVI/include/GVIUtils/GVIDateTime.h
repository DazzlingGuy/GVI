#ifndef _GVIDATETIME_H
#define _GVIDATETIME_H

#include <QString>

class GVIDateTime
{
public:
    GVIDateTime();
    ~GVIDateTime();

    static QString getCurrentTime();
};

#endif // _GVIDATETIME_H