#include <GL/gl.h>
#include <GL/glcorearb.h>
#include <GL/glext.h>
#include <GL/glx.h>
#include <GL/glxext.h>
#include <GL/glxint.h>
#include <GL/glxmd.h>
#include <GL/glxtokens.h>

#include <gtk-4.0/gtk/gtk.h>

PFNGLCREATESHADERPROC               glCreateShader;
PFNGLSHADERSOURCEPROC               glShaderSource;
PFNGLCOMPILESHADERPROC              glCompileShader;
PFNGLGETSHADERIVPROC                glGetShaderiv;
PFNGLGETSHADERINFOLOGPROC           glGetShaderInfoLog;

PFNGLCREATEPROGRAMPROC              glCreateProgram;
PFNGLATTACHSHADERPROC               glAttachShader;
PFNGLLINKPROGRAMPROC                glLinkProgram;
PFNGLGETPROGRAMIVPROC               glGetProgramiv;
PFNGLGETPROGRAMINFOLOGPROC          glGetProgramInfoLog;
PFNGLUSEPROGRAMPROC                 glUseProgram;

PFNGLGENVERTEXARRAYSPROC            glGenVertexArrays;
PFNGLBINDVERTEXARRAYPROC            glBindVertexArray;
PFNGLGENBUFFERSPROC                 glGenBuffers;
PFNGLBINDBUFFERPROC                 glBindBuffer;
PFNGLBUFFERDATAPROC                 glBufferData;
PFNGLVERTEXATTRIBPOINTERPROC        glVertexAttribPointer;
PFNGLENABLEVERTEXATTRIBARRAYPROC    glEnableVertexAttribArray;

PFNGLGETUNIFORMLOCATIONPROC         glGetUniformLocation;
PFNGLUNIFORM1IPROC                  glUniform1i;

#define PROC(t, n) (t) glXGetProcAddress((GLubyte*)n);

void __ob_init_gl(void)
{
    glCreateShader = PROC(PFNGLCREATESHADERPROC, "glCreateShader");
    glShaderSource = PROC(PFNGLSHADERSOURCEPROC, "glShaderSource");
    glCompileShader = PROC(PFNGLCOMPILESHADERPROC, "glCompileShader");
    glGetShaderiv = PROC(PFNGLGETSHADERIVPROC, "glGetShaderiv");
    glGetShaderInfoLog = PROC(PFNGLGETSHADERINFOLOGPROC, "glGetShaderInfoLog");

    glCreateProgram = PROC(PFNGLCREATEPROGRAMPROC, "glCreateProgram");
    glAttachShader = PROC(PFNGLATTACHSHADERPROC, "glAttachShader");
    glLinkProgram = PROC(PFNGLLINKPROGRAMPROC, "glLinkProgram");
    glGetProgramiv = PROC(PFNGLGETPROGRAMIVPROC, "glGetProgramiv");
    glGetProgramInfoLog = PROC(PFNGLGETPROGRAMINFOLOGPROC, "glGetProgramInfoLog");
    glUseProgram = PROC(PFNGLUSEPROGRAMPROC, "glUseProgram");

    glGenVertexArrays = PROC(PFNGLGENVERTEXARRAYSPROC, "glGenVertexArrays");
    glBindVertexArray = PROC(PFNGLBINDVERTEXARRAYPROC, "glBindVertexArray");
    glGenBuffers = PROC(PFNGLGENBUFFERSPROC, "glGenBuffers");
    glBindBuffer = PROC(PFNGLBINDBUFFERPROC, "glBindBuffer");
    glBufferData = PROC(PFNGLBUFFERDATAPROC, "glBufferData");
    glVertexAttribPointer = PROC(PFNGLVERTEXATTRIBPOINTERPROC, "glVertexAttribPointer");
    glEnableVertexAttribArray = PROC(PFNGLENABLEVERTEXATTRIBARRAYPROC, "glEnableVertexAttribArray");

    glGetUniformLocation = PROC(PFNGLGETUNIFORMLOCATIONPROC, "glGetUniformLocation");
    glUniform1i = PROC(PFNGLUNIFORM1IPROC, "glUniform1i");
}