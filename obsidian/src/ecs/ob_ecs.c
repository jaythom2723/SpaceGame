#include "ecs/ob_ecs.h"

#include "utility/ob_logger.h"
#include "utility/ob_error.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define __OB_MAX_ENTITIES 0xFFFF
#define __OB_MAX_POOLS __OB_MAX_ENTITIES
#ifndef OB_MAX_COMPONENTS
#define OB_MAX_COMPONENTS 0xFF
#endif

struct __ob_componentpool {
    size_t count;
    size_t cap;
    struct obsidian_component* components;
    struct obsidian_component* cptr;
    ob_entity_t parent;
};

extern bool __ob_error_pusherror(enum obsidian_error_code, enum obsidian_error_severity, enum obsidian_error_category, const char*, const char*, const uint32_t);
extern bool __ob_error_readerror(void);

extern void __ob_log_wline(enum ob_logger_message_type, const char* const);
extern void __ob_log_wsline(const char* const);

extern uint32_t __ob_asset_genUniqueIdentity(void);

extern uint32_t __ob_util_genUniqueIdentity(const void* aptr, const void* bptr);

bool __ob_ecs_initmodule(void);
bool __ob_ecs_closemodule(void);

static struct __ob_componentpool __ob_ecs_getcomponentpool(const ob_entity_t, uint32_t*);
static struct obsidian_component __ob_ecs_getcomponent(const struct __ob_componentpool, const enum obsidian_component_type, uint32_t*);
struct obsidian_component __ob_ecs_getentcomp(const ob_entity_t, const enum obsidian_component_type, uint32_t*);

ob_entity_t* __ob_ents = NULL;
static ob_entity_t* __ob_entptr = NULL;
static struct __ob_componentpool* __ob_cpools = NULL;
static struct __ob_componentpool* __ob_cpoolptr = NULL;

bool __ob_ecs_initmodule(void)
{
    if (__ob_ents != NULL || __ob_cpools != NULL)
        return false;

    __ob_ents = calloc(__OB_MAX_ENTITIES, sizeof(ob_entity_t));
    __ob_cpools = calloc(__OB_MAX_POOLS, sizeof(struct __ob_componentpool));

    if (__ob_ents == NULL || __ob_cpools == NULL)
    {
        (void)__ob_error_pusherror(ERR_OUT_OF_MEMORY, SEV_WARNING, CAT_MEMORY, "Failed to allocate enough memory to initialize Obsidian ECS: Core Module", __FILE__, __LINE__);
        (void)__ob_error_readerror();
        return false;
    }

    __ob_entptr = __ob_ents;
    __ob_cpoolptr = __ob_cpools;

    return true;
}

bool __ob_ecs_closemodule(void)
{
    if (__ob_ents == NULL || __ob_cpools == NULL)
        return false;
    for (int i = 0; i < __OB_MAX_POOLS; i++)
    {
        if (__ob_cpools[i].components != NULL)
        {
            free((__ob_cpools + i)->components);
            __ob_cpools[i].components = NULL;
        }
    }

    __ob_cpoolptr = NULL;
    __ob_entptr = NULL;

    free(__ob_cpools);
    __ob_cpools = NULL;

    free(__ob_ents);
    __ob_ents = NULL;

    return true;
}

static struct __ob_componentpool __ob_ecs_getcomponentpool(const ob_entity_t ent, uint32_t* index)
{
    // TODO: sorting algorithm
    if (__ob_ents == NULL || __ob_cpools == NULL)
        return (struct __ob_componentpool) {0};

    for (int i = 0; i < __OB_MAX_POOLS; i++)
        if (__ob_cpools[i].parent == ent)
        {
            (*index) = i;
            return __ob_cpools[i];
        }

    return (struct __ob_componentpool) {0};
}

static struct obsidian_component __ob_ecs_getcomponent(const struct __ob_componentpool pool, const enum obsidian_component_type type, uint32_t* index)
{
    // TODO: sorting algorithm
    if (__ob_ents == NULL || __ob_cpools == NULL || pool.components == NULL)
        return (struct obsidian_component) {0};

    for (int i = 0; i < OB_MAX_COMPONENTS; i++)
        if (pool.components[i].type == type)
        {
            (*index) = i;
            return pool.components[i];
        }

    return (struct obsidian_component) {0};
}

struct obsidian_component __ob_ecs_getentcomp(const ob_entity_t ent, const enum obsidian_component_type type, uint32_t* index)
{
    uint32_t pooli = 0;
    struct __ob_componentpool pool = __ob_ecs_getcomponentpool(ent, &pooli);
    struct obsidian_component comp = __ob_ecs_getcomponent(pool, type, index);
    return comp;
}

ob_entity_t OBECScreateEntity(void)
{
    if (__ob_ents == NULL || __ob_cpools == NULL)
        return 0;

    if (__ob_entptr - __ob_ents >= __OB_MAX_ENTITIES)
        return 0;

    ob_entity_t ent = __ob_util_genUniqueIdentity(__ob_ents, __ob_entptr);
    struct __ob_componentpool pool = { 0 };

    (*__ob_entptr) = ent;

    pool.cap = OB_MAX_COMPONENTS;
    pool.count = 0;
    pool.parent = ent;

    (*__ob_cpoolptr) = pool;
    (*__ob_cpoolptr).components = calloc(OB_MAX_COMPONENTS, sizeof(struct obsidian_component));
    if ((*__ob_cpoolptr).components == NULL)
    {
        (void)__ob_error_pusherror(ERR_OUT_OF_MEMORY, SEV_WARNING, CAT_MEMORY, "Failed to allocate memory for an entity's component pool.", __FILE__, __LINE__);
        (void)__ob_error_readerror();
        return 0;
    }
    (*__ob_cpoolptr).cptr = (*__ob_cpoolptr).components;

    __ob_entptr++;
    __ob_cpoolptr++;

    return ent;
}

void OBECSdestroyEntity(ob_entity_t* ent)
{
    // destroy the component pool owned by the entity
    uint32_t index = 0;
    struct __ob_componentpool pool = __ob_ecs_getcomponentpool(*ent, &index);

    if (pool.components != NULL)
    {
        for (int i = 0; i < OB_MAX_COMPONENTS; i++)
        {
            if (pool.components[i].data == NULL)
                continue;
            free(pool.components[i].data);
            pool.components[i].data = NULL;
        }

        free(pool.components);
        pool.components = NULL;
    }

    memcpy(__ob_cpools + index, &pool, sizeof(struct __ob_componentpool));

    // TODO: sorting algorithm needed for sure.
    for (int i = 0; i < __OB_MAX_ENTITIES; i++)
    {
        if (__ob_ents[i] == *ent)
        {
            __ob_ents[i] = 0;
            break;
        }
    }

    (*ent) = 0;
}

bool OBECSaddComponent(const ob_entity_t ent, const ob_comptype_t type, void* data, size_t dsize)
{
    if (__ob_ents == NULL || __ob_cpools == NULL)
        return false;

    // FIXME: data loss issue revolving around void* data usage.
    struct obsidian_component comp = { 0 };
    comp.data = malloc(dsize);
    if (comp.data == NULL) return false;
    memcpy(comp.data, data, dsize);
    comp.size = dsize;
    comp.uid = __ob_util_genUniqueIdentity(__ob_cpools, __ob_cpoolptr);
    comp.type = type;

    uint32_t index = 0;
    struct __ob_componentpool pool = __ob_ecs_getcomponentpool(ent, &index);

    if (pool.components == NULL || pool.count >= pool.cap)
        return false;

    if (pool.cptr != (pool.components + pool.count) && pool.cptr->data != NULL)
        pool.cptr = pool.components + pool.count;
    
    memcpy(pool.cptr, &comp, sizeof(struct obsidian_component));

    (pool.cptr)++;
    (pool.count)++;

    memcpy(__ob_cpools + index, &pool, sizeof(struct __ob_componentpool));

    return true;
}

bool OBECSremoveComponent(const ob_entity_t ent, const ob_comptype_t type)
{
    if (__ob_ents == NULL || __ob_cpools == NULL)
        return false;

    uint32_t pooli = 0, compi = 0;
    struct __ob_componentpool pool = __ob_ecs_getcomponentpool(ent, &pooli);
    struct obsidian_component comp = __ob_ecs_getcomponent(pool, type, &compi);

    if (comp.data == NULL)
        return false;

    free(comp.data);
    comp.data = NULL;
    memset(pool.components + compi, 0, sizeof(struct obsidian_component));

    pool.count--;
    pool.cptr = pool.components + compi;
    memcpy(__ob_cpools + pooli, &pool, sizeof(struct __ob_componentpool));
    return true;
}

bool OBECShasComponent(const ob_entity_t ent, const ob_comptype_t type)
{
    if (__ob_ents == NULL || __ob_cpools == NULL)
        return false;

    uint32_t pooli = 0, compi = 0;
    struct __ob_componentpool pool = __ob_ecs_getcomponentpool(ent, &pooli);
    struct obsidian_component comp = __ob_ecs_getcomponent(pool, type, &compi);
    return comp.data != NULL;
}

const void* OBECSgetComponentData(const ob_entity_t ent, const ob_comptype_t type)
{
    if (__ob_ents == NULL || __ob_cpools == NULL)
        return NULL;

    uint32_t pooli = 0, compi = 0;
    struct __ob_componentpool pool = __ob_ecs_getcomponentpool(ent, &pooli);
    struct obsidian_component comp = __ob_ecs_getcomponent(pool, type, &compi);
    return comp.data;
}

bool OBECSsetComponentData(const ob_entity_t ent, const ob_comptype_t type, const void* data, const size_t dsize)
{
    if (__ob_ents == NULL || __ob_cpools == NULL)
        return NULL;

    uint32_t pooli = 0, compi = 0;
    struct __ob_componentpool pool = __ob_ecs_getcomponentpool(ent, &pooli);
    struct obsidian_component comp = __ob_ecs_getcomponent(pool, type, &compi);
    if (comp.data == NULL)
        return false;
    memcpy(((__ob_cpools + pooli)->components + compi)->data, data, dsize);
    return true;
}
