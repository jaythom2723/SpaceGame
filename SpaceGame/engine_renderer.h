#ifndef ENGINE_RENDERER_H
#define ENGINE_RENDERER_H

#include "engine_defs.h"

// TODO: refactor so that this takes in an EngineEntity* instead of a key
/**
 * \brief render a single entity to the window.
 * 
 * \param ctx The context
 * \param key A valid entity key with the renderer component
 */
void engineRenderEntity(EngineContext* ctx, EngineEntityKey key);

#endif // ENGINE_RENDERER_H
