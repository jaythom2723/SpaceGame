#ifndef ENGINE_TEXTURES_H
#define ENGINE_TEXTURES_H

#include "engine_defs.h"

/**
 * \brief Create a texture.
 * 
 * \param ctx The context
 * \param key The key of the newly created texture
 * 
 * \return Returns EFALSE if the operation was unsuccessful
 */
const EBOOL engineCreateTexture(EngineContext* ctx, unsigned int key);

/**
 * \brief Generate a texture from a file.
 * 
 * \param ctx The context
 * \param key The texture's key
 * \param path The path to the texture's file
 * \param alpha Should the texture have transparency?
 * 
 * \return Returns EFALSE if the operation was unsuccessful
 */
const EBOOL engineGenerateTexture(EngineContext* ctx, unsigned int key, const char* path, const EBOOL alpha);

/**
 * \brief Generate a texture and replace if missing.
 * 
 * Generate a texture, however, replace the texture with a missing texture if the original texture failed to generate
 * 
 * \param ctx The context
 * \param key The texture's key
 * \param path The file path
 * \param alpha Should the texture have transparency?
 */
void engineGenTextureCheckMissing(EngineContext* ctx, unsigned int key, const char* path, const EBOOL alpha);

/**
 * \brief Wrapper for glBindTexture().
 * 
 * \param ctx The context
 * \param id The texture to bind's ID
 */
void engineBindTexture(EngineContext* ctx, unsigned int id);

/**
 * \brief Destroy all textures created at runtime.
 * 
 * \param ctx The context
 */
void engineDestroyAllTextures(EngineContext* ctx);

/**
 * \brief Destroy a single texture created at runtime.
 * 
 * \param ctx The context
 * \param key The texture to destroy's key
*/
void engineDestroyTexture(EngineContext* ctx, unsigned int key);

#endif // ENGINE_TEXTURES_H


