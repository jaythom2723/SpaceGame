#ifndef OB_TIME_H
#define OB_TIME_H

#include <stdint.h>

void OBTIMEgetDayMonthYear(uint32_t* year, uint32_t* month, uint32_t* day);
void OBTIMEgetTimestamp(uint32_t* hours, uint32_t* minutes, uint32_t* seconds);

#endif