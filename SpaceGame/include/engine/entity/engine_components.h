#ifndef ENGINE_COMPONENTS_H
#define ENGINE_COMPONENTS_H

#include "engine_defs.h"

/**
 * \brief Add a component to an entity.
 * 
 * \param ctx A valid context
 * \param key The entity
 * \param type The type of component to add
 */
void engineAddComponent(EngineContext* ctx, EngineEntityKey key, EngineComponentType type);

/**
 * \brief Add a component to an entity on another scene.
 * 
 * \param ctx A valid context
 * \param key The entity
 * \param type The type of component to add
 * \param state The state associated with a valid scene
 */
void engineAddComponentScene(EngineContext* ctx, EngineEntityKey key, EngineComponentType type, GameState state);

/**
 * \brief Set a component's callback function.
 * 
 * \param ctx A valid context
 * \param key The entity
 * \param type The type of component
 * \param func The function to set
 */
void engineSetComponentCallback(EngineContext* ctx, EngineEntityKey key, EngineComponentType type, EngineComponentFunc func);

/**
 * \brief Remove a component from an entities component list.
 * 
 * \param ctx A valid context
 * \param key The entity
 * \param type The type of component to remove
 * 
 * \return Returns EFALSE if unsuccessful, ETRUE if successful
 */
const EBOOL engineRemoveComponent(EngineContext* ctx, EngineEntityKey key, EngineComponentType type);

/**
 * \brief Check if an entity has a component of a specific type on the current scene.
 * 
 * \param ctx A valid context
 * \param key The entity
 * \param type The component type to check for
 * 
 * \return Returns ETRUE if the entity has the component, otherwise EFALSE
 */
const EBOOL engineHasComponent(EngineContext* ctx, EngineEntityKey key, EngineComponentType type);

/**
 * \brief Check if an entity has a component of a specific type on a specific scene.
 * 
 * \param ctx A valid context
 * \param key The entity
 * \param type The component type to check for
 * \param state The state associated with a valid scene
 * 
 * \return Returns ETRUE if the entity has the component, EFALSE if not.
 */
const EBOOL engineHasComponentScene(EngineContext* ctx, EngineEntityKey key, EngineComponentType type, GameState state);

/**
 * \brief Run a component's set callback function or default callback function.
 * 
 * \param ctx A valid context
 * \param key The entity
 * \param comp The component to run
 * \param deltaTime The difference between frames 
 */
void engineRunComponentFunc(EngineContext* ctx, EngineEntity* key, EngineComponent* comp, float deltaTime);

#endif // ENGINE_COMPONENTS_H
