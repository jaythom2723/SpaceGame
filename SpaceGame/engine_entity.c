#include "engine_entity.h"

#include "engine_components.h"

#include <stdlib.h>
#include <string.h>

const EBOOL engineCreateEntity(EngineContext* ctx, vec2 pos, vec2 size, const EngineEntityKey key, const EngineTextureKey texkey)
{
	EngineEntity tmp = {
		.x = pos[0],
		.y = pos[1],
		.width = size[0],
		.height = size[1],
		.texkey = texkey,
		.key = key,
		.components = NULL
	};

	memcpy(ctx->entities + ctx->nentities, &tmp, sizeof(EngineEntity));

	ctx->nentities++;

	return ETRUE;
}

void engineDestroyAllEntities(EngineContext* ctx)
{
	for (unsigned i = 0; i < ctx->nentities; i++)
	{
		EngineEntity* cur = &ctx->entities[i];

		if (cur->components != NULL)
		{
			free(cur->components);
			cur->components = NULL;
		}

		(*cur) = (EngineEntity){ 0 };
		cur = NULL;
	}
	ctx->nentities = 0;
}

void engineDestroyEntity(EngineContext* ctx, EngineEntityKey key)
{
	EngineEntity* ent = engineGetEntity(ctx, key);

	if (ent->components != NULL)
	{
		free(ent->components);
		ent->components = NULL;
	}

	(*ent) = (EngineEntity){ 0 };
	ent = NULL;
}

EngineEntity* engineGetEntity(EngineContext* ctx, EngineEntityKey key)
{
	for (unsigned i = 0; i < ctx->nentities; i++)
	{
		if (ctx->entities[i].key != key)
			continue;

		return &ctx->entities[i];
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
	for (unsigned i = 0; i < ctx->nentities; i++)
	{
		EngineEntity* cur = &ctx->entities[i];
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
