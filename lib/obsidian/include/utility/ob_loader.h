#ifndef OB_LOADER_H
#define OB_LOADER_H

#include <stdint.h>
#include <stdbool.h>

struct obsidian_asset {
    char* data;
    uint32_t datasize;
    uint32_t uid;

    // TODO: Figure out what to do about model assets?
};



#endif