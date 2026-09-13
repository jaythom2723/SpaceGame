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

#include <cglm/cglm.h>

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

int main(void)
{
    OBinit();

    OBWNDsetTitle("Test Window");
    OBWNDsetSize(800, 600);
    OBWNDcreateWindow();

    obsidian_shader_t vertex, fragment, perlin;
    obsidian_program_t program, noiseProgram;

    vertex = OBSHDRcreateShader("res/shaders/global_vertex.glsl", OBSHDR_VERTEX_SHADER);
    fragment = OBSHDRcreateShader("res/shaders/global_fragment.glsl", OBSHDR_FRAGMENT_SHADER);
    perlin = OBSHDRcreateShader("res/shaders/perlin_noise.glsl", OBSHDR_COMPUTE_SHADER);
    program = OBSHDRcreateProgram();
    noiseProgram = OBSHDRcreateProgram();

    OBSHDRprogramAttach(program, 2, vertex, fragment);
    OBSHDRprogramLink(program);
    OBSHDRdestroyShader(vertex);
    OBSHDRdestroyShader(fragment);
    OBSHDRuseProgram(program);

    OBSHDRprogramAttach(noiseProgram, 1, perlin);
    OBSHDRprogramLink(noiseProgram);
    OBSHDRdestroyShader(perlin);

    struct obsidian_asset* primitive_model = NULL;
    struct obsidian_asset* texture = NULL;

    OBLDRloadAsset(OB_ASSET_TEXTURE, &texture, "res/textures/test.obtf");
    OBASTcreatePrimitiveModel(&primitive_model, vertices, sizeof(vertices), indices, sizeof(indices));

    mat4 projection;
    glm_ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f, projection);

    OBSHDRseti(program, "OBTex", 0);
    OBSHDRsetmat4f(program, "projection", projection);

    glViewport(0, 0, 800, 600);

    // TODO: optimize this so it doesn't use 6,400,000 invokations.
    float* noise = NULL;
    OBSHDRinvoke(noiseProgram, 8000, 8000, (void*)&noise, 8000 * 8000 * sizeof(float));

    free(noise);
    noise = NULL;

    while (OBWNDshouldClose() == false)
    {
        OBWNDpollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        OBSHDRuseProgram(program);

        OBWNDswapBuffers();
    }
    
    OBSHDRdestroyShader(perlin);

    OBASTdestroyAsset(primitive_model);
    OBASTdestroyAsset(texture);

    OBSHDRdestroyProgram(program);
    OBWNDdestroyWindow();
    OBclose();

    return 0;
}
