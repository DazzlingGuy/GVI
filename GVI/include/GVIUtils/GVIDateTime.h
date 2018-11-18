#ifndef _GVIDATETIME_H
#define _GVIDATETIME_H

#include <iostream>

using namespace std;

class GVIDateTime
{
public:
    GVIDateTime();
    ~GVIDateTime();

    static std::string getCurrentTime();
};

#endif // _GVIDATETIME_H