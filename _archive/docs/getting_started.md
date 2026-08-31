```c
#include "engine.h"
#include "engine_shaders.h"
#include "engine_buffers.h"
#include "engine_textures.h"
#include "engine_entity.h"
#include "engine_renderer.h"
#include "engine_components.h"
#include "engine_utils.h"

float vertices[] = {
	// pos		color				texture
	0.0f, 1.0f,	1.0f, 1.0f, 1.0f,	0.0f, 1.0f
	1.0f, 0.0f, 1.0f, 1.0f, 1.0f,	1.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
	0.0f, 1.0f, 1.0f, 1.0f, 1.0f,	0.0f, 1.0f,
	1.0f, 1.0f,	1.0f, 1.0f, 1.0f,	1.0f, 1.0f,
	1.0f, 0.0f,	1.0f, 1.0f, 1.0f,	1.0f, 0.0f,
};

int main(int argc, char** argv)
{
	// Step 1: initialize the engine
	EngineContext* ctx = engineInit();

	// Step 2: Create the window
	WindowConfig cfg = {
		.title = "Getting Started with Engine!",
		.width = 800,
		.height = 600,
		.resizable = ETRUE,
		.doublebuffer = ETRUE
	};
	engineCreateWindow(ctx, cfg);

	// Step 3: create the shaders
	EngineShader vertex, fragment;
	vertex = engineCreateShader(GL_VERTEX_SHADER);
	fragment = engineCreateShader(GL_FRAGMENT_SHADER);
	
	// Step 4: Create the shader program
	engineCreateProgram(ctx);

	// Step 5: Compile the shaders
	const EBOOL a, b;
	a = engineCompileShader(&vertex, "Shaders/vertex.glsl");
	b = engineCompileShader(&fragment, "Shaders/fragment.glsl");
	if (a == EFALSE || b == EFALSE)
	{
		engineClose(ctx);
		return -1;
	}

	// Step 6: Attach the shaders to the shader program
	// Step 7: Link the shader program
	engineAttachAndLink(ctx, &vertex, &fragment, NULL);

	// Step 8: Initialize the projection matrix
	engineInitProjectionMatrix(ctx);

	// Step 9: Initialize the VAO and VBO for rendering
	engineInitVBOVAOPair(ctx, sizeoF(vertices), vertices);

	// Step 15: Load a valid texture
	const unsigned int TEXKEY = 1;
	engineGenTextureCheckMissing(ctx, TEXKEY, "Texture/dbg_smile.png", EFALSE);

	// Step 16: Create an entity
	const unsigned int ENTKEY = 1;
	vec2 size = { 32, 32 };
	vec2 pos = { 0, 0 };
	engineCreateEntity(ctx, pos, size, ENTKEY, TEXKEY);
	
	// Step 17: Add the renderer component to the entity
	engineAddComponent(ctx, ENTKEY, ENGINE_COMPONENT_RENDERER);

	// Step 10: Create the game loop
	while (engineWindowShouldClose(ctx) == EFALSE)
	{
		// Step 11: Poll events
		engineWindowPollEvents(ctx);

		// Step 12: Clear the screen
		engineSetClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		engineClear();

		// Step 13: Process entities
		engineProcessEntities(ctx, 0.0f);

		// Step 14: Swap window buffers
		engineWindowSwapBuffers(ctx);
	}
}
```