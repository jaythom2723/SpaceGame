#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

uint32_t __ob_math_ndgts(uint32_t n) {
  if (n == 0)
    return 1;
  return floor(log10(abs((int)n))) + 1;
}