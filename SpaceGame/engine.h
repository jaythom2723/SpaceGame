#ifndef ENGINE_H
#define ENGINE_H

#include <cglm/cglm.h>

#include "engine_defs.h"

/**
 * \brief Initialize the engine and return the initialized context.
 * 
 * \return The initialized context
 */
EngineContext* engineInit(void);

/**
 * \brief Create the window for the game via WindowConfig.
 * 
 * \param ctx The context
 * \param config The config used to inform GLFW about how to create the window
 * 
 * \return Returns EFALSE if unsuccessful in creating the window
 */
EBOOL engineCreateWindow(EngineContext* ctx, WindowConfig config);

/**
 * \brief Destroy everything the engine has created.
 * 
 * \param ctx The context
 */
void engineClose(EngineContext* ctx);

/**
 * \brief Initialize the projection matrix for rendering.
 * 
 * \param ctx The context
 */
void engineInitProjectionMatrix(EngineContext* ctx);

#define engineDrawSprite renamed_engineRenderEntity(...)

// glfw wrapper functions

/**
 * \brief wrapper for glfwWindowShouldClose().
 * 
 * \param ctx The context.
 * 
 * \return Returns whether or not the window should close.
 */
EBOOL engineWindowShouldClose(EngineContext* ctx);

/**
 * \brief wrapper for glfwSetWindowShouldClose().
 * 
 * \param ctx The context
 * \param value true or false
 */
void engineSetWindowClose(EngineContext* ctx, EBOOL value);

/**
 * \brief wrapper for glfwPollEvents().
 * 
 * \param ctx The context
 */
void engineWindowPollEvents(EngineContext* ctx);

/**
 * \brief wrapper for glfwSwapBuffers().
 * 
 * \param ctx The context
 */
void engineWindowSwapBuffers(EngineContext* ctx);

/**
 * \brief wrapper for glfwWindowGeSize().
 * 
 * \param ctx The context
 * \param width The width to return
 * \param height The height to return
 */
void engineWindowGetSize(EngineContext* ctx, int* width, int* height);

// gl wrapper functions

/**
 * \brief wrapper for glClearColor().
 * 
 * \param r the red color (0-1)
 * \param g the green color (0-1)
 * \param b the blue color (0-1)
 * \param a the alpha channel (0-1)
 */
void engineSetClearColor(float r, float g, float b, float a);

/**
 * \brief wrapper for glClear().
 */
void engineClear(void);

#endif // ENGINE_H
