#include "engine.h"
#include "engine_shaders.h"
#include "engine_buffers.h"
#include "engine_textures.h"
#include "engine_entity.h"
#include "engine_renderer.h"
#include "engine_components.h"
#include "engine_scene.h"
#include "engine_utils.h"
#include "engine_serial.h"

#include <stdio.h>

#include "game_defs.h"
#include "scene_funcs.h"

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

EngineTextureKey STAR_TEXTURE = 0x9669;

GameStar* stars = NULL;
GameSolarSystem* systems = NULL;

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
	engineSetGameState(ctx, GAME_GENERATE_GALAXY_MAP);
	engineSetupCamera(ctx, 150.0f);
	engineSetCameraControl(ctx, ETRUE);

	float oldtime = 0.0f;
	float time = 0.0f;
	float deltaTime = 0.0f;

	engineSerialOpenFile(ENGINE_GALAXY_MAP_SERIAL_FILE, "w", "tmp0");
	engineSerialWriteFile("C3E3M213");
	engineSerialCloseFile();

	while (!engineWindowShouldClose(ctx))
	{
		engineWindowPollEvents(ctx);

		time = (float)glfwGetTime();
		deltaTime = time - oldtime;
		oldtime = time;

		engineSetClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		engineClear();

		engineRunScene(ctx, deltaTime);
		assert(ctx != NULL);

		engineWindowSwapBuffers(ctx);
	}

	free(stars);
	stars = NULL;

	engineDestroyCamera(ctx);
	engineDestroyAllScenes(ctx);
	engineDestroyAllEntities(ctx);
	engineDestroyAllTextures(ctx);
	engineClose(ctx);

	return 0;
}

void load(void) 
{
	engineRegisterScene(ctx, GAME_GENERATE_GALAXY_MAP, game_generate_galaxy_map_init, NULL, NULL);
	engineRegisterScene(ctx, GAME_GENERATE_SOLAR_SYSTEM_MAP, game_generate_solar_system_init, NULL, NULL);
	engineRegisterScene(ctx, GAME_GENERATE_PLANET_MAP, game_generate_planet_map_init, NULL, NULL);
	engineRegisterScene(ctx, GAME_GALAXY_MAP, game_galaxy_map_init, game_galaxy_map_update, game_galaxy_map_draw);

	engineCreateTexture(ctx, STAR_TEXTURE);
	engineGenerateTexture(ctx, STAR_TEXTURE, "Textures/star.png", ETRUE);

	stars = calloc(GAME_MAX_STARS, sizeof(GameStar));
	assert(stars != NULL);

	systems = calloc(GAME_MAX_SOLAR_SYSTEMS, sizeof(GameSolarSystem));
	assert(systems != NULL);
	
	for (int i = 0; i < GAME_MAX_SOLAR_SYSTEMS; i++)
	{
		systems[i].stars = NULL;
		systems[i].planets = NULL;
	}
}

void input_controller(EngineEntity* ent, EngineContext* ctx, float deltaTime)
{
	
}