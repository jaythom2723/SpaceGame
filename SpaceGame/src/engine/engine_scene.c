#include "engine_scene.h"
#include "engine_components.h"
#include "engine_entity.h"
#include "engine_math.h"
#include "engine_logger.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

void _engine_resetcamera(EngineContext* ctx, float deltaTime)
{
	ctx->zoomfac = ENGINE_DEFAULT_ZOOM_FACTOR;
	ctx->camera->vx = 0;
	ctx->camera->vy = 0;

	EngineCamera* camera = ctx->camera;
	EngineScene* curscene = engineGetCurrentScene(ctx);
	if (curscene == NULL)
	{
		engineWriteMessage(ctx, "Cannot reset the camera if the scene does not exist!", ELOG_MSGTYPE_WARN);
		return;
	}

	if (curscene->entities == NULL)
	{
		engineWriteMessage(ctx, "Cannot reset the camera if the scene does not have any entities!", ELOG_MSGTYPE_WARN);
		return;
	}
	
	for (int i = 0; i < curscene->nentities; i++)
	{
		EngineEntity* ent = &(curscene->entities[i]);

		if (ctx->camera->x != 0 || ctx->camera->y != 0)
		{
			ent->x = ent->x - ctx->camera->x;
			ent->y = ent->y - ctx->camera->y;
		}
	}

	ctx->camera->x = 0;
	ctx->camera->y = 0;
}

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

	camera->x += camera->vx;
	camera->y += camera->vy;

	if (ctx->keys[EKEY_ESCAPE] == ETRUE)
		_engine_resetcamera(ctx, deltaTime);
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
	if (curscene == NULL)
		return;

	if (curscene->entities == NULL)
		return;

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
	{
		engineWriteMessage(ctx, "Cannot register more than one scene for a given state!", ELOG_MSGTYPE_WARN);
		return;
	}
	engineWriteMessage(ctx, "Registering a new scene!", ELOG_MSGTYPE_INFORM);


	// NOTE: wtf is this shit? Why did I do this?
	EngineScene* tmp = malloc(sizeof(EngineScene));
	ctx->scenes[state] = malloc(sizeof(EngineScene));
	if (ctx->scenes[state] == NULL)
	{
		engineWriteMessage(ctx, "Failed to allocate memory! (engine_scene.c>engineRegisterScene():136)", ELOG_MSGTYPE_ERROR);
		free(tmp);
		tmp = NULL;
		return;
	}

	if (tmp == NULL)
	{
		engineWriteMessage(ctx, "Failed to allocate memory! (engine_scene.c>engineRegisterScene():144)", ELOG_MSGTYPE_ERROR);
		free(ctx->scenes[state]);
		ctx->scenes[state] = NULL;
		return;
	}

	tmp->initialized = EFALSE;
	tmp->init = init;
	tmp->update = update;
	tmp->draw = draw;
	tmp->nentities = 0;

	memcpy(ctx->scenes[state], tmp, sizeof(EngineScene));

	ctx->scenes[state]->entities = calloc(ENGINE_MAX_ENTITIES, sizeof(EngineEntity));
	if (ctx->scenes[state]->entities == NULL)
	{
		engineWriteMessage(ctx, "Failed to allocate memory! (engine_scene.c>engineRegisterScene():161)", ELOG_MSGTYPE_ERROR);
		free(ctx->scenes[state]);
		ctx->scenes[state] = NULL;
		return;
	}

	free(tmp);
	tmp = NULL;

	engineWriteMessage(ctx, "Scene registered successfully!", ELOG_MSGTYPE_INFORM);
}

void engineDestroyScene(EngineContext* ctx, GameState state)
{
	if (ctx->scenes[state] == NULL)
	{
		engineWriteMessage(ctx, "Cannot destroy a NULL scene!", ELOG_MSGTYPE_WARN);
		return;
	}

	EngineScene* scene = engineGetScene(ctx, state);
	engineWriteMessage(ctx, "Destroying a scene...", ELOG_MSGTYPE_INFORM);

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
	int numNulls = 0;
	for (int i = 0; i < ENGINE_MAX_SCENES; i++)
	{
		if (numNulls >= 3)
			break;
		
		numNulls = ctx->scenes[i] == NULL ? numNulls + 1 : 0;
		if (ctx->scenes[i] != NULL)
			engineDestroyScene(ctx, i);
	}
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
