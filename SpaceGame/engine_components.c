#include "engine_components.h"

#include "engine_entity.h"

#include <string.h>
#include <assert.h>

extern void __engine__defaultRendererComponentFunc(EngineEntity*, EngineContext*, float);
extern void __engine__defaultTickComponentFunc(EngineEntity*, EngineContext*, float);
extern void __engine__defaultControllerComponentFunc(EngineEntity*, EngineContext*, float);

static EngineComponentFunc defaultfuncs[ENGINE_MAX_COMPONENTS] = {
	&__engine__defaultRendererComponentFunc,
	&__engine__defaultTickComponentFunc,
	& __engine__defaultControllerComponentFunc,
};

void engineAddComponent(EngineContext* ctx, EngineEntityKey key, EngineComponentType type)
{
	EngineEntity* ent = engineGetEntity(ctx, key);
	assert(ent != NULL);

	if (ent->components == NULL)
		engineInitEntityComponentList(ent);
	
	if (ent->components == NULL)
		return; // if ent->components continues to be NULL, just return

	EngineComponent comp = {
		.id = ((int)(ent->components + type) % 100),
		.type = type,
		.func = NULL
	};
	memcpy(ent->components + type, &comp, sizeof(EngineComponent));
}

void engineSetComponentCallback(EngineContext* ctx, EngineEntityKey key, EngineComponentType type, EngineComponentFunc func)
{
	EngineEntity* ent = engineGetEntity(ctx, key);
	assert(ent != NULL);

	if (ent->components == NULL)
		return;

	(ent->components + type)->func = func;
}

const EBOOL engineRemoveComponent(EngineContext* ctx, EngineEntityKey key, EngineComponentType type)
{
	EngineEntity* ent = engineGetEntity(ctx, key);
	assert(ent != NULL);

	if (ent->components == NULL)
		return EFALSE;

	*(ent->components + type) = (EngineComponent){ 0 };

	return ETRUE;
}

const EBOOL engineHasComponent(EngineContext* ctx, EngineEntityKey key, EngineComponentType type)
{
	EngineEntity* ent = engineGetEntity(ctx, key);
	assert(ent != NULL);

	if (ent->components == NULL)
		return EFALSE;

	return (ent->components + type)->id != 0;
}

void engineRunComponentFunc(EngineContext* ctx, EngineEntity* ent, EngineComponent* comp, float deltaTime)
{
	if (ent->components == NULL || comp == NULL)
		return;
	
	if (comp->func != NULL)
	{
		(*comp->func)(ent, ctx, deltaTime);
		return;
	}

	(*defaultfuncs[comp->type])(ent, ctx, deltaTime);
}
