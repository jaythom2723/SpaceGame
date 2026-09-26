#include "obgl.h"

#include "gtk/gtk.h"
#include "gui.h"

#include <GL/gl.h>
#include <GL/glcorearb.h>
#include <GL/glext.h>
#include <GL/glx.h>
#include <GL/glxext.h>
#include <GL/glxint.h>
#include <GL/glxmd.h>
#include <GL/glxtokens.h>

#include <stdbool.h>

extern struct obgtk_preview preview;

static float vertices[] = {
    1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
};
static int indices[] = {
    0, 1, 3,
    1, 2, 3
};
static unsigned int vbo, vao, ebo;
static int vert, frag, shader;
// static int tex;

static const char* vertSource = "#version 460 core\nlayout (location=0) in vec3 aPos;\nlayout (location=1) in vec2 tPos;\nout vec4 vCol;\nout vec2 tCoord;\nvoid main()\n{\n\tgl_Position=vec4(aPos, 1.0);\n\tvCol = vec4(1.0, 1.0, 1.0, 1.0);\n\ttCoord=tPos;\n}";
static const char* fragSource = "#version 460 core\nout vec4 FragColor;\nin vec4 vCol;\nin vec2 tCoord;\nuniform sampler2D tex;\nvoid main()\n{\n\tFragColor = texture(tex, tCoord) * vCol;\n}";

static int success;
static char infoLog[1024];

bool __compile_shader(int* s, const char* const* src, GLenum type)
{
    extern PFNGLCREATESHADERPROC glCreateShader;
    extern PFNGLSHADERSOURCEPROC glShaderSource;
    extern PFNGLCOMPILESHADERPROC glCompileShader;
    extern PFNGLGETSHADERIVPROC glGetShaderiv;
    extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;

    (*s) = glCreateShader(type);
    glShaderSource(*s, 1, src, NULL);
    glCompileShader(*s);

    glGetShaderiv(*s, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(*s, 1024, NULL, infoLog);
        printf("%s\n", infoLog);
        return false;
    }

    return true;
}

void OBGLonGLAreaRealize(GtkGLArea* self)
{
    extern void __ob_init_gl(void);

    gtk_gl_area_set_required_version(self, 4, 6);
    gtk_gl_area_set_has_depth_buffer(self, TRUE);
    gtk_gl_area_make_current(self);
    if (gtk_gl_area_get_error(self) != NULL)
    {
        printf("Failed to create GL Context!\n");
        return;
    }

    __ob_init_gl();

    bool v = __compile_shader(&vert, (const char* const*)&vertSource, GL_VERTEX_SHADER);
    if (!v)
        printf("Failed to compile Vertex Shader!\n");

    bool f = __compile_shader(&frag, (const char* const*)&fragSource, GL_FRAGMENT_SHADER);
    if (!f)
        printf("Failed to compile the fragment shader!\n");

    extern PFNGLCREATEPROGRAMPROC glCreateProgram;
    extern PFNGLATTACHSHADERPROC glAttachShader;
    extern PFNGLLINKPROGRAMPROC glLinkProgram;
    extern PFNGLGETPROGRAMIVPROC glGetProgramiv;
    extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
    extern PFNGLUSEPROGRAMPROC glUseProgram;

    shader = glCreateProgram();
    glAttachShader(shader, vert);
    glAttachShader(shader, frag);
    glLinkProgram(shader);

    memset(infoLog, 0, 1024 * sizeof(char));
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (success == GL_FALSE)
    {
        glGetProgramInfoLog(shader, 1024, NULL, infoLog);
        printf("%s\n", infoLog);
        return;
    }

    glUseProgram(shader);

    extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
    extern PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
    extern PFNGLGENBUFFERSPROC glGenBuffers;
    extern PFNGLBINDBUFFERPROC glBindBuffer;
    extern PFNGLBUFFERDATAPROC glBufferData;
    extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
    extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
    extern PFNGLUNIFORM1IPROC glUniform1i;

    glUniform1i(glGetUniformLocation(shader, "tex"), 0);
}

gboolean OBGLglAreaRender(GtkGLArea* area, GdkGLContext* context)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    GLint screen_fb = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &screen_fb);

    extern PFNGLUSEPROGRAMPROC glUseProgram;
    extern PFNGLBINDVERTEXARRAYPROC glBindVertexArray;

    glActiveTexture(GL_TEXTURE0);
    glUseProgram(shader);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    
    printf("%p\n%p\n", area, context);

    return TRUE;
}