#ifndef ENGINE_RENDERER_H
#define ENGINE_RENDERER_H

#include "engine_defs.h"

#include <cglm/cglm.h>

// TODO: refactor so that this takes in an EngineEntity* instead of a key
/**
 * \brief render a single entity to the window.
 * 
 * \param ctx The context
 * \param key A valid entity key with the renderer component
 */
void engineRenderEntity(EngineContext* ctx, EngineEntityKey key);

/**
 * \brief render a specific texture to the screen.
 *
 * \param ctx A valid context
 * \param key The texture key
 * \param pos The position of the texture
 * \param size The size of the texture in pixels;
 */
void engineRenderTexture(EngineContext* ctx, EngineTextureKey key, vec2 pos, vec2 size);


/**
 * \brief set the spriteColor uniform in the shader program (if available).
 * 
 * \param ctx A valid context
 * \param r Red normalized
 * \param g Green normalized
 * \param b Blue normalized
 */
void engineSetDrawColor(EngineContext* ctx, float r, float g, float b);

#endif // ENGINE_RENDERER_H



