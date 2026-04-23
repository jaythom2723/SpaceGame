#include "engine_entity.h"

#include "engine_components.h"
#include "engine_scene.h"

#include <stdlib.h>
#include <string.h>

const EBOOL engineCreateEntity(EngineContext* ctx, vec2 pos, vec2 size, const EngineEntityKey key, const EngineTextureKey texkey)
{	
	if (ctx->state == -1 || ctx->scenes[ctx->state] == NULL)
		return EFALSE;

	return engineCreateEntityScene(ctx, pos, size, key, texkey, ctx->state);
}

const EBOOL engineCreateEntityScene(EngineContext* ctx, vec2 pos, vec2 size, const EngineEntityKey key, const EngineTextureKey texkey, GameState state)
{
	EngineEntity tmp = {
		.x = pos[0],
		.y = pos[1],
		.r = 1.0f,
		.g = 1.0f,
		.b = 1.0f,
		.width = size[0],
		.height = size[1],
		.texkey = texkey,
		.key = key,
		.components = NULL
	};

	if (ctx->scenes[state] == NULL)
		return EFALSE;

	EngineScene* scene = engineGetScene(ctx, state);
	assert(scene != NULL);
	memcpy(scene->entities + scene->nentities, &tmp, sizeof(EngineEntity));
	scene->nentities++;

	return ETRUE;
}

void engineDestroyAllEntities(EngineContext* ctx)
{
	if (ctx->state == -1)
		return;

	engineDestroyAllEntitiesScene(ctx, ctx->state);
}

void engineDestroyAllEntitiesScene(EngineContext* ctx, GameState state)
{
	if (ctx->scenes[ctx->state] == NULL)
		return;

	EngineScene* scene = engineGetScene(ctx, state);
	assert(scene != NULL);

	for (unsigned i = 0; i < scene->nentities; i++)
	{
		EngineEntity* cur = &(scene->entities[i]);

		if (cur->components != NULL)
		{
			free(cur->components);
			cur->components = NULL;
		}

		(*cur) = (EngineEntity){ 0 };
		cur = NULL;
	}
	scene->nentities = 0;
}

void engineDestroyEntity(EngineContext* ctx, EngineEntityKey key)
{
	if (ctx->state == -1)
		return;

	engineDestroyEntityScene(ctx, ctx->state, key);
}

void engineDestroyEntityScene(EngineContext* ctx, GameState state, EngineEntityKey key)
{
	if (ctx->scenes[ctx->state] == NULL)
		return;

	EngineEntity* ent = engineGetEntityScene(ctx, state, key);

	if (ent->components != NULL)
	{
		free(ent->components);
		ent->components = NULL;
	}

	ctx->scenes[state]->nentities--;
	(*ent) = (EngineEntity){ 0 };
	ent = NULL;
}

EngineEntity* engineGetEntity(EngineContext* ctx, EngineEntityKey key)
{
	if (ctx->state == -1)
		return NULL;

	EngineEntity* ent = engineGetEntityScene(ctx, ctx->state, key);
	return ent;
}

EngineEntity* engineGetEntityScene(EngineContext* ctx, GameState state, EngineEntityKey key)
{
	if (ctx->scenes[ctx->state] == NULL)
		return NULL;

	EngineScene* scene = engineGetScene(ctx, state);
	assert(scene != NULL);

	for (unsigned i = 0; i < scene->nentities; i++)
	{
		if (scene->entities[i].key != key)
			continue;
		return &scene->entities[i];
	}

	return NULL;
}

void engineInitEntityComponentList(EngineEntity* ent)
{
	ent->components = malloc(ENGINE_MAX_COMPONENTS * sizeof(EngineComponent));
	if (ent->components == NULL)
		exit(-10);

	for (int i = 0; i < ENGINE_MAX_COMPONENTS; i++)
	{
		ent->components[i] = (EngineComponent){
							.id = 0,
							.func = NULL,
							.type = i
						};
	}
}

void engineProcessEntities(EngineContext* ctx, float deltaTime)
{
	EngineScene* curscene = engineGetCurrentScene(ctx);
	assert(curscene != NULL);

	for (unsigned i = 0; i < curscene->nentities; i++)
	{
		EngineEntity* cur = &curscene->entities[i];
		assert(cur != NULL);

		if (cur->components == NULL)
			continue;

		for (int j = 0; j < ENGINE_MAX_COMPONENTS; j++)
		{
			if (cur->components[j].id == 0)
				continue;

			engineRunComponentFunc(ctx, cur, (cur->components + j), deltaTime);
		}
	}
}

void engineEntitySetDrawColor(EngineContext* ctx, EngineEntityKey key, float r, float g, float b)
{
	EngineEntity* ent = engineGetEntity(ctx, key);
	assert(ent != NULL);

	ent->r = r;
	ent->g = g;
	ent->b = b;
}
