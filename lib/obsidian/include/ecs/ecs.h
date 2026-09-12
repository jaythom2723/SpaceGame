#ifndef ECS_H
#define ECS_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

enum obsidian_component_type {
    OB_TEXTURE_COMPONENT,
    OB_MODEL_COMPONENT,
    OB_COLOR_COMPONENT,
    OB_POSITION_COMPONENT,
    OB_SCALE_COMPONENT,
    OB_ROTATION_COMPONENT,
};

typedef uint32_t ob_entity_t;
typedef uint32_t ob_compid_t;
typedef uint32_t ob_comptype_t;

struct obsidian_component {
    size_t size;
    ob_compid_t uid;
    ob_comptype_t type;
    void* data;
};

ob_entity_t OBECScreateEntity(void);
void OBECSdestroyEntity(ob_entity_t* ent);

bool OBECSaddComponent(const ob_entity_t ent, const ob_comptype_t type, void* data, size_t dsize);
bool OBECSremoveComponent(const ob_entity_t ent, const ob_comptype_t type);
bool OBECShasComponent(const ob_entity_t ent, const ob_comptype_t type);

const void* OBECSgetComponentData(const ob_entity_t ent, const ob_comptype_t type);
bool OBECSsetComponentData(const ob_entity_t ent, const ob_comptype_t type, const void* data, const size_t dsize);

#endif
