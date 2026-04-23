#ifndef ENGINE_ENTITY_H
#define ENGINE_ENTITY_H

#include "engine_defs.h"

#include <cglm/cglm.h>

/**
 * \brief Create an entity.
 * 
 * \param ctx A valid context
 * \param pos The position of the entity
 * \param size The size of the entity
 * \param key The key to assign the entity
 * \param texkey The texture assigned to the entity
 * 
 * \return Returns EFALSE if unsuccessful, ETRUE if successful
 */
const EBOOL engineCreateEntity(EngineContext* ctx, vec2 pos, vec2 size, const EngineEntityKey key, const EngineTextureKey texkey);

/**
 * \brief Create an entity for a specific scene.
 * 
 * \param ctx
 * \param pos
 * \param size
 * \param key
 * \param texkey
 * \param state
 * 
 * \return Returns EFALSE if unsuccessful, ETRUE if successful
 */
const EBOOL engineCreateEntityScene(EngineContext* ctx, vec2 pos, vec2 size, const EngineEntityKey key, const EngineTextureKey texkey, GameState state);

/**
 * \brief Destroy all entities from the current scene
 * 
 * \param ctx A valid context
 */
void engineDestroyAllEntities(EngineContext* ctx);

/**
 * \brief Destroy all entities from a specific scene
 * 
 * \param ctx A valid context
 * \param state The state associated with a valid scene
 */
void engineDestroyAllEntitiesScene(EngineContext* ctx, GameState state);

/**
 * \brief Destroy a single entity from the current scene
 * 
 * \param ctx A valid context
 * \param key An entity
 */
void engineDestroyEntity(EngineContext* ctx, EngineEntityKey key);

/**
 * \brief Destroy a single entity from a specific scene
 * 
 * \param ctx A valid context
 * \param The state associated with a valid scene
 * \param An entity
 */
void engineDestroyEntityScene(EngineContext* ctx, GameState state, EngineEntityKey key);

/**
 * \brief Get a single entity from the current scene and return it as a pointer.
 * 
 * \param ctx A valid context
 * \param key An entity
 * 
 * \return Returns the entity or NULL if unsuccessful
 */
EngineEntity* engineGetEntity(EngineContext* ctx, EngineEntityKey key);

/**
 * \brief Get a single entity from a specific scene and return it as a pointer.
 * 
 * \param ctx A valid context
 * \param state The state associated with a valid scene
 * \param An entity
 * 
 * \return Returns the entity or NULL if unsuccessful
 */
EngineEntity* engineGetEntityScene(EngineContext* ctx, GameState state, EngineEntityKey key);

/**
 * \brief Initialize a entities component list, priming it for components.
 * 
 * \param ent The entity
 */
void engineInitEntityComponentList(EngineEntity* ent);

/**
 * \brief Process all entities for the current scene
 * 
 * \param ctx A valid context
 * \param deltaTime The difference in time between frames
 */
void engineProcessEntities(EngineContext* ctx, float deltaTime);

/**
 * \brief Set the draw color for an entity on the current scene.
 * 
 * \param ctx A valid context
 * \param key A valid entity
 * \param r Red normalizezd
 * \param g Green normalized
 * \param b Blue normalized
 */
void engineEntitySetDrawColor(EngineContext* ctx, EngineEntityKey key, float r, float g, float b);

#endif // ENGINE_ENTITY_H


