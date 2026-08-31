#ifndef SCENE_FUNCS_H
#define SCENE_FUNCS_H

#include "engine_defs.h"

void game_generate_galaxy_map_init(EngineScene* scene, EngineContext* ctx, float deltaTime);
void game_generate_solar_system_init(EngineScene* scene, EngineContext* ctx, float deltaTime);
void game_generate_planet_map_init(EngineScene* scene, EngineContext* ctx, float deltaTime);

void game_galaxy_map_init(EngineScene* scene, EngineContext* ctx, float deltaTime);
void game_galaxy_map_update(EngineScene* scene, EngineContext* ctx, float deltaTime);
void game_galaxy_map_draw(EngineScene* scene, EngineContext* ctx, float deltaTime);

#endif // SCENE_FUNCS_H
