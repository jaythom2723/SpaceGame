#include "engine.h"
#include "engine_shaders.h"
#include "engine_buffers.h"
#include "engine_textures.h"
#include "engine_entity.h"
#include "engine_renderer.h"
#include "engine_components.h"
#include "engine_scene.h"
#include "engine_utils.h"

#include <stdio.h>

enum gamestate {
	GAME_GENERATE_GALAXY_MAP,
	GAME_GENERATE_SOLAR_SYSTEM_MAP,
	GAME_GENERATE_PLANET_MAP,
	GAME_DEBUG_TEST_STATE
};

float vertices[] = {
	// pos			tex
	0.0f, 1.0f,		0.0f, 1.0f,
	1.0f, 0.0f,		1.0f, 0.0f,
	0.0f, 0.0f,		0.0f, 0.0f,
	0.0f, 1.0f,		0.0f, 1.0f,
	1.0f, 1.0f,		1.0f, 1.0f,
	1.0f, 0.0f,		1.0f, 0.0f,
};

static void load(void);

static EngineContext* ctx = NULL;

int main(void)
{
	WindowConfig cfg = {
		.title = "Test Window",
		.width = 800,
		.height = 600,
		.resizable = ETRUE,
		.doublebuffer = ETRUE
	};
	ctx = engineBootstrap(cfg, sizeof(vertices), vertices);
	assert(ctx != NULL);

	load();

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

		engineRunScene(ctx, deltaTime);

		engineWindowSwapBuffers(ctx);
	}

	engineDestroyAllScenes(ctx);
	engineDestroyAllEntities(ctx);
	engineDestroyAllTextures(ctx);
	engineClose(ctx);

	return 0;
}

void load(void) 
{

}

void input_controller(EngineEntity* ent, EngineContext* ctx, float deltaTime)
{

}