#ifndef ENGINE_DEFS_H
#define ENGINE_DEFS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#define CGLM_VERSION_MAJOR			0
#define CGLM_VERSION_MINOR			9
#define GL_VERSION_MAJOR			4
#define GL_VERSION_MINOR			3
#define ENGINE_VERSION_MAJOR		0
#define ENGINE_VERSION_MINOR		1
#define ENGINE_VERSION_PATCH		0

#define ENGINE_NUM_KEYS				1024
#define ENGINE_NUM_MOUSE_BUTTONS	2

#define ENGINE_MAX_TEXTURES			1024
#define ENGINE_MAX_ENTITIES			0x1000
#define ENGINE_MAX_COMPONENTS		0x10
#define ENGINE_MAX_SCENES			0xFF

#define ENGINE_MISSING_TEXTURE_KEY ENGINE_MAX_TEXTURES-1

#define MAKE_ENT_PAIR(k,pos,sz,tk) deprecated DO NOT USE.

#define ENGINE_DEFAULT_ZOOM_FACTOR 1.0

typedef unsigned char EBOOL;
#define ETRUE 1
#define EFALSE 0

typedef unsigned int EngineShader;
typedef unsigned int EngineProgram;

typedef unsigned int EngineTextureKey;
typedef int EngineEntityKey;

typedef unsigned int EngineComponentID;

typedef enum gamestate GameState;
typedef enum engine_serial_file_type EngineSerialFileType;

typedef struct engine_context EngineContext;
typedef struct engine_component EngineComponent;
typedef struct engine_entity EngineEntity;
typedef struct engine_scene EngineScene;
typedef struct engine_camera EngineCamera;
typedef struct engine_noise_mask EngineNoiseMask;
typedef struct engine_ui_element EngineUIElement;

typedef void (*EngineComponentFunc)(EngineEntity*,EngineContext*,float);
typedef void (*EngineSceneFunc)(EngineScene*,EngineContext*,float);
typedef void (*EngineUIButtonFunc)(EngineUIElement*,EngineContext*,EngineScene*,float);

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

#define ENGINE_IMG_READ_ONLY	GL_READ_ONLY
#define ENGINE_IMG_WRITE_ONLY	GL_WRITE_ONLY
#define ENGINE_RGBA32F			GL_RGBA32F

enum engine_serial_file_type {
	ENGINE_GALAXY_MAP_SERIAL_FILE,
	ENGINE_SOLAR_SYSTEM_SERIAL_FILE
};

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

typedef enum engine_ui_type {
	ENGINE_UI_PANEL,
	ENGINE_UI_TEXT,
	ENGINE_UI_BUTTON,
	ENGINE_UI_TEXT_INPUT,
	ENGINE_UI_CHECK_BUTTON,
	ENGINE_UI_RADIAL_BUTTON
} EngineUIElementType;

/**
 * \brief The type of noise mask.
 */
typedef enum noise_type {
	ENGINE_NOISEMASK_GALAXY,
} EngineNoiseType;

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
 * \brief Perlin noise mask.
 */
struct engine_noise_mask {
	EngineNoiseType type;
	int width;
	int height;

	union {
		struct {
			float bulgeintensity;
			float bulgecenter;
			float armcurve;
			float arms;
			float armthickness;
			float inradius;
			float outradius;
		} mask_galaxy;
	};
};

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
	float x, y;
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

struct engine_ui_element
{
	EngineUIElement** parent;

	EngineUIElementType type;
	EBOOL visible;

	union {
		char* text;
		char* inputbuffer;
		EngineUIButtonFunc func;
		EBOOL enabled;
	};

	float x, y;
	float w, h;
	float padding;
	float margin;
	float border;

	vec3 bgcol;			// background color
	vec3 fgcol;			// foreground color
	vec3 borcol;		// border color
};

/** 
 * \brief The engine itself, nothing can function without this being initialized!
 */
struct engine_context {
	GLFWwindow* window;

	GameState state;

	EngineTexturePair textures[ENGINE_MAX_TEXTURES];
	EBOOL keys[ENGINE_NUM_KEYS];
	EBOOL mousebtns[ENGINE_NUM_MOUSE_BUTTONS];
	EngineScene* scenes[ENGINE_MAX_SCENES];

	EngineCamera* camera;
	EngineProgram program;

	FILE* logfile;

	unsigned int vao;
	unsigned int ntextures;

	float scroll_debounce_timer;
	EBOOL zoomin, zoomout;
	float zoomfac;

	float mousex, mousey;
};

#endif // ENGINE_DEFS_H