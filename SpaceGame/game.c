#include "engine.h"
#include "engine_shaders.h"
#include "engine_buffers.h"
#include "engine_textures.h"
#include "engine_entity.h"
#include "engine_renderer.h"
#include "engine_components.h"
#include "engine_utils.h"

#include <stdio.h>

float vertices[] = {
	// pos			color				tex
	0.0f, 1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 1.0f,
	1.0f, 0.0f,		1.0f, 1.0f, 1.0f,	1.0f, 0.0f,
	0.0f, 0.0f,		1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	0.0f, 1.0f,		1.0f, 1.0f, 1.0f,	0.0f, 1.0f,
	1.0f, 1.0f,		1.0f, 1.0f, 1.0f,	1.0f, 1.0f,
	1.0f, 0.0f,		1.0f, 1.0f, 1.0f,	1.0f, 0.0f,
};

static void init(void);
static void load(void);

static void input_controller(EngineEntity* ent, EngineContext* ctx, float deltaTime);

static EngineContext* ctx = NULL;

static const unsigned int DBG_PLANET_TEXTURE_KEY = 1;
static const unsigned int DBG_PLANET_ENTITY_KEY = 1;

int main(void)
{
	init();
	if (ctx == NULL)
		return -2;

	load();

	int winwidth, winheight;
	engineWindowGetSize(ctx, &winwidth, &winheight);

	vec2 size = { 32, 32 };
	vec2 pos = { winwidth / 2 - size[0] / 2, winheight / 2 - size[1] / 2 };
	engineCreateEntity(ctx, pos, size, DBG_PLANET_ENTITY_KEY, DBG_PLANET_TEXTURE_KEY);
	engineAddComponent(ctx, DBG_PLANET_ENTITY_KEY, ENGINE_COMPONENT_RENDERER);

	float oldtime = 0.0f;
	float time = 0.0f;
	float deltaTime = 0.0f;

	while (!engineWindowShouldClose(ctx))
	{
		engineWindowPollEvents(ctx);

		time = (float)glfwGetTime();
		deltaTime = time - oldtime;
		oldtime = time;

		engineSetClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		engineClear();

		engineProcessEntities(ctx, deltaTime);

		engineWindowSwapBuffers(ctx);
	}

	engineDestroyAllEntities(ctx);
	engineDestroyAllTextures(ctx);
	engineClose(ctx);

	return 0;
}

void init(void) 
{
	ctx = engineInit();
	WindowConfig cfg = {
		.title = "Test Window",
		.width = 800,
		.height = 600,
		.resizable = ETRUE,
		.doublebuffer = ETRUE
	};

	engineCreateWindow(ctx, cfg);

	// TODO: internalize?
	EngineShader vertex, fragment;
	// TODO: wrap gl shader types to engine shader types
	vertex = engineCreateShader(GL_VERTEX_SHADER);
	fragment = engineCreateShader(GL_FRAGMENT_SHADER);
	const EBOOL a = engineCompileShader(&vertex, "Shaders/vertex.glsl");
	const EBOOL b = engineCompileShader(&fragment, "Shaders/fragment.glsl");
	if (!a || !b)
	{
		engineClose(ctx);
		return;
	}
	engineCreateProgram(ctx);
	engineAttachAndLink(ctx, &vertex, &fragment, NULL);

	engineInitProjectionMatrix(ctx);

	engineInitVBOVAOPair(ctx, sizeof(vertices), vertices);
}

void load(void) 
{
	engineGenTextureCheckMissing(ctx, DBG_PLANET_TEXTURE_KEY, "Textures/planet.png", ETRUE);
}

void input_controller(EngineEntity* ent, EngineContext* ctx, float deltaTime)
{

}