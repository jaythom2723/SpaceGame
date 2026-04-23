#include "engine_scene.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

void engineRegisterScene(EngineContext* ctx, GameState state, EngineSceneFunc init, EngineSceneFunc update, EngineSceneFunc draw)
{
	if (ctx->scenes[state] != NULL)
		return;

	EngineScene* tmp = malloc(sizeof(EngineScene));
	ctx->scenes[state] = malloc(sizeof(EngineScene));
	if (ctx->scenes[state] == NULL || tmp == NULL)
		return;

	tmp->initialized = EFALSE;
	tmp->init = init;
	tmp->update = update;
	tmp->draw = draw;
	tmp->nentities = 0;

	memcpy(ctx->scenes[state], tmp, sizeof(EngineScene));

	ctx->scenes[state]->entities = calloc(ENGINE_MAX_ENTITIES, sizeof(EngineEntity));
	assert(ctx->scenes[state]->entities != NULL);

	free(tmp);
	tmp = NULL;
}

void engineDestroyScene(EngineContext* ctx, GameState state)
{
	if (ctx->scenes[state] == NULL)
		return;

	free(ctx->scenes[state]->entities);
	ctx->scenes[state]->entities = NULL;

	free(ctx->scenes[state]);
	ctx->scenes[state] = NULL;
}

void engineDestroyAllScenes(EngineContext* ctx)
{
	for (int i = 0; i < ENGINE_MAX_SCENES; i++)
		engineDestroyScene(ctx, i);
}

EngineScene* engineGetScene(EngineContext* ctx, GameState state)
{
	return ctx->scenes[state];
}

EngineScene* engineGetCurrentScene(EngineContext* ctx)
{
	if (ctx->state == -1 || ctx->scenes[ctx->state] == NULL)
		return NULL;

	return engineGetScene(ctx, ctx->state);
}

void engineRunScene(EngineContext* ctx, float deltaTime)
{
	EngineScene* curscene = engineGetCurrentScene(ctx);
	if (curscene == NULL)
		return;

	if (curscene->initialized == EFALSE)
	{
		(*curscene->init)(curscene, ctx, deltaTime);
		curscene->initialized = ETRUE;
	}

	if (curscene->update != NULL)
		(*curscene->update)(curscene, ctx, deltaTime);

	if (curscene->draw != NULL)
		(*curscene->draw)(curscene, ctx, deltaTime);
}
