#include "assets/ob_asset.h"

#include "utility/ob_error.h"
#include "utility/ob_logger.h"
#include "utility/ob_time.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#include <glad/glad.h>

// TODO: Find out a way to relate two assets with each other (linked list ts)

// bool __ob_astman_initmodule(void);
// bool __ob_astman_closemodule(void);

extern bool __ob_error_pusherror(enum obsidian_error_code, enum obsidian_error_severity, enum obsidian_error_category, const char*, const char*, const uint32_t);
extern bool __ob_error_readerror(void);

extern void __ob_log_wline(enum ob_logger_message_type, const char* const);
extern void __ob_log_wsline(const char* const);

extern uint32_t __ob_buf_createvbo(void);
extern uint32_t __ob_buf_createebo(void);
extern uint32_t __ob_buf_createvao(void);
extern uint32_t __ob_tex_createtex(void);

extern bool __ob_buf_bindvbo(uint32_t);
extern bool __ob_buf_setvbodata(uint32_t, size_t, const void*, uint32_t);
extern bool __ob_buf_bindebo(uint32_t);
extern bool __ob_buf_setebodata(uint32_t, size_t, const void*, uint32_t);
extern bool __ob_buf_bindvao(uint32_t);
extern bool __ob_tex_gendata(uint32_t, const struct obsidian_asset*);

extern void __ob_buf_unbindvao(void);
extern void __ob_buf_unbindvbo(void);
extern void __ob_buf_unbindebo(void);
extern void __ob_tex_unbindtex(void);
extern void __ob_buf_deletevao(uint32_t);
extern void __ob_buf_deletevbo(uint32_t);
extern void __ob_buf_deleteebo(uint32_t);
extern void __ob_tex_deletetex(uint32_t);
extern void __ob_buf_setattribpointer(uint32_t, uint32_t, size_t, void*);

static inline struct obsidian_asset* __ob_create_new_asset(void);
static inline bool __ob_is_asset_valid(const struct obsidian_asset* const restrict);

uint32_t __ob_asset_genUniqueIdentity(void);

bool OBASTcreatePrimitiveModel(struct obsidian_asset** asset, const float* vertices, const size_t vsize, const unsigned int* indices, const size_t isize)
{
    (*asset) = __ob_create_new_asset();
    if (!__ob_is_asset_valid(*asset)) return false;

    (*asset)->type = OB_ASSET_MODEL;
    (*asset)->uid = __ob_asset_genUniqueIdentity(); // TODO: prototype
    (*asset)->rdata = NULL;

    (*asset)->mdlprim.vaoi = __ob_buf_createvao();
    (void)__ob_buf_bindvao((*asset)->mdlprim.vaoi);
    (*asset)->mdlprim.vboi = __ob_buf_createvbo();
    (void)__ob_buf_bindvbo((*asset)->mdlprim.vboi);
    (void)__ob_buf_setvbodata((*asset)->mdlprim.vboi, vsize, vertices, GL_STATIC_DRAW);
    (*asset)->mdlprim.eboi = __ob_buf_createebo();
    (void)__ob_buf_bindebo((*asset)->mdlprim.eboi);
    (void)__ob_buf_setebodata((*asset)->mdlprim.eboi, isize, indices, GL_STATIC_DRAW);

    __ob_buf_setattribpointer(0, 3, 5 * sizeof(float), (void*)0);
    __ob_buf_setattribpointer(1, 2, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    return true;
}

void OBASTdestroyAsset(struct obsidian_asset* restrict asset)
{
    if (!__ob_is_asset_valid(asset)) return;

    switch (asset->type)
    {
        case OB_ASSET_TEXTURE:
            __ob_tex_deletetex(asset->texture.texi);
            free(asset->texture.pixels);
            asset->texture.pixels = NULL;
        break;
        case OB_ASSET_MODEL:
            __ob_buf_unbindvao();
            __ob_buf_unbindvbo();
            __ob_buf_unbindebo();
            __ob_tex_unbindtex();
            __ob_buf_deletevao(asset->mdlprim.vaoi);
            __ob_buf_deletevbo(asset->mdlprim.vboi);
            __ob_buf_deleteebo(asset->mdlprim.eboi);
        break;
        default:
        break;
    }

    free(asset->rdata);
    asset->rdata = NULL;

    free(asset);
    asset = NULL;
}

static inline struct obsidian_asset* __ob_create_new_asset(void)
{
    struct obsidian_asset* ret = calloc(1, sizeof(struct obsidian_asset));
    if (ret == NULL)
        return NULL;
    return ret;
}

static inline bool __ob_is_asset_valid(const struct obsidian_asset* const restrict asset) {
    if (asset == NULL)
    {
        (void)__ob_error_pusherror(ERR_NONE, SEV_WARNING, CAT_CORE_SYS, "An asset is invalid", __FILE__, __LINE__);
        (void)__ob_error_readerror();
        return false;
    }
    return true;
}

uint32_t __ob_asset_genUniqueIdentity(void)
{
    // TODO: COMPARTMENTALIZE
    // get seed components
    uint32_t year,month,day,hours,minutes,seconds;
    OBTIMEgetDayMonthYear(&year, &month, &day);
    OBTIMEgetTimestamp(&hours, &minutes, &seconds);

    // generate the seed
    uint32_t seed = year << rand();
    seed |= (month % 2) >> rand();
    seed |= day;
    seed |= (hours ^ ~(day)) * rand();
    seed |= (minutes & seconds) << rand();
    seed |= (((~(seconds) ^ day) | hours) & ~(year)) << rand();

    // END COMPARTMENTALIZE

    srand(seed);
    return rand();
}