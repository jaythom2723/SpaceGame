#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>

#include <obsidian.h>
#include <display/ob_window.h>
#include <graphics/ob_shader.h>
#include <graphics/ob_render.h>
#include <utility/ob_loader.h>
#include <assets/ob_asset.h>
#include <ecs/ob_ecs.h>
#include <ext/ob_perlin.h>

#include <cglm/cglm.h>

#include "gen/gen_galaxy.h"

float vertices[] = {
    0.5f, 0.5f, 0.0f,           1.0f, 1.0f,       // top right
    0.5f, -0.5f, 0.0f,          1.0f, 0.0f,      // bottom right
    -0.5f, -0.5f, 0.0f,     0.0f, 0.0f,    // bottom left
    -0.5f, 0.5f, 0.0f,      0.0f, 1.0f,     // top left
};

unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3,
};

ob_entity_t* entities = NULL;
obsidian_program_t program;

#define PERLIN_NOISE_WIDTH 1000
#define PERLIN_NOISE_HEIGHT PERLIN_NOISE_WIDTH

int main(void)
{
    OBinit();
    OBbootstrap(&program, "Test Window", 800, 600);
    OBinitExtension(OB_EXT_PERLIN_NOISE);

    OBEXTperlinSetSize(PERLIN_NOISE_WIDTH, PERLIN_NOISE_HEIGHT);
    float* noise = OBEXTperlinInvoke();

    printf("1 to 100\n");
    for (int i = 0; i < PERLIN_NOISE_WIDTH * PERLIN_NOISE_HEIGHT; i++)
    {
        printf("[%d] %.02f\n", i, noise[i]);
    }

    free(noise);
    noise = NULL;

    glViewport(0, 0, 800, 600);

    struct obsidian_asset* tex;
    struct obsidian_asset* mdl;

    // welp....

    OBASTcreatePrimitiveModel(&mdl, vertices, sizeof(vertices), indices, sizeof(indices));
    OBLDRloadAsset(OB_ASSET_TEXTURE, &tex, "res/textures/test.obtf");
    

    ob_entity_t ent = OBECScreateEntity();

    vec3 pos = { (800.0f/2.0f)-(64.0f/2.0f), (600.0f/2.0f)-(64.0f/2.0f), 1.0f };
    vec3 scale = { 64.0f, 64.0f, 1.0f };
    float rot = 0.0f;

    OBECSaddComponent(ent, OB_TEXTURE_COMPONENT, tex, sizeof(*tex));
    OBECSaddComponent(ent, OB_MODEL_COMPONENT, mdl, sizeof(*mdl));
    OBECSaddComponent(ent, OB_POSITION_COMPONENT, pos, sizeof(pos));
    OBECSaddComponent(ent, OB_SCALE_COMPONENT, scale, sizeof(scale));
    OBECSaddComponent(ent, OB_ROTATION_COMPONENT, &rot, sizeof(rot));

    // So that's what it's been doing and I have ZERO clue why. Don't we just love it!

    while (OBWNDshouldClose() == false)
    {
        OBWNDpollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        OBSHDRuseProgram(program);
        OBRNDRdrawEntity(ent);

        OBWNDswapBuffers();
    }

    OBECSdestroyEntity(&ent);

    OBASTdestroyAsset(mdl);
    OBASTdestroyAsset(tex);

    OBSHDRdestroyProgram(program);
    OBWNDdestroyWindow();
    OBclose();

    return 0;
}
