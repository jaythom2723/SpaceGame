#ifndef ENGINE_ENTITY_H
#define ENGINE_ENTITY_H

#include "engine_defs.h"

#include <cglm/cglm.h>

const EBOOL engineCreateEntity(EngineContext* ctx, vec2 pos, vec2 size, const EngineEntityKey key, const EngineTextureKey texkey);

void engineDestroyAllEntities(EngineContext* ctx);
void engineDestroyEntity(EngineContext* ctx, EngineEntityKey key);

EngineEntity* engineGetEntity(EngineContext* ctx, EngineEntityKey key);

void engineInitEntityComponentList(EngineEntity* ent);

void engineProcessEntities(EngineContext* ctx, float deltaTime);

#endif // ENGINE_ENTITY_H