#ifndef ENGINE_SCENE_H
#define ENGINE_SCENE_H

#include "engine_defs.h"

void engineRegisterScene(EngineContext* ctx, GameState state, 
	EngineSceneFunc init, EngineSceneFunc update, EngineSceneFunc draw);

void engineDestroyScene(EngineContext* ctx, GameState state);
void engineDestroyAllScenes(EngineContext* ctx);

EngineScene* engineGetScene(EngineContext* ctx, GameState state);
EngineScene* engineGetCurrentScene(EngineContext* ctx);

void engineRunScene(EngineContext* ctx, float deltaTime);

#endif // ENGINE_SCENE_H
