#include "obsidian.h"

struct obsidian_context* OBcreateContext(void)
{
    struct obsidian_context* ctx = (struct obsidian_context*) calloc(1, sizeof(struct obsidian_context));
    if (ctx == NULL) {
        // TODO: woohoo, logger and error handler being developed first will help a lot with debugging!
        return NULL;
    }

    return ctx;
}

void OBdestroyContext(struct obsidian_context* ctx)
{
    free((void*) ctx);
    ctx = NULL;
}