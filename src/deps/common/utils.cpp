#include "utils.h"

std::string getTimeStamp(const std::string& format)
{
    time_t now = time(0);
    struct tm *tstruct;
    char buf[80];
    tstruct = localtime(&now);
    strftime(buf, sizeof(buf), format.c_str(), tstruct);
    return std::string(buf);
}
