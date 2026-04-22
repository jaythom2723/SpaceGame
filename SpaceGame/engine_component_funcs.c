#include "engine_defs.h"
#include "engine_components.h"
#include "engine_renderer.h"

#include <stdio.h>

void __engine__defaultRendererComponentFunc(EngineEntity* ent, EngineContext* ctx, float deltaTime)
{
	engineSetDrawColor(ctx, ent->r, ent->g, ent->b);
	engineRenderEntity(ctx, ent->key);
}

void __engine__defaultControllerComponentFunc(EngineEntity* ent, EngineContext* ctx, float deltaTime)
{
	/*
		NOTE:
			There really isn't a "default" controller function,
			as most controls are/should be defined by the engine's user.
		TODO:
			prototype a control loading scheme for a csv or cfg file.
	*/
	for (int i = 0; i < ENGINE_NUM_KEYS; i++)
	{
		if (ctx->keys[i] == ETRUE)
			printf("Pressed a key: %d!\n", i);
	}
}

void __engine__defaultTickComponentFunc(EngineEntity* ent, EngineContext* ctx, float deltaTime)
{
	/*
		NOTE: 
			There really isn't a "default" tick compononent function
			either, as all of this logic should be handled by
			the game programmer
	*/
}