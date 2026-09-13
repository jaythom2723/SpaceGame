#include "gen/gen_galaxy.h"

#include <ext/ob_perlin.h>

#include <stdio.h>

// TODO: come up with a better name for this
void gen_galaxy_one(void)
{
    OBEXTperlinSetSize(8000, 8000);
    float* noise = NULL;
    OBEXTperlinInvoke((void*)&noise, 8000 * 8000 * sizeof(float));

    for (int y = 0; y < 32; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            uint32_t index = y * 8000 + x;
            float nVal = noise[index];

            printf("%.02f ", nVal);
        }
        printf("\n");
    }

    free(noise);
    noise = NULL;
}