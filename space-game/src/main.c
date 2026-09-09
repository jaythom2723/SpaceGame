#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include <obsidian.h>
#include <display/ob_window.h>
#include <graphics/ob_shader.h>
#include <utility/ob_loader.h>

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

extern bool __ob_buf_bindvao(uint32_t);
extern bool __ob_buf_unbindvao(void);

int main(void)
{
    OBinit();

    OBWNDsetTitle("Test Window");
    OBWNDsetSize(800, 600);
    OBWNDcreateWindow();

    obsidian_shader_t vertex, fragment;
    obsidian_program_t program;

    vertex = OBSHDRcreateShader("res/shaders/global_vertex.glsl", OBSHDR_VERTEX_SHADER);
    fragment = OBSHDRcreateShader("res/shaders/global_fragment.glsl", OBSHDR_FRAGMENT_SHADER);
    program = OBSHDRcreateProgram();

    OBSHDRprogramAttach(program, 2, vertex, fragment);
    OBSHDRprogramLink(program);
    OBSHDRdestroyShader(vertex);
    OBSHDRdestroyShader(fragment);
    OBSHDRuseProgram(program);

    struct obsidian_asset* primitive_model = NULL;
    struct obsidian_asset* texture = NULL;

    OBLDRloadAsset(OB_ASSET_TEXTURE, &texture, "res/textures/test.obtf");
    OBLDRcreatePrimitive(&primitive_model, texture, vertices, sizeof(vertices), indices, sizeof(indices));
    
    OBSHDRseti(program, "OBTex", 0);

    while (OBWNDshouldClose() == false)
    {
        OBWNDpollEvents();

        OBSHDRuseProgram(program);
        __ob_buf_bindvao(primitive_model->mdlprim.vaoi);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        __ob_buf_unbindvao();

        OBWNDswapBuffers();
    }
 
    OBLDRdestroyAsset(primitive_model);
    OBLDRdestroyAsset(texture);

    OBSHDRdestroyProgram(program);
    OBWNDdestroyWindow();
    OBclose();

    return 0;
}