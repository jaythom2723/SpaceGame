#include "engine.h"

#include "engine_shaders.h"
#include "engine_buffers.h"
#include "engine_textures.h"
#include "engine_utils.h"
#include "engine_logger.h"

#include <cglm/cglm.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static EngineContext* int_ctx = NULL;
static EngineLinkedList* keylist = NULL;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	mat4 projection;
	glm_ortho(0.0f, (float)width, (float)height, 0.0f, -1.f, 1.f, projection);
	engineSetMatrix4fv(int_ctx, "projection", projection);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (yoffset < 0)
	{
		int_ctx->zoomin = EFALSE;
		int_ctx->zoomout = ETRUE;
	}
	else if (yoffset > 0)
	{
		int_ctx->zoomin = ETRUE;
		int_ctx->zoomout = EFALSE;
	}
	int_ctx->scroll_debounce_timer = 0.05f;
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

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (glfwWindowShouldClose(window) == GLFW_TRUE)
		return;

	if (action == GLFW_PRESS)
		int_ctx->mousebtns[button] = ETRUE;
	else
		int_ctx->mousebtns[button] = EFALSE;
}

void cursor_pos_callback(GLFWwindow* window, double x, double y)
{
	int_ctx->mousex = (float)x;
	int_ctx->mousey = (float)y;
}

EngineContext* engineInit(void)
{
	EngineContext* engine = malloc(sizeof(EngineContext));
	if (engine == NULL)
	{
		fprintf(stderr, "Failed to initialize the engine!\n");
		return NULL;
	}

	// initialize logger above all else
	// TODO: error handler
	engineInitializeLogger(engine);
	engineWriteMessage(engine, "Engine logger has been successfully initialized!", ELOG_MSGTYPE_INFORM);

	if (glfwInit() != GLFW_TRUE)
	{
		fprintf(stderr, "Failed to initialize GLFW!\n");
		engineWriteMessage(engine, "Failed to initialize GLFW!\n", ELOG_MSGTYPE_ERROR);
		engineClose(engine);
		return NULL;
	}

	// initialize other engine-level things
	engine->program = 0;
	memset(engine->textures, 0, ENGINE_MAX_TEXTURES * sizeof(EngineTexturePair));
	engine->ntextures = 0;
	engine->state = -1;
	engine->zoomfac = ENGINE_DEFAULT_ZOOM_FACTOR;
	engine->zoomin = EFALSE;
	engine->zoomout = EFALSE;
	engine->scroll_debounce_timer = 0.0f;
	engine->mousex = 0.0f;
	engine->mousey = 0.0f;

	memset(engine->scenes, 0, ENGINE_MAX_SCENES * sizeof(EngineScene*));

	keylist = engineCreateLinkedList(engine->keys, NULL, ENGINE_NUM_KEYS, sizeof(EBOOL));
	assert(keylist != NULL);

	engine->camera = NULL;

	int_ctx = engine;
	engineWriteMessage(engine, "Engine is initialized!", ELOG_MSGTYPE_INFORM);

	return engine;
}

EngineContext* engineBootstrap(WindowConfig cfg, size_t vertBytes, float* vertices)
{
	EngineContext* ctx = engineInit();
	engineWriteMessage(ctx, "Bootstrapping engine context...", ELOG_MSGTYPE_INFORM);
	if (ctx == NULL)
	{
		return NULL;
	}

	engineCreateWindow(ctx, cfg);

	engineWriteMessage(ctx, "Initializing OpenGL Shader Program", ELOG_MSGTYPE_INFORM);
	EngineShader vertex, fragment;
	vertex = engineCreateShader(ENGINE_VERTEX_SHADER);
	fragment = engineCreateShader(ENGINE_FRAGMENT_SHADER);
	const EBOOL a = engineCompileShader(&vertex, "Shaders/vertex.glsl");
	const EBOOL b = engineCompileShader(&fragment, "Shaders/fragment.glsl");
	if (a == EFALSE || b == EFALSE)
	{
		engineWriteMessage(ctx, "Failed to compile one or both shaders. (engne.c>engineBootstrap())", ELOG_MSGTYPE_ERROR);
		engineClose(ctx);
		return NULL;
	}

	engineCreateProgram(ctx);
	engineAttachAndLinkv(ctx, 2, &vertex, &fragment);
	engineInitProjectionMatrix(ctx);

	engineWriteMessage(ctx, "Initializing the Vertex Buffer Object and Vertex Array Object pair.", ELOG_MSGTYPE_INFORM);
	engineInitVBOVAOPair(ctx, vertBytes, vertices);
	
	return ctx;
}

EBOOL engineCreateWindow(EngineContext* ctx, const WindowConfig config)
{
	engineWriteMessage(ctx, "Initializing GLFW and the window...", ELOG_MSGTYPE_INFORM);
	glfwWindowHint(GLFW_RESIZABLE, config.resizable);
	glfwWindowHint(GLFW_DOUBLEBUFFER, config.doublebuffer);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	engineWriteMessage(ctx, "Successfully initialized GLFW...", ELOG_MSGTYPE_INFORM);
	engineWriteMessage(ctx, "Creating GLFW Window...", ELOG_MSGTYPE_INFORM);
	ctx->window = glfwCreateWindow(config.width, config.height, config.title, NULL, NULL);
	if (ctx->window == NULL)
	{
		char* err;
		glfwGetError(&err);
		engineWriteMessage(ctx, "Failed to create a GLFW window!", ELOG_MSGTYPE_ERROR);
		engineWriteMessage(ctx, err, ELOG_MSGTYPE_ERROR);
		return EFALSE;
	}

	glfwSetFramebufferSizeCallback(ctx->window, framebuffer_size_callback);
	glfwSetKeyCallback(ctx->window, key_callback);
	glfwSetScrollCallback(ctx->window, scroll_callback);
	glfwSetMouseButtonCallback(ctx->window, mouse_button_callback);
	glfwSetCursorPosCallback(ctx->window, cursor_pos_callback);

	engineWriteMessage(ctx, "Initializing OpenGL...", ELOG_MSGTYPE_INFORM);
	glfwMakeContextCurrent(ctx->window);
	if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) != ETRUE)
	{
		fprintf(stderr, "Could not initialize GLAD!\n");
		engineWriteMessage(ctx, "Failed to initialize GLAD and/or OpenGL!", ELOG_MSGTYPE_ERROR);
		engineClose(ctx);
		return EFALSE;
	}

	return ETRUE;
}

void engineClose(EngineContext* ctx)
{
	int_ctx = NULL;
	engineCloseLogger(ctx);
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
	engineWriteMessage(ctx, "Initializing Projection Matrix!", ELOG_MSGTYPE_INFORM);

	int width, height;
	glfwGetWindowSize(ctx->window, &width, &height);
	
	mat4 projection;
	glm_ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f, projection);

	engineUseShader(ctx);
	engineSetMatrix4fv(ctx, "projection", projection);
	engineSetInteger(ctx, "image", 0);

	engineWriteMessage(ctx, "Projection Matrix initialized.", ELOG_MSGTYPE_INFORM);
}

const EBOOL engineSetupCamera(EngineContext* ctx, float speed)
{
	engineWriteMessage(ctx, "Initializing Camera.", ELOG_MSGTYPE_INFORM);

	if (ctx->camera != NULL)
	{
		engineWriteMessage(ctx, "Cannot create more than one camera at a time!", ELOG_MSGTYPE_WARN);
		return EFALSE;
	}

	ctx->camera = malloc(sizeof(EngineCamera));
	if (ctx->camera == NULL)
	{
		engineWriteMessage(ctx, "Allocation Failure (engine.c>engineSetupCamera())", ELOG_MSGTYPE_WARN);
		return EFALSE;
	}

	ctx->camera->speed = speed;
	ctx->camera->vx = 0.f;
	ctx->camera->vy = 0.f;
	ctx->camera->x = 0.f;
	ctx->camera->y = 0.f;
	ctx->camera->control = EFALSE;

	return ETRUE;
}

void engineDestroyCamera(EngineContext* ctx)
{
	if (ctx->camera == NULL)
	{
		engineWriteMessage(ctx, "No camera exists.", ELOG_MSGTYPE_INFORM);
		return;
	}

	free(ctx->camera);
	ctx->camera = NULL;
}

void engineSetCameraControl(EngineContext* ctx, EBOOL value)
{
	ctx->camera->control = value;
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
