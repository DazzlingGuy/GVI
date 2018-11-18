#include "GVIDateTime.h"

#include <time.h>
#include <sstream>

GVIDateTime::GVIDateTime()
{
}

GVIDateTime::~GVIDateTime()
{
}

std::string GVIDateTime::getCurrentTime()
{
    time_t nowTimeBit = time(nullptr);

    tm *localTimeReal = localtime(&nowTimeBit);

    std::stringstream strTimeStream;

    strTimeStream
        << (1900 + localTimeReal->tm_year)
        << (1 + localTimeReal->tm_mon)
        << localTimeReal->tm_mday
        << localTimeReal->tm_hour
        << localTimeReal->tm_min
        << localTimeReal->tm_sec;

    std::string str = strTimeStream.str();

    return str;
}