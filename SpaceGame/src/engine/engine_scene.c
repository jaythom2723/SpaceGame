#include "engine_scene.h"
#include "engine_components.h"
#include "engine_entity.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

void _engine_docamerainput(EngineContext* ctx, float deltaTime)
{
	EngineCamera* camera = ctx->camera;

	if (ctx->keys[EKEY_W] == EFALSE || ctx->keys[EKEY_S] == EFALSE)
		camera->vy = 0.f;
	if (ctx->keys[EKEY_A] == EFALSE || ctx->keys[EKEY_D] == EFALSE)
		camera->vx = 0.f;

	if (ctx->keys[EKEY_W] == ETRUE)
		camera->vy = -(camera->speed) * deltaTime;
	else if (ctx->keys[EKEY_S] == ETRUE)
		camera->vy = camera->speed * deltaTime;

	if (ctx->keys[EKEY_A] == ETRUE)
		camera->vx = -(camera->speed) * deltaTime;
	else if (ctx->keys[EKEY_D] == ETRUE)
		camera->vx = camera->speed * deltaTime;
}

void _engine_dozoom(EngineContext* ctx, float deltaTime)
{
	if (ctx->zoomin == ETRUE)
		ctx->zoomfac += 1 * deltaTime;

	if (ctx->zoomout == ETRUE)
		ctx->zoomfac -= 1 * deltaTime;

	if (ctx->scroll_debounce_timer > 0.0f)
	{
		ctx->scroll_debounce_timer -= deltaTime;
		ctx->zoomin = EFALSE;
		ctx->zoomout = EFALSE;
	}
}

void _engine_update(EngineContext* ctx, float deltaTime)
{
	if (ctx->camera == NULL)
		return;

	if (ctx->camera->control == EFALSE)
		return;

	_engine_docamerainput(ctx, deltaTime);
	_engine_dozoom(ctx, deltaTime);

	// propogate the movement of the camera to all entities
	EngineScene* curscene = engineGetCurrentScene(ctx);
	assert(curscene != NULL);
	assert(curscene->entities != NULL);

	// emulating the camera movement
	for (int i = 0; i < curscene->nentities; i++)
	{
		EngineEntity* ent = &(curscene->entities[i]);
		ent->x += -(ctx->camera->vx);
		ent->y += -(ctx->camera->vy);
	}
}

void _engine_draw(EngineContext* ctx, float deltaTime)
{

}

void engineRegisterScene(EngineContext* ctx, GameState state, EngineSceneFunc init, EngineSceneFunc update, EngineSceneFunc draw)
{
	if (ctx->scenes[state] != NULL)
		return;
	
	// NOTE: wtf is this shit? Why did I do this?
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
	// TODO: make this honky ah function more readable
	if (ctx->scenes[state] == NULL)
		return;

	EngineScene* scene = engineGetScene(ctx, state);

	// loop through entity component lists and make sure they are not null
	for (int i = 0; i < scene->nentities; i++)
	{
		if (scene->entities[i].components == NULL)
			continue;

		free(scene->entities[i].components);
		scene->entities[i].components = NULL;
	}

	free(scene->entities);
	scene->entities = NULL;

	scene = NULL;

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
	_engine_update(ctx, deltaTime);

	if (curscene->draw != NULL)
		(*curscene->draw)(curscene, ctx, deltaTime);
	_engine_draw(ctx, deltaTime);
}
