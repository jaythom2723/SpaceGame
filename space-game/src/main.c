#include <obsidian.h>
#include <display/ob_window.h>
#include <graphics/ob_shader.h>
#include <utility/ob_loader.h>

// float vertices[] = {
//     0.0f, 1.0f,     0.0f, 1.0f,
//     1.0f, 0.0f,     1.0f, 0.0f,
//     0.0f, 0.0f,     0.0f, 0.0f,
//     0.0f, 1.0f,     0.0f, 1.0f,
//     1.0f, 1.0f,     1.0f, 1.0f,
//     1.0f, 0.0f,     1.0f, 0.0f    
// };

float vertices[] = {
    0.5f, 0.5f, 0.0f,           1.0f, 1.0f,       // top right
    0.5f, -0.5f, 0.0f,          1.0f, -1.0f,      // bottom right
    -0.5f, -0.5f, 0.0f,     -1.0f, -1.0f,    // bottom left
    -0.5f, 0.5f, 0.0f,      -1.0f, 1.0f,     // top left
};

unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3,
};

extern uint32_t __ob_buf_createvbo(void);
extern bool __ob_buf_deletevbo(uint32_t);
extern bool __ob_buf_bindvbo(uint32_t);
extern bool __ob_buf_setvbodata(uint32_t, size_t, const void*, uint32_t);
extern void __ob_buf_unbindvbo(void);

extern uint32_t __ob_buf_createebo(void);
extern bool __ob_buf_deleteebo(uint32_t);
extern bool __ob_buf_bindebo(uint32_t);
extern bool __ob_buf_setebodata(uint32_t, size_t, const void*, uint32_t);
extern void __ob_buf_unbindebo(void);

extern uint32_t __ob_buf_createvao(void);
extern bool __ob_buf_deletevao(uint32_t);
extern bool __ob_buf_bindvao(uint32_t);
extern bool __ob_buf_setattribpointer(uint32_t,uint32_t,size_t,void*);
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

    // TODO: perhaps make an exposed asset loading system that can create asset prototypes via coordinate files (3D models?)
    uint32_t vao, vbo, ebo;

    vao = __ob_buf_createvao();
    if(__ob_buf_bindvao(vao) == false)
        printf("Error %d!\n", __LINE__);

    vbo = __ob_buf_createvbo();
    (void)__ob_buf_bindvbo(vbo);
    (void)__ob_buf_setvbodata(vbo, sizeof(vertices), vertices, GL_STATIC_DRAW);

    ebo = __ob_buf_createebo();
    (void)__ob_buf_bindebo(ebo);
    (void)__ob_buf_setebodata(ebo, sizeof(indices), indices, GL_STATIC_DRAW);

    __ob_buf_setattribpointer(0, 3, 5 * sizeof(float), (void*)0);
    __ob_buf_setattribpointer(1, 2, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    __ob_buf_unbindvao();
    __ob_buf_unbindebo();
    __ob_buf_unbindvbo();
    
    __ob_buf_deleteebo(ebo);
    __ob_buf_deletevbo(vbo);
    
    struct obsidian_asset* asset = NULL;
    (void)OBLDRloadAsset(OB_ASSET_TEXTURE, asset, "test.bmp");
    OBLDRdestroyAsset(asset);

    while (OBWNDshouldClose() == false)
    {
        OBWNDpollEvents();

        OBSHDRuseProgram(program);
        __ob_buf_bindvao(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        __ob_buf_unbindvao();

        OBWNDswapBuffers();
    }
 
    OBSHDRdestroyProgram(program);
    OBWNDdestroyWindow();
    OBclose();

    return 0;
}