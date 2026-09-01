#include "utility/ob_time.h"

#include <time.h>

void OBTIMEgetDayMonthYear(uint32_t* year, uint32_t* month, uint32_t* day)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    (*year) = tm.tm_year + 1900;
    (*month) = tm.tm_mon + 1;
    (*day) = tm.tm_mday;
}

void OBTIMEgetTimestamp(uint32_t* hours, uint32_t* minutes, uint32_t* seconds)
{
    time_t t = time(NULL);
    struct tm tm = *gmtime(&t);

    (*hours) = tm.tm_hour % 24;
    (*minutes) = tm.tm_min;
    (*seconds) = tm.tm_sec;
}