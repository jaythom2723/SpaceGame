#include "obgl.h"

#include "gui.h"

#include <glad/glad.h>
#include <GL/gl.h>

extern struct obgtk_preview preview;

// static float vertices[] = {
//     1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
//     1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
//     -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
//     -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
// };
// static int indices[] = {
//     0, 1, 3,
//     1, 2, 3
// };
// static int vbo, vao, ebo;
static int vert, frag, shader;
// static int tex;

static const char* vertSource = "";
static const char* fragSource = "";

static int success;
static char infoLog[1024];

void __compile_shader(int* shdr, GLenum type, const char* const* src)
{
    (*shdr) = glCreateShader(type);
    glShaderSource(*shdr, 1, src, NULL);
    glCompileShader(*shdr);

    glGetShaderiv(*shdr, GL_COMPILE_STATUS, &success);
    if (success != 1)
    {
        glGetShaderInfoLog(*shdr, 1024, NULL, infoLog);
        printf("%s\n", infoLog);
        memset(infoLog, 0, 1024);        
        success = 0;
        return;
    }
}

void OBGLonGLAreaRealize(GtkGLArea* self)
{
    gtk_gl_area_set_required_version(self, 4, 6);
    gtk_gl_area_make_current(self);
    if (gtk_gl_area_get_error(self) != NULL)
        return;

    uint32_t width, height;
    width = gtk_widget_get_width(GTK_WIDGET(self));
    height = gtk_widget_get_height(GTK_WIDGET(self));

    glViewport(0, 0, width, height);

    if(gladLoadGL() <= 0)
        return;

    // initialize all of the OpenGL boilerplate stuff
    __compile_shader(&vert, GL_VERTEX_SHADER, (const char* const*)&vertSource);
    __compile_shader(&frag, GL_FRAGMENT_SHADER, (const char* const*)&fragSource);
    shader = glCreateProgram();
    glAttachShader(shader, vert);
    glAttachShader(shader, frag);
    glLinkProgram(shader);

    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (success != 1)
    {
        glGetProgramInfoLog(shader, 1024, NULL, infoLog);
        printf("%s\n", infoLog);
        memset(infoLog, 0, 1024);
        return;
    }
}

gboolean OBGLglAreaRender(GtkGLArea* area, GdkGLContext* context)
{
    glClearColor(.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLint screen_fb = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &screen_fb);

    printf("%p\n%p\n", area, context);

    return TRUE;
}