#include <obsidian.h>
#include <display/ob_window.h>
#include <graphics/ob_shader.h>

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
    // OBSHDRprogramLink(program);
    // OBSHDRdestroyShader(vertex);
    // OBSHDRdestroyShader(fragment);

    while (OBWNDshouldClose() == false)
    {
        OBWNDpollEvents();

        OBSHDRuseProgram(program);

        OBWNDswapBuffers();
    }
 
    OBSHDRdestroyProgram(program);
    OBWNDdestroyWindow();
    OBclose();

    return 0;
}