#ifndef ENGINE_COMPONENTS_H
#define ENGINE_COMPONENTS_H

#include "engine_defs.h"

void engineAddComponent(EngineContext* ctx, EngineEntityKey key, EngineComponentType type);
void engineSetComponentCallback(EngineContext* ctx, EngineEntityKey key, EngineComponentType type, EngineComponentFunc func);
const EBOOL engineRemoveComponent(EngineContext* ctx, EngineEntityKey key, EngineComponentType type);
const EBOOL engineHasComponent(EngineContext* ctx, EngineEntityKey key, EngineComponentType type);

void engineRunComponentFunc(EngineContext* ctx, EngineEntity* key, EngineComponent* comp, float deltaTime);

//void engineRunComponentFunc(EngineContext* ctx, EngineEntityKey key, EngineComponentType type, float deltaTime);

#endif // ENGINE_COMPONENTS_H