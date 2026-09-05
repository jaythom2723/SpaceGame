#include <obsidian.h>
#include <display/ob_window.h>
#include <graphics/ob_shader.h>

float vertices[] = {
    0.0f, 1.0f,     0.0f, 1.0f,
    1.0f, 0.0f,     1.0f, 0.0f,
    0.0f, 0.0f,     0.0f, 0.0f,
    0.0f, 1.0f,     0.0f, 1.0f,
    1.0f, 1.0f,     1.0f, 1.0f,
    1.0f, 0.0f,     1.0f, 0.0f    
};

extern uint32_t __ob_buf_createvbo(void);
extern bool __ob_buf_deletevbo(uint32_t);
extern bool __ob_buf_bindvbo(uint32_t);
extern bool __ob_buf_setvbodata(uint32_t, size_t, const void*, uint32_t);

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

    // TODO: internalize
    // TODO: perhaps make an exposed asset loading system that can create asset prototypes via coordinate files (3D models?)
    uint32_t vao, vbo;

    glGenVertexArrays(1, &vao);
    vbo = __ob_buf_createvbo();
    printf("%d\n", vbo);

    glBindVertexArray(vao);

    (void)__ob_buf_setvbodata(vbo, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    while (OBWNDshouldClose() == false)
    {
        OBWNDpollEvents();

        OBSHDRuseProgram(program);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        OBWNDswapBuffers();
    }
 
    OBSHDRdestroyProgram(program);
    OBWNDdestroyWindow();
    OBclose();

    return 0;
}