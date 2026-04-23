#include "engine.h"

#include "engine_shaders.h"
#include "engine_buffers.h"
#include "engine_textures.h"
#include "engine_utils.h"

#include <cglm/cglm.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static EngineLinkedList* keylist = NULL;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key >= 0 && key <= ENGINE_NUM_KEYS)
	{
		if (action == GLFW_PRESS)
			engineSetValue(keylist, key, ETRUE);
		else if (action == GLFW_RELEASE)
			engineSetValue(keylist, key, EFALSE);
	}
}

EngineContext* engineInit(void)
{
	EngineContext* engine = malloc(sizeof(EngineContext));
	if (engine == NULL)
	{
		fprintf(stderr, "Failed to initialize the engine!\n");
		return NULL;
	}

	if (glfwInit() != GLFW_TRUE)
	{
		fprintf(stderr, "Failed to initialize GLFW!\n");
		engineClose(engine);
		return NULL;
	}

	// initialize other engine-level things
	engine->program = 0;
	memset(engine->textures, 0, ENGINE_MAX_TEXTURES * sizeof(EngineTexturePair));
	engine->ntextures = 0;
	engine->state = -1;

	memset(engine->scenes, NULL, ENGINE_MAX_SCENES * sizeof(EngineScene*));

	keylist = engineCreateLinkedList(engine->keys, NULL, ENGINE_NUM_KEYS, sizeof(EBOOL));
	assert(keylist != NULL);

	return engine;
}

EngineContext* engineBootstrap(WindowConfig cfg, size_t vertBytes, float* vertices)
{
	EngineContext* ctx = engineInit();
	if (ctx == NULL)
		return NULL;

	engineCreateWindow(ctx, cfg);

	EngineShader vertex, fragment;
	vertex = engineCreateShader(ENGINE_VERTEX_SHADER);
	fragment = engineCreateShader(ENGINE_FRAGMENT_SHADER);
	const EBOOL a = engineCompileShader(&vertex, "Shaders/vertex.glsl");
	const EBOOL b = engineCompileShader(&fragment, "Shaders/fragment.glsl");
	if (a == EFALSE || b == EFALSE)
	{
		engineClose(ctx);
		return NULL;
	}

	engineCreateProgram(ctx);
	engineAttachAndLink(ctx, &vertex, &fragment, NULL);
	engineInitProjectionMatrix(ctx);
	engineInitVBOVAOPair(ctx, vertBytes, vertices);
	
	return ctx;
}

EBOOL engineCreateWindow(EngineContext* ctx, const WindowConfig config)
{
	glfwWindowHint(GLFW_RESIZABLE, config.resizable);
	glfwWindowHint(GLFW_DOUBLEBUFFER, config.doublebuffer);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	ctx->window = glfwCreateWindow(config.width, config.height, config.title, NULL, NULL);
	if (ctx->window == NULL)
	{
		char* err;
		glfwGetError(&err);
		fprintf(stderr, "Could not create a GLFW window!\n\t%s\n", err);
		return EFALSE;
	}

	glfwSetFramebufferSizeCallback(ctx->window, framebuffer_size_callback);
	glfwSetKeyCallback(ctx->window, key_callback);

	glfwMakeContextCurrent(ctx->window);
	if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) != ETRUE)
	{
		fprintf(stderr, "Could not initialize GLAD!\n");
		engineClose(ctx);
		return EFALSE;
	}

	return ETRUE;
}

void engineClose(EngineContext* ctx)
{
	if (glIsProgram(ctx->program))
		engineDeleteProgram(ctx);
	if (ctx->window != NULL)
	{
		glfwDestroyWindow(ctx->window);
		glfwTerminate();
	}
	free(ctx);
	ctx = NULL;
}

// matrix linear-algebra bullshit
// this shit is retarded, but opengl needs it for proper rendering
// woohoo
void engineInitProjectionMatrix(EngineContext* ctx)
{
	int width, height;
	glfwGetWindowSize(ctx->window, &width, &height);
	
	mat4 projection;
	glm_ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f, projection);

	engineUseShader(ctx);
	engineSetMatrix4fv(ctx, "projection", projection);
	engineSetInteger(ctx, "image", 0);
}

void engineSetGameState(EngineContext* ctx, const GameState state)
{
	ctx->state = state;
}

const GameState engineGetGameState(EngineContext* ctx)
{
	return ctx->state;
}

// glfw wrapper functions
EBOOL engineWindowShouldClose(EngineContext* ctx)
{
	return glfwWindowShouldClose(ctx->window);
}

void engineSetWindowClose(EngineContext* ctx, EBOOL value)
{
	glfwSetWindowShouldClose(ctx->window, value);
}

void engineWindowPollEvents(EngineContext* ctx)
{
	glfwPollEvents();
	engineGetValues(keylist, ctx->keys, NULL);
}

void engineWindowSwapBuffers(EngineContext* ctx)
{
	glfwSwapBuffers(ctx->window);
}

void engineWindowGetSize(EngineContext* ctx, int* width, int* height)
{
	glfwGetWindowSize(ctx->window, width, height);
}

// opengl wrapper functions
void engineSetClearColor(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}

void engineClear(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
}
