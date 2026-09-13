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

int main(void)
{
    OBinit();
    OBbootstrap(&program, "Test Window", 800, 600);
    OBinitExtension(OB_EXT_PERLIN_NOISE);

    struct obsidian_asset* primitive_model = NULL;
    OBASTcreatePrimitiveModel(&primitive_model, vertices, sizeof(vertices), indices, sizeof(indices));

    glViewport(0, 0, 800, 600);

    gen_galaxy_one();

    while (OBWNDshouldClose() == false)
    {
        OBWNDpollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        OBSHDRuseProgram(program);

        OBWNDswapBuffers();
    }

    OBASTdestroyAsset(primitive_model);

    OBSHDRdestroyProgram(program);
    OBWNDdestroyWindow();
    OBclose();

    return 0;
}
