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
 * \brief Bootstrap the engine to handle all boilerplater initialization.
 * 
 * This function is for users who do not wish to implement their own shaders
 * Be sure to have Shaders/vertex.glsl and Fragment/fragment.glsl, as they are
 * required for the function to work properly.
 * 
 * If they do not exist, a default shader source will be provided to the shaders
 * 
 * \param cfg The window configuration
 * \param vertBytes The size of vertices in bytes
 * \param vertices The vertex data to assign to a VBO and VAO pair
 * 
 * \return Returns the fully initialized context
 */
EngineContext* engineBootstrap(WindowConfig cfg, size_t vertBytes, float* vertices);

/**
 * \brief Create the window for the game via WindowConfig.
 * 
 * \param ctx A valid context
 * \param config The config used to inform GLFW about how to create the window
 * 
 * \return Returns EFALSE if unsuccessful in creating the window
 */
EBOOL engineCreateWindow(EngineContext* ctx, WindowConfig config);

/**
 * \brief Destroy everything the engine has created.
 * 
 * \param ctx A valid context
 */
void engineClose(EngineContext* ctx);

/**
 * \brief Initialize the projection matrix for rendering.
 * 
 * \param ctx A valid context
 */
void engineInitProjectionMatrix(EngineContext* ctx);

/**
 * \brief Setup a game camera.
 * 
 * \param ctx A valid context
 * 
 * \return Returns ETRUE if able to create a camera, EFALSE if otherwise
 */
const EBOOL engineSetupCamera(EngineContext* ctx, float speed);

/**
 * \brief Destroy the game camera.
 * 
 * \param ctx A valid context
 */
void engineDestroyCamera(EngineContext* ctx);

/**
 * \brief Set if the camera should be in control or not.
 * 
 * \param ctx A valid context
 * \param value The value to set
 */
void engineSetCameraControl(EngineContext* ctx, EBOOL value);

/**
 * \brief Set the engine's internal reference to the gamestate.
 * 
 * \param ctx A valid context
 * \param state The gamestate to set
 */
void engineSetGameState(EngineContext* ctx, const GameState state);

/**
 * \brief Get the engine's internal reference to the gamestate.
 * 
 * \param ctx A valid context
 * 
 * \return Returns the gamestate as a pointer
 */
const GameState engineGetGameState(EngineContext* ctx);

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
