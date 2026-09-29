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
#include <ext/ob_noise_mask.h>

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

#define PERLIN_NOISE_WIDTH 800
#define PERLIN_NOISE_HEIGHT PERLIN_NOISE_WIDTH

int main(void)
{
    OBinit();
    OBbootstrap(&program, "Project: Celestial", 800, 600);
    OBinitExtension(OB_EXT_PERLIN_NOISE | OB_EXT_NOISE_MASK);

    glViewport(0, 0, 800, 600);

    struct obsidian_asset* mdl;
    OBEXTperlinSetNumLayers(10);
    OBEXTperlinSetFrequency(20);
    OBEXTperlinSetSize(PERLIN_NOISE_WIDTH, PERLIN_NOISE_HEIGHT);
    struct obsidian_asset* perlin = OBEXTperlinInvokeAsset();

    OBEXTnoiseMaskSetSize(PERLIN_NOISE_WIDTH, PERLIN_NOISE_HEIGHT);
    OBEXTnoiseMaskSetGalaxyCenter(0.5f, 0.5f);
    OBEXTnoiseMaskSetGalaxyRadius(0.42f);
    OBEXTnoiseMaskSetSpiralArmCount(4.0f);
    OBEXTnoiseMaskSetSpiralArmTightness(2.0f);
    OBEXTnoiseMaskSetSpiralArmWidth(0.5f);
    struct obsidian_asset* mask = OBEXTnoiseMaskInvokeAsset();

    OBASTcreatePrimitiveModel(&mdl, vertices, sizeof(vertices), indices, sizeof(indices));
    
    ob_entity_t ent = OBECScreateEntity();

    vec3 pos = { 0.0f, 0.0f, 1.0f };
    vec3 scale = { PERLIN_NOISE_WIDTH, PERLIN_NOISE_HEIGHT, 1.0f };
    float rot = 0.0f;

    OBECSaddComponent(ent, OB_TEXTURE_COMPONENT, mask, sizeof(*mask));
    OBECSaddComponent(ent, OB_MODEL_COMPONENT, mdl, sizeof(*mdl));
    OBECSaddComponent(ent, OB_POSITION_COMPONENT, pos, sizeof(pos));
    OBECSaddComponent(ent, OB_SCALE_COMPONENT, scale, sizeof(scale));
    OBECSaddComponent(ent, OB_ROTATION_COMPONENT, &rot, sizeof(rot));

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
    OBASTdestroyAsset(perlin);

    OBSHDRdestroyProgram(program);
    OBWNDdestroyWindow();
    OBclose();

    return 0;
}
