#include <stdint.h>
#include <math.h>
#include <stdlib.h>

uint32_t __ob_math_ndigits(uint32_t n)
{
    if (n == 0) return 1;
    return floor(log10(abs((int)n))) + 1;
}