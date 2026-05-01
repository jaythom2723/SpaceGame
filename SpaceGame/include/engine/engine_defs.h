#ifndef ENGINE_DEFS_H
#define ENGINE_DEFS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define ENGINE_MAX_TEXTURES			1024
#define ENGINE_MAX_ENTITIES			0x1000
#define ENGINE_MAX_COMPONENTS		0x10
#define ENGINE_NUM_KEYS				1024
#define ENGINE_MAX_SCENES			0xFF

#define ENGINE_MISSING_TEXTURE_KEY ENGINE_MAX_TEXTURES-1

#define MAKE_ENT_PAIR(k,pos,sz,tk) deprecated DO NOT USE.

typedef unsigned char EBOOL;
#define ETRUE 1
#define EFALSE 0

typedef unsigned int EngineShader;
typedef unsigned int EngineProgram;

typedef unsigned int EngineTextureKey;
typedef unsigned int EngineEntityKey;

typedef unsigned int EngineComponentID;

typedef enum gamestate GameState;

typedef struct engine_context EngineContext;
typedef struct engine_component EngineComponent;
typedef struct engine_entity EngineEntity;
typedef struct engine_scene EngineScene;
typedef struct engine_camera EngineCamera;

typedef void (*EngineComponentFunc)(EngineEntity*,EngineContext*,float);
typedef void (*EngineSceneFunc)(EngineScene*,EngineContext*,float);

#define EKEY_ESCAPE		GLFW_KEY_ESCAPE
#define EKEY_W			GLFW_KEY_W
#define EKEY_S			GLFW_KEY_S
#define EKEY_A			GLFW_KEY_A
#define EKEY_D			GLFW_KEY_D
#define EKEY_UP			GLFW_KEY_UP
#define EKEY_DOWN		GLFW_KEY_DOWN
#define EKEY_RIGHT		GLFW_KEY_RIGHT
#define EKEY_LEFT		GLFW_KEY_LEFT

#define ENGINE_VERTEX_SHADER	GL_VERTEX_SHADER
#define ENGINE_FRAGMENT_SHADER	GL_FRAGMENT_SHADER
#define ENGINE_GEOEMTRY_SHADER	GL_GEOMETRY_SHADER
#define ENGINE_COMPUTE_SHADER	GL_COMPUTE_SHADER

// offsets the component ptr by the values assigned by the compiler
/**
 * \brief Types of components embedded into the engine
 * 
 * You can create your own component types as well
 */
typedef enum component_type {
	ENGINE_COMPONENT_RENDERER,		// 0
	ENGINE_COMPONENT_TICK,			// 1
	ENGINE_COMPONENT_CONTROLLER,	// ...
} EngineComponentType;

/**
 * \brief The window config tells the engine how to setup the window
 */
typedef struct window_config {
	char* title;
	unsigned int width, height;
	EBOOL resizable, doublebuffer;
} WindowConfig;

/**
 * \brief A texture handle that contains all necessary texture information
 */
typedef struct engine_texture {
	unsigned int id;
	unsigned int width, height;
	unsigned int internalformat;
	unsigned int imageformat;
	unsigned int wraps, wrapt;
	unsigned int filtermax, filtermin;
} EngineTexture;

/**
 * \brief Used to make a pseudo-dictionary for the engine
 */
typedef struct engine_texture_pair {
	EngineTextureKey key;
	EngineTexture* value;
} EngineTexturePair;

/**
 * \brief An entity, contains position, size, texture, and component data
 */
struct engine_entity {
	float x, y;
	float width, height;
	float r, g, b;
	EngineEntityKey key;
	EngineTextureKey texkey;
	EngineComponent* components;
};

/**
 * \brief The camera for a game.
 */
struct engine_camera {
	float vx, vy;
	float speed;

	EBOOL control;
};

/**
 * \brief A component used for the functionality of entities
 */
struct engine_component {
	EngineComponentID id;
	EngineComponentType type;
	EngineComponentFunc func;
};

/**
 * \brief A standard link list data structure
 */
typedef struct linklist {
	char* lista;
	char* listb;

	size_t typesize;
	size_t arrsize;

	EBOOL aalloc;
	EBOOL balloc;
} EngineLinkedList;

/**
 * \brief A scene that gets run based off of GameState.
 */
struct engine_scene {
	EngineEntity* entities;
	unsigned int nentities;
	EBOOL initialized;

	EngineSceneFunc init;
	EngineSceneFunc update;
	EngineSceneFunc draw;
};

/** 
 * \brief The engine itself, nothing can function without this being initialized!
 */
struct engine_context {
	GLFWwindow* window;

	GameState state;

	EngineTexturePair textures[ENGINE_MAX_TEXTURES];
	EBOOL keys[ENGINE_NUM_KEYS];
	EngineScene* scenes[ENGINE_MAX_SCENES];

	EngineCamera* camera;
	EngineProgram program;

	unsigned int vao;
	unsigned int ntextures;
};

#endif // ENGINE_DEFS_H